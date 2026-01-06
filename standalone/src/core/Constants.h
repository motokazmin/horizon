#pragma once

#include <QString>

namespace HorizonUTM {
namespace Constants {

// Application
constexpr const char* APP_NAME = "Horizon UTM";
constexpr const char* APP_VERSION = "1.0.0-MVP";
constexpr const char* APP_ORGANIZATION = "Horizon Materials Testing";

// Database
constexpr const char* DB_NAME = "horizon_utm.db";

// Hardware
constexpr int DEFAULT_SAMPLING_RATE_HZ = 100;
constexpr double DEFAULT_SPEED_MM_PER_MIN = 5.0;
constexpr double DEFAULT_FORCE_LIMIT_N = 10000.0;

// Test Methods
constexpr const char* METHOD_ISO_527_2 = "ISO 527-2";
constexpr const char* METHOD_ASTM_D638 = "ASTM D638";

// UI
constexpr int CHART_MAX_POINTS = 5000;
constexpr int METRICS_UPDATE_INTERVAL_MS = 50;

} // namespace Constants
} // namespace HorizonUTM
