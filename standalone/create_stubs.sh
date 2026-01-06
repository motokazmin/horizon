#!/bin/bash

# Domain Entities
cat > src/domain/entities/Test.h << 'EOF'
#pragma once
// Phase 2: Test entity
namespace HorizonUTM { class Test {}; }
EOF

cat > src/domain/entities/Test.cpp << 'EOF'
#include "Test.h"
// Phase 2 implementation
EOF

cat > src/domain/entities/Sample.h << 'EOF'
#pragma once
namespace HorizonUTM { class Sample {}; }
EOF

cat > src/domain/entities/Sample.cpp << 'EOF'
#include "Sample.h"
EOF

cat > src/domain/entities/TestMethod.h << 'EOF'
#pragma once
namespace HorizonUTM { class TestMethod {}; }
EOF

cat > src/domain/entities/TestMethod.cpp << 'EOF'
#include "TestMethod.h"
EOF

# Domain Services
cat > src/domain/services/StressStrainCalculator.h << 'EOF'
#pragma once
namespace HorizonUTM { class StressStrainCalculator {}; }
EOF

cat > src/domain/services/StressStrainCalculator.cpp << 'EOF'
#include "StressStrainCalculator.h"
EOF

cat > src/domain/services/TestMethodValidator.h << 'EOF'
#pragma once
namespace HorizonUTM { class TestMethodValidator {}; }
EOF

cat > src/domain/services/TestMethodValidator.cpp << 'EOF'
#include "TestMethodValidator.h"
EOF

# Infrastructure - Hardware
cat > src/infrastructure/hardware/MockUTMDriver.h << 'EOF'
#pragma once
#include "domain/interfaces/IUTMDriver.h"
namespace HorizonUTM { class MockUTMDriver : public IUTMDriver {}; }
EOF

cat > src/infrastructure/hardware/MockUTMDriver.cpp << 'EOF'
#include "MockUTMDriver.h"
// Phase 3 implementation
EOF

# Infrastructure - Persistence
cat > src/infrastructure/persistence/DatabaseManager.h << 'EOF'
#pragma once
namespace HorizonUTM { class DatabaseManager {}; }
EOF

cat > src/infrastructure/persistence/DatabaseManager.cpp << 'EOF'
#include "DatabaseManager.h"
EOF

cat > src/infrastructure/persistence/SQLiteTestRepository.h << 'EOF'
#pragma once
#include "domain/interfaces/ITestRepository.h"
namespace HorizonUTM { class SQLiteTestRepository : public ITestRepository {}; }
EOF

cat > src/infrastructure/persistence/SQLiteTestRepository.cpp << 'EOF'
#include "SQLiteTestRepository.h"
EOF

# Infrastructure - Export
cat > src/infrastructure/export/CSVExportService.h << 'EOF'
#pragma once
#include "domain/interfaces/IExportService.h"
namespace HorizonUTM { class CSVExportService : public IExportService {}; }
EOF

cat > src/infrastructure/export/CSVExportService.cpp << 'EOF'
#include "CSVExportService.h"
EOF

# Application - Controllers
cat > src/application/controllers/TestController.h << 'EOF'
#pragma once
#include <QObject>
namespace HorizonUTM { class TestController : public QObject {}; }
EOF

cat > src/application/controllers/TestController.cpp << 'EOF'
#include "TestController.h"
EOF

cat > src/application/controllers/HardwareController.h << 'EOF'
#pragma once
#include <QObject>
namespace HorizonUTM { class HardwareController : public QObject {}; }
EOF

cat > src/application/controllers/HardwareController.cpp << 'EOF'
#include "HardwareController.h"
EOF

cat > src/application/controllers/DataExportController.h << 'EOF'
#pragma once
#include <QObject>
namespace HorizonUTM { class DataExportController : public QObject {}; }
EOF

cat > src/application/controllers/DataExportController.cpp << 'EOF'
#include "DataExportController.h"
EOF

# Application - DTOs
cat > src/application/dto/TestParametersDTO.h << 'EOF'
#pragma once
namespace HorizonUTM { struct TestParametersDTO {}; }
EOF

cat > src/application/dto/TestResultDTO.h << 'EOF'
#pragma once
namespace HorizonUTM { struct TestResultDTO {}; }
EOF

# Presentation
cat > src/presentation/MainWindow.h << 'EOF'
#pragma once
#include <QMainWindow>
namespace HorizonUTM { class MainWindow : public QMainWindow {}; }
EOF

cat > src/presentation/MainWindow.cpp << 'EOF'
#include "MainWindow.h"
EOF

# Presentation - Views
cat > src/presentation/views/DashboardView.h << 'EOF'
#pragma once
#include <QWidget>
namespace HorizonUTM { class DashboardView : public QWidget {}; }
EOF

cat > src/presentation/views/DashboardView.cpp << 'EOF'
#include "DashboardView.h"
EOF

cat > src/presentation/views/SampleQueueView.h << 'EOF'
#pragma once
#include <QWidget>
namespace HorizonUTM { class SampleQueueView : public QWidget {}; }
EOF

cat > src/presentation/views/SampleQueueView.cpp << 'EOF'
#include "SampleQueueView.h"
EOF

cat > src/presentation/views/SettingsDialog.h << 'EOF'
#pragma once
#include <QDialog>
namespace HorizonUTM { class SettingsDialog : public QDialog {}; }
EOF

cat > src/presentation/views/SettingsDialog.cpp << 'EOF'
#include "SettingsDialog.h"
EOF

# Presentation - Widgets
cat > src/presentation/widgets/MetricWidget.h << 'EOF'
#pragma once
#include <QWidget>
namespace HorizonUTM { class MetricWidget : public QWidget {}; }
EOF

cat > src/presentation/widgets/MetricWidget.cpp << 'EOF'
#include "MetricWidget.h"
EOF

cat > src/presentation/widgets/RealtimeChartWidget.h << 'EOF'
#pragma once
#include <QWidget>
namespace HorizonUTM { class RealtimeChartWidget : public QWidget {}; }
EOF

cat > src/presentation/widgets/RealtimeChartWidget.cpp << 'EOF'
#include "RealtimeChartWidget.h"
EOF

cat > src/presentation/widgets/StatusIndicator.h << 'EOF'
#pragma once
#include <QWidget>
namespace HorizonUTM { class StatusIndicator : public QWidget {}; }
EOF

cat > src/presentation/widgets/StatusIndicator.cpp << 'EOF'
#include "StatusIndicator.h"
EOF

echo "All stub files created!"
