cmake_minimum_required(VERSION 3.20)
project(HorizonUTM VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Widgets 
    Charts 
    Sql
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCES
    # Main
    src/main.cpp
    
    # Core
    src/core/Logger.cpp
    src/core/Config.cpp
    
    # Domain - Entities
    src/domain/entities/Test.cpp
    src/domain/entities/Sample.cpp
    src/domain/entities/TestMethod.cpp
    
    # Domain - Services
    src/domain/services/StressStrainCalculator.cpp
    src/domain/services/TestMethodValidator.cpp
    
    # Infrastructure - Hardware
    src/infrastructure/hardware/MockUTMDriver.cpp
    
    # Infrastructure - Persistence
    src/infrastructure/persistence/DatabaseManager.cpp
    src/infrastructure/persistence/SQLiteTestRepository.cpp
    
    # Infrastructure - Export
    src/infrastructure/export/CSVExportService.cpp
    
    # Application - Controllers
    src/application/controllers/TestController.cpp
    src/application/controllers/HardwareController.cpp
    src/application/controllers/DataExportController.cpp
    
    # Presentation - Main Window
    src/presentation/MainWindow.cpp
    
    # Presentation - Views
    src/presentation/views/DashboardView.cpp
    src/presentation/views/SampleQueueView.cpp
    src/presentation/views/SettingsDialog.cpp
    
    # Presentation - Widgets
    src/presentation/widgets/MetricWidget.cpp
    src/presentation/widgets/RealtimeChartWidget.cpp
    src/presentation/widgets/StatusIndicator.cpp
)

set(HEADERS
    # Core
    src/core/Logger.h
    src/core/Config.h
    src/core/Constants.h
    
    # Domain - Interfaces
    src/domain/interfaces/IUTMDriver.h
    src/domain/interfaces/ITestRepository.h
    src/domain/interfaces/IExportService.h
    
    # Domain - Entities
    src/domain/entities/Test.h
    src/domain/entities/Sample.h
    src/domain/entities/TestMethod.h
    
    # Domain - Value Objects
    src/domain/value_objects/SensorData.h
    src/domain/value_objects/TestResult.h
    src/domain/value_objects/MachineState.h
    
    # Domain - Services
    src/domain/services/StressStrainCalculator.h
    src/domain/services/TestMethodValidator.h
    
    # Infrastructure - Hardware
    src/infrastructure/hardware/MockUTMDriver.h
    
    # Infrastructure - Persistence
    src/infrastructure/persistence/DatabaseManager.h
    src/infrastructure/persistence/SQLiteTestRepository.h
    
    # Infrastructure - Export
    src/infrastructure/export/CSVExportService.h
    
    # Application - Controllers
    src/application/controllers/TestController.h
    src/application/controllers/HardwareController.h
    src/application/controllers/DataExportController.h
    
    # Application - DTOs
    src/application/dto/TestParametersDTO.h
    src/application/dto/TestResultDTO.h
    
    # Presentation
    src/presentation/MainWindow.h
    src/presentation/views/DashboardView.h
    src/presentation/views/SampleQueueView.h
    src/presentation/views/SettingsDialog.h
    src/presentation/widgets/MetricWidget.h
    src/presentation/widgets/RealtimeChartWidget.h
    src/presentation/widgets/StatusIndicator.h
)

# Create executable
add_executable(HorizonUTM ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(HorizonUTM PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    Qt6::Sql
)

# Windows specific settings
if(WIN32)
    set_target_properties(HorizonUTM PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

# Copy resources to build directory
add_custom_command(TARGET HorizonUTM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)

# Testing
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS HorizonUTM DESTINATION bin)
install(DIRECTORY resources/ DESTINATION bin/resources)
