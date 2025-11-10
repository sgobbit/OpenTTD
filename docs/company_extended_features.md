# Extended Company Features - Implementation Documentation

## Overview

This document describes the foundational framework for extended company features in OpenTTD, including infrastructure sharing, company hierarchies (holdings and subsidiaries), asset trading, and a stock market system.

## Current Implementation Status

### Phase 1: Foundation Framework ✓ COMPLETE

The foundation has been implemented with the following components:

#### 1. Data Structures

**New Type Definitions** (`src/company_extended_type.h`):
- `CompanyHierarchyType`: Distinguishes between holding companies and subsidiaries
- `InfrastructureSharingMode`: Controls infrastructure sharing permissions
- `AssetType`: Defines types of assets that can be traded
- `AssetTransferStatus`: Tracks the status of asset transfers
- `AssetTransfer`: Data structure for asset transactions
- `ShareOwnership`: Tracks stock ownership between companies
- `InfrastructureFee`: Fee structure for infrastructure usage

**Extended Company Properties** (`src/company_base.h`):
```cpp
CompanyHierarchyType company_type;        // Holding or subsidiary
CompanyID parent_company;                  // Parent company ID (if subsidiary)
InfrastructureSharingMode infra_sharing_mode;  // Infrastructure sharing mode
InfrastructureFee infra_fees;             // Infrastructure usage fees
```

#### 2. Core Functions

**Company Hierarchy Functions** (`src/company_extended.cpp`):
- `GetHoldingCompanyCount()`: Count holding companies (those that count toward limit)
- `GetSubsidiaryCompanyCount()`: Count subsidiary companies
- `CanCreateSubsidiary()`: Check if a company can create a subsidiary
- `IsSubsidiaryOf()`: Check parent-child relationships

**Infrastructure Sharing Functions**:
- `CalculateInfrastructureFee()`: Calculate fees for infrastructure usage
- `InitializeInfrastructureFees()`: Initialize default fee structure

**Financial Functions**:
- `TransferFundsBetweenCompanies()`: Transfer funds between related companies

#### 3. Configuration Settings

**New Game Settings** (`src/settings_type.h`, `src/table/settings/economy_settings.ini`):
- `enable_company_subsidiaries`: Enable/disable subsidiary system
- `enable_infrastructure_sharing`: Enable/disable infrastructure sharing
- `enable_asset_trading`: Enable/disable asset trading
- `enable_stock_market`: Enable/disable stock market

**Localization** (`src/lang/english.txt`):
All new settings have proper UI strings and help text.

## Architecture Design

### Company Hierarchy System

```
Holding Company (counts toward limit)
├── Subsidiary A (doesn't count)
├── Subsidiary B (doesn't count)
└── Subsidiary C (doesn't count)
```

**Key Features**:
- Only holding companies count toward the max company limit
- Subsidiaries are unlimited (within practical constraints)
- Parent-child relationships are tracked
- Fund transfers allowed between related companies

### Infrastructure Sharing System

**Fee Calculation**:
- Base fees defined per company
- Reduced fees (50%) for subsidiaries
- No fees for self-owned infrastructure

**Fee Types**:
- Rail fee per tile
- Road fee per tile
- Station fee per use
- Airport fee per use

### Asset Trading System (Framework)

**Tradeable Assets**:
- Individual vehicles
- Vehicle groups
- Stations
- Depots
- Infrastructure sections

**Transfer Flow**:
1. Seller proposes asset and price
2. Buyer reviews offer
3. Buyer accepts or rejects
4. On acceptance: ownership transfer and payment

### Stock Market System (Framework)

**Features**:
- Share ownership tracking
- Dividend distribution
- Potential for majority control (future)

## Technical Implementation Details

### Initialization

When a new company is created (`Company::Company()`):
```cpp
this->company_type = CHT_HOLDING;
this->parent_company = INVALID_OWNER;
this->infra_sharing_mode = ISM_DISABLED;
InitializeInfrastructureFees(this);
```

### Data Persistence

**IMPORTANT**: The new fields in `CompanyProperties` will need to be added to the save/load system:
- File: `src/saveload/company_sl.cpp`
- Need to increment save version
- Add proper serialization/deserialization

### Multiplayer Considerations

**Network Safety**:
All new features must be implemented with network commands to ensure:
- Proper synchronization across clients
- Deterministic behavior
- Prevention of desyncs

**Files to Modify** (Future Work):
- `src/command_type.h`: Add new command types
- `src/company_cmd.h`: Add command declarations
- Network validation in all commands

## Next Steps (Future Implementation)

### Phase 2: Infrastructure Sharing
- [ ] Implement infrastructure usage tracking
- [ ] Add fee collection mechanism
- [ ] Create GUI for configuration
- [ ] Add network commands
- [ ] Integrate with save/load system

### Phase 3: Company Hierarchy
- [ ] Implement subsidiary creation command
- [ ] Add GUI for company management
- [ ] Update company limit logic
- [ ] Add network synchronization
- [ ] Test multiplayer scenarios

### Phase 4: Asset Trading
- [ ] Implement asset valuation
- [ ] Create trading GUI
- [ ] Add offer/acceptance system
- [ ] Implement ownership transfer
- [ ] Add transaction history

### Phase 5: Stock Market
- [ ] Implement share tracking
- [ ] Create stock market GUI
- [ ] Add trading mechanism
- [ ] Implement dividend system
- [ ] Add price calculation

## Configuration

All features are disabled by default and can be enabled via the game settings:

**Settings → Economy → Extended Company Features**:
- Enable company subsidiaries
- Enable infrastructure sharing
- Enable asset trading
- Enable stock market

## Compatibility

### Save Game Compatibility
- New save format version required when features are enabled
- Backward compatibility: Old saves load with features disabled
- Forward compatibility: New saves can't be loaded by old versions

### Network Multiplayer
- All clients must have matching feature flags
- Features checked during connection handshake
- Mismatched settings prevent connection

## Known Limitations

1. **GUI Not Implemented**: No user interface for accessing new features yet
2. **Save/Load Not Implemented**: New fields not persisted in save games
3. **Network Commands Missing**: Can't use in multiplayer yet
4. **No Active Usage**: Features are defined but not actively used in game logic

## Testing

Current testing status:
- ✓ Code compiles successfully
- ✓ Settings load without errors
- ✓ Basic functions compile
- ⚠ No automated tests yet
- ⚠ No integration testing
- ⚠ No multiplayer testing

## Contributing

To continue this work:

1. **Pick a Phase**: Choose from phases 2-5 above
2. **Implement Core Logic**: Add the game mechanics
3. **Add Network Commands**: Ensure multiplayer safety
4. **Create GUI**: Make features accessible to players
5. **Add Save/Load**: Persist data in save games
6. **Test Thoroughly**: Especially multiplayer scenarios

## References

- JGR Patches: https://github.com/JGRennison/OpenTTD-patches
- OpenTTD Development: https://www.openttd.org/development
- Network Protocol: See `src/network/network_command.cpp`
- Save Format: See `src/saveload/` directory
