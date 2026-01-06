# Horizon UTM - Universal Testing Machine Software

## Overview
Horizon UTM is a professional desktop application for controlling and monitoring Universal Testing Machines (UTM). Built with C++ and Qt6, it provides real-time data acquisition, analysis, and reporting for tensile testing.

## Architecture
This project follows **Clean Architecture** principles with clear separation of concerns:

```
Presentation Layer (Qt UI)
    ↓
Application Layer (Controllers)
    ↓
Domain Layer (Business Logic)
    ↓
Infrastructure Layer (Hardware, Database, Export)
```

### Key Features
- ✅ Real-time stress-strain curve visualization
- ✅ Support for ISO 527-2 and ASTM D638 test methods
- ✅ SQLite database for test data persistence
- ✅ CSV export functionality
- ✅ Mock hardware driver for testing without physical equipment
- ✅ Dark theme UI
- ✅ Extensible architecture for new hardware drivers

## Project Structure

```
HorizonUTM_MVP/
├── src/
│   ├── core/                  # Core utilities (Logger, Config)
│   ├── domain/                # Business logic & interfaces
│   │   ├── entities/          # Domain entities
│   │   ├── value_objects/     # Immutable value objects
│   │   ├── services/          # Domain services
│   │   └── interfaces/        # Contracts for infrastructure
│   ├── infrastructure/        # External concerns
│   │   ├── hardware/          # Hardware drivers
│   │   ├── persistence/       # Database access
│   │   └── export/            # Export services
│   ├── application/           # Use cases & controllers
│   └── presentation/          # UI layer
├── resources/                 # Resources (icons, styles, SQL)
├── tests/                     # Unit tests
└── docs/                      # Documentation
```

## Building

### Prerequisites
- CMake 3.20+
- Qt 6.5+
- C++20 compatible compiler (MSVC 2019+, GCC 11+, Clang 14+)

### Windows Build
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Running
```bash
./bin/HorizonUTM.exe
```

## Configuration
Application data is stored in:
- Windows: `%APPDATA%/Horizon Materials Testing/Horizon UTM/`
- Database: `horizon_utm.db`
- Logs: `logs/`

## MVP Scope
Current MVP includes:
- ✅ Mock hardware driver (simulated UTM)
- ✅ Real-time dashboard with metrics
- ✅ Live stress-strain chart
- ✅ Sample queue management
- ✅ SQLite persistence
- ✅ CSV export

Future phases:
- ❌ Real Tinius Olsen driver
- ❌ PDF/Excel export
- ❌ User authentication
- ❌ Calibration module
- ❌ Advanced analysis

## Extensibility

### Adding a New Hardware Driver
1. Implement `IUTMDriver` interface
2. Register in dependency injection container
3. No changes needed in other layers!

```cpp
class MyCustomDriver : public IUTMDriver {
    // Implement all virtual methods
};
```

### Adding a New Export Format
1. Implement `IExportService` interface
2. Register in export service factory

```cpp
class PDFExportService : public IExportService {
    // Implement export logic
};
```

## License
Proprietary - Horizon Materials Testing

## Contact
For questions or support, contact the development team.
