# Extended Company Features - README

This directory contains the foundation for advanced economic and multiplayer features in OpenTTD.

## Quick Start

### For Users

These features are **not yet functional** - this is a foundational framework only. To enable the settings (which won't do anything yet):

1. Start OpenTTD
2. Go to Settings → Economy
3. Scroll to "Extended Company Features" section
4. Enable desired features (note: GUI and functionality not implemented yet)

### For Developers

To continue implementing these features, see `docs/company_extended_features.md` for:
- Complete architecture documentation
- Implementation roadmap
- Technical specifications
- Integration guidelines

## What's Included

### ✓ Implemented (Foundation Only)

1. **Data Structures**: All type definitions and data structures
2. **Core Functions**: Basic helper functions for feature logic
3. **Settings**: Configuration framework for all features
4. **Documentation**: Complete technical documentation

### ✗ Not Yet Implemented

1. **GUI**: No user interface
2. **Save/Load**: Data not persisted
3. **Network Commands**: Not multiplayer-safe
4. **Active Integration**: Features don't affect gameplay
5. **Testing**: No test coverage

## Features Overview

### Company Hierarchies
- Holdings and subsidiaries system
- Subsidiaries don't count toward company limit
- Fund transfers between related companies

### Infrastructure Sharing
- Companies can share rail, road, stations, airports
- Configurable usage fees
- Reduced fees for subsidiaries

### Asset Trading
- Buy/sell vehicles and infrastructure
- Offer/acceptance system
- Asset valuation

### Stock Market
- Buy/sell company shares
- Dividend distribution
- Ownership tracking

## File Structure

```
src/
  ├── company_extended_type.h     # Type definitions
  ├── company_extended_func.h     # Function declarations
  ├── company_extended.cpp        # Implementation
  ├── company_base.h              # Extended with new fields
  ├── company_cmd.cpp             # Modified for initialization
  ├── settings_type.h             # New settings
  ├── lang/english.txt            # UI strings
  └── table/settings/
      └── economy_settings.ini    # Setting definitions

docs/
  └── company_extended_features.md  # Complete documentation
```

## Building

The code compiles successfully with the existing OpenTTD build system:

```bash
mkdir build && cd build
cmake .. -DOPTION_DEDICATED=ON
make
```

## Next Steps

See the detailed roadmap in `docs/company_extended_features.md`. The most logical next step is Phase 2: Infrastructure Sharing, as it's the most self-contained feature.

## Contributing

This is foundational work. To make these features functional:

1. Read the architecture documentation
2. Choose a feature phase to implement
3. Follow OpenTTD's coding standards
4. Ensure multiplayer safety
5. Add proper save/load support
6. Create comprehensive tests

## License

This code is part of OpenTTD and follows the same license (GPL v2).

## Credits

Inspired by the JGR Patches infrastructure sharing implementation.
Foundation framework by OpenTTD Contributors.
