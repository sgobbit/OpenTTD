/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file company_extended.cpp Extended company features implementation. */

#include "stdafx.h"
#include "company_base.h"
#include "company_func.h"
#include "company_extended_type.h"
#include "network/network.h"

#include "safeguards.h"

/**
 * Count the number of holding companies (companies that count toward the limit).
 * @return Number of holding companies
 */
uint GetHoldingCompanyCount()
{
	uint count = 0;
	for (const Company *c : Company::Iterate()) {
		if (c->company_type == CHT_HOLDING) {
			count++;
		}
	}
	return count;
}

/**
 * Count the number of subsidiary companies.
 * @return Number of subsidiary companies
 */
uint GetSubsidiaryCompanyCount()
{
	uint count = 0;
	for (const Company *c : Company::Iterate()) {
		if (c->company_type == CHT_SUBSIDIARY) {
			count++;
		}
	}
	return count;
}

/**
 * Check if a company can create a subsidiary.
 * @param parent Parent company ID
 * @return True if the company can create a subsidiary
 */
bool CanCreateSubsidiary(CompanyID parent)
{
	const Company *c = Company::GetIfValid(parent);
	if (c == nullptr) return false;
	
	/* Only holding companies can create subsidiaries */
	if (c->company_type != CHT_HOLDING) return false;
	
	/* Check if there's room for more companies (subsidiaries don't count toward limit) */
	if (Company::GetNumItems() >= Company::GetPoolSize()) return false;
	
	return true;
}

/**
 * Check if a company is a subsidiary of another company.
 * @param company Company to check
 * @param parent Potential parent company
 * @return True if company is a subsidiary of parent
 */
bool IsSubsidiaryOf(CompanyID company, CompanyID parent)
{
	const Company *c = Company::GetIfValid(company);
	if (c == nullptr) return false;
	
	if (c->company_type != CHT_SUBSIDIARY) return false;
	if (c->parent_company != parent) return false;
	
	return true;
}

/**
 * Calculate infrastructure usage fee for a company using another company's infrastructure.
 * @param user Company using the infrastructure
 * @param owner Company owning the infrastructure
 * @param tiles Number of tiles used
 * @return Fee to be paid
 */
Money CalculateInfrastructureFee(CompanyID user, CompanyID owner, uint tiles)
{
	const Company *owner_company = Company::GetIfValid(owner);
	if (owner_company == nullptr) return 0;
	
	/* No fee if infrastructure sharing is disabled */
	if (owner_company->infra_sharing_mode == ISM_DISABLED) return 0;
	
	/* No fee if user is the owner */
	if (user == owner) return 0;
	
	/* Check if user is a subsidiary of owner or vice versa */
	if (IsSubsidiaryOf(user, owner) || IsSubsidiaryOf(owner, user)) {
		/* Reduced fee for subsidiaries */
		return (owner_company->infra_fees.rail_fee_per_tile * tiles) / 2;
	}
	
	return owner_company->infra_fees.rail_fee_per_tile * tiles;
}

/**
 * Transfer funds between a holding and its subsidiary.
 * @param from Company transferring funds
 * @param to Company receiving funds
 * @param amount Amount to transfer
 * @return True if transfer was successful
 */
bool TransferFundsBetweenCompanies(CompanyID from, CompanyID to, Money amount)
{
	Company *from_company = Company::GetIfValid(from);
	Company *to_company = Company::GetIfValid(to);
	
	if (from_company == nullptr || to_company == nullptr) return false;
	if (amount <= 0) return false;
	
	/* Check if companies are in parent-child relationship */
	bool valid_transfer = false;
	if (IsSubsidiaryOf(from, to) || IsSubsidiaryOf(to, from)) {
		valid_transfer = true;
	}
	if (from_company->parent_company == to_company->parent_company && 
	    from_company->parent_company != INVALID_OWNER) {
		/* Sibling subsidiaries under same parent */
		valid_transfer = true;
	}
	
	if (!valid_transfer) return false;
	
	/* Check if from company has enough money */
	if (from_company->money < amount) return false;
	
	/* Perform the transfer */
	from_company->money -= amount;
	to_company->money += amount;
	
	return true;
}

/**
 * Initialize default infrastructure fees for a company.
 * @param c Company to initialize fees for
 */
void InitializeInfrastructureFees(Company *c)
{
	/* Set default fees - these can be configured later */
	c->infra_fees.rail_fee_per_tile = 100;
	c->infra_fees.road_fee_per_tile = 50;
	c->infra_fees.station_fee_per_use = 500;
	c->infra_fees.airport_fee_per_use = 1000;
}
