/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file company_extended_func.h Functions for extended company features. */

#ifndef COMPANY_EXTENDED_FUNC_H
#define COMPANY_EXTENDED_FUNC_H

#include "company_type.h"
#include "economy_type.h"

uint GetHoldingCompanyCount();
uint GetSubsidiaryCompanyCount();
bool CanCreateSubsidiary(CompanyID parent);
bool IsSubsidiaryOf(CompanyID company, CompanyID parent);
Money CalculateInfrastructureFee(CompanyID user, CompanyID owner, uint tiles);
bool TransferFundsBetweenCompanies(CompanyID from, CompanyID to, Money amount);
void InitializeInfrastructureFees(Company *c);

#endif /* COMPANY_EXTENDED_FUNC_H */
