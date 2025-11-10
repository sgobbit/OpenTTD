/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file company_extended_type.h Types for extended company features (holdings, subsidiaries, infrastructure sharing). */

#ifndef COMPANY_EXTENDED_TYPE_H
#define COMPANY_EXTENDED_TYPE_H

#include "company_type.h"
#include "economy_type.h"

/**
 * Type of company in the hierarchy system.
 * Holdings are main companies that count towards the company limit.
 * Subsidiaries are child companies that don't count towards the limit.
 */
enum CompanyHierarchyType : uint8_t {
	CHT_HOLDING = 0,    ///< A holding company (main company)
	CHT_SUBSIDIARY = 1, ///< A subsidiary company (child company)
};

/**
 * Infrastructure sharing permissions between companies.
 */
enum InfrastructureSharingMode : uint8_t {
	ISM_DISABLED = 0,     ///< Infrastructure sharing is disabled
	ISM_ALLOWED = 1,      ///< Companies can allow others to use their infrastructure
	ISM_RESTRICTED = 2,   ///< Infrastructure sharing with restrictions
};

/**
 * Types of assets that can be traded between companies.
 */
enum AssetType : uint8_t {
	AT_VEHICLE = 0,       ///< Single vehicle or vehicle group
	AT_STATION = 1,       ///< Station
	AT_DEPOT = 2,         ///< Depot
	AT_INFRASTRUCTURE = 3, ///< Infrastructure section (tracks, etc)
};

/**
 * Status of an asset transfer offer.
 */
enum AssetTransferStatus : uint8_t {
	ATS_PENDING = 0,     ///< Offer is pending
	ATS_ACCEPTED = 1,    ///< Offer has been accepted
	ATS_REJECTED = 2,    ///< Offer has been rejected
	ATS_CANCELLED = 3,   ///< Offer has been cancelled
};

/**
 * Data for an asset transfer between companies.
 */
struct AssetTransfer {
	CompanyID seller;         ///< Company selling the asset
	CompanyID buyer;          ///< Company buying the asset
	AssetType asset_type;     ///< Type of asset being transferred
	uint32_t asset_id;        ///< ID of the asset (vehicle, station, etc)
	Money price;              ///< Offered price
	AssetTransferStatus status; ///< Current status of the transfer
};

/**
 * Share ownership data for stock market system.
 */
struct ShareOwnership {
	CompanyID owner;          ///< Company owning the shares
	CompanyID target;         ///< Company whose shares are owned
	uint8_t percentage;       ///< Percentage of shares owned (0-100)
};

/**
 * Infrastructure usage fee structure.
 */
struct InfrastructureFee {
	Money rail_fee_per_tile;     ///< Fee per rail tile used
	Money road_fee_per_tile;     ///< Fee per road tile used
	Money station_fee_per_use;   ///< Fee per station use
	Money airport_fee_per_use;   ///< Fee per airport use
};

#endif /* COMPANY_EXTENDED_TYPE_H */
