#pragma once

#include <QDateTime>
#include <QMetaType>

namespace HorizonUTM {

/**
 * @brief Immutable value object representing a single sensor reading
 * 
 * Contains raw sensor data (force, extension) and calculated values
 * (stress, strain) at a specific point in time
 */
struct SensorData {
    qint64 timestamp;       ///< Milliseconds since epoch
    double force;           ///< Force in Newtons
    double extension;       ///< Extension in millimeters
    double stress;          ///< Calculated stress in MPa
    double strain;          ///< Calculated strain in %
    double temperature;     ///< Temperature in °C
    
    SensorData()
        : timestamp(0)
        , force(0.0)
        , extension(0.0)
        , stress(0.0)
        , strain(0.0)
        , temperature(0.0)
    {}
    
    SensorData(qint64 ts, double f, double ext, double s, double str, double temp)
        : timestamp(ts)
        , force(f)
        , extension(ext)
        , stress(s)
        , strain(str)
        , temperature(temp)
    {}
    
    /**
     * @brief Get timestamp as QDateTime
     */
    QDateTime getDateTime() const {
        return QDateTime::fromMSecsSinceEpoch(timestamp);
    }
    
    /**
     * @brief Check if data point is valid (non-zero timestamp)
     */
    bool isValid() const {
        return timestamp > 0;
    }
};

} // namespace HorizonUTM

// Register with Qt meta-object system for signal/slot usage
Q_DECLARE_METATYPE(HorizonUTM::SensorData)
