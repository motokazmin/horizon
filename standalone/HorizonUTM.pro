#-------------------------------------------------
# Horizon UTM - Qt Creator Project File
# Universal Testing Machine Control Software
# Compatible with Qt Creator 15.0.0+ and Qt 6.4.0+
# Using QCustomPlot instead of Qt Charts
#-------------------------------------------------

QT += core gui widgets sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Use C++20 (or C++2a for older compilers)
CONFIG += c++20

# Compiler-specific flags
msvc {
    QMAKE_CXXFLAGS += /std:c++20
}

gcc {
    # Try c++20, fallback to c++2a for older GCC versions
    QMAKE_CXXFLAGS += -std=c++2a
}

clang {
    QMAKE_CXXFLAGS += -std=c++20
}

VERSION = 1.0.0

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QCUSTOMPLOT_USE_LIBRARY

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Application name
TARGET = HorizonUTM
TEMPLATE = app

# Output directories
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/build/debug
    OBJECTS_DIR = $$PWD/build/debug/obj
    MOC_DIR = $$PWD/build/debug/moc
    RCC_DIR = $$PWD/build/debug/rcc
    UI_DIR = $$PWD/build/debug/ui
}
CONFIG(release, debug|release) {
    DESTDIR = $$PWD/build/release
    OBJECTS_DIR = $$PWD/build/release/obj
    MOC_DIR = $$PWD/build/release/moc
    RCC_DIR = $$PWD/build/release/rcc
    UI_DIR = $$PWD/build/release/ui
}

# Include paths
INCLUDEPATH += $$PWD/src

#-------------------------------------------------
# Source Files
#-------------------------------------------------

SOURCES += \
    src/main.cpp \
    # Third-party: QCustomPlot
    src/third_party/qcustomplot/qcustomplot.cpp \
    # Core
    src/core/Logger.cpp \
    src/core/Config.cpp \
    # Domain - Entities
    src/domain/entities/Test.cpp \
    src/domain/entities/Sample.cpp \
    src/domain/entities/TestMethod.cpp \
    # Domain - Services
    src/domain/services/StressStrainCalculator.cpp \
    src/domain/services/TestMethodValidator.cpp \
    # Infrastructure - Hardware
    src/infrastructure/hardware/MockUTMDriver.cpp \
    # Infrastructure - Persistence
    src/infrastructure/persistence/DatabaseManager.cpp \
    src/infrastructure/persistence/SQLiteTestRepository.cpp \
    # Infrastructure - Export
    src/infrastructure/export/CSVExportService.cpp \
    # Application - Controllers
    src/application/controllers/TestController.cpp \
    src/application/controllers/HardwareController.cpp \
    src/application/controllers/DataExportController.cpp \
    # Presentation - Main Window
    src/presentation/MainWindow.cpp \
    # Presentation - Views
    src/presentation/views/DashboardView.cpp \
    src/presentation/views/SampleQueueView.cpp \
    src/presentation/views/SettingsDialog.cpp \
    # Presentation - Widgets
    src/presentation/widgets/MetricWidget.cpp \
    src/presentation/widgets/RealtimeChartWidget.cpp \
    src/presentation/widgets/StatusIndicator.cpp

#-------------------------------------------------
# Header Files
#-------------------------------------------------

HEADERS += \
    # Third-party: QCustomPlot
    src/third_party/qcustomplot/qcustomplot.h \
    # Core
    src/core/Logger.h \
    src/core/Config.h \
    src/core/Constants.h \
    # Domain - Interfaces
    src/domain/interfaces/IUTMDriver.h \
    src/domain/interfaces/ITestRepository.h \
    src/domain/interfaces/IExportService.h \
    # Domain - Entities
    src/domain/entities/Test.h \
    src/domain/entities/Sample.h \
    src/domain/entities/TestMethod.h \
    # Domain - Value Objects
    src/domain/value_objects/SensorData.h \
    src/domain/value_objects/TestResult.h \
    src/domain/value_objects/MachineState.h \
    # Domain - Services
    src/domain/services/StressStrainCalculator.h \
    src/domain/services/TestMethodValidator.h \
    # Infrastructure - Hardware
    src/infrastructure/hardware/MockUTMDriver.h \
    # Infrastructure - Persistence
    src/infrastructure/persistence/DatabaseManager.h \
    src/infrastructure/persistence/SQLiteTestRepository.h \
    # Infrastructure - Export
    src/infrastructure/export/CSVExportService.h \
    # Application - Controllers
    src/application/controllers/TestController.h \
    src/application/controllers/HardwareController.h \
    src/application/controllers/DataExportController.h \
    # Application - DTOs
    src/application/dto/TestParametersDTO.h \
    src/application/dto/TestResultDTO.h \
    # Presentation
    src/presentation/MainWindow.h \
    src/presentation/views/DashboardView.h \
    src/presentation/views/SampleQueueView.h \
    src/presentation/views/SettingsDialog.h \
    src/presentation/widgets/MetricWidget.h \
    src/presentation/widgets/RealtimeChartWidget.h \
    src/presentation/widgets/StatusIndicator.h

#-------------------------------------------------
# Resources
#-------------------------------------------------

# Uncomment when resources are added
# RESOURCES += resources/resources.qrc

#-------------------------------------------------
# Deployment
#-------------------------------------------------

# Copy database schema on build
copydata.commands = $(COPY_DIR) $$PWD/resources $$DESTDIR
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#-------------------------------------------------
# Documentation
#-------------------------------------------------

DISTFILES += \
    README.md \
    BUILD_INSTRUCTIONS.md \
    QUICK_START.md \
    PROJECT_STRUCTURE.txt \
    CMakeLists.txt \
    docs/architecture.md \
    docs/mvp_scope.md \
    docs/phase2_plan.md \
    resources/database/schema.sql \
    resources/styles/dark_theme.qss \
    verify_setup.bat \
    quick_build.bat
