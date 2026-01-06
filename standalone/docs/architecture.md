# Horizon UTM Architecture

## Clean Architecture Layers

### 1. Presentation Layer (UI)
- Qt Widgets-based user interface
- Views: DashboardView, SampleQueueView, SettingsDialog
- Custom Widgets: MetricWidget, RealtimeChartWidget, StatusIndicator

### 2. Application Layer (Use Cases)
- Controllers orchestrate business logic
- TestController: Manages test lifecycle
- HardwareController: Manages hardware connection
- DataExportController: Handles data export

### 3. Domain Layer (Business Logic)
- Entities: Test, Sample, TestMethod
- Value Objects: SensorData, TestResult, MachineState
- Services: StressStrainCalculator, TestMethodValidator
- Interfaces: IUTMDriver, ITestRepository, IExportService

### 4. Infrastructure Layer (External Concerns)
- Hardware: MockUTMDriver, TiniusOlsenDriver (stub)
- Persistence: DatabaseManager, SQLiteTestRepository
- Export: CSVExportService

## Dependency Flow
```
Presentation → Application → Domain ← Infrastructure
```

All dependencies point inward toward the Domain layer.
