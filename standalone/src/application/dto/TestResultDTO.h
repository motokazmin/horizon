#pragma once

#include <QString>
#include <QDateTime>
#include "domain/value_objects/TestResult.h"

namespace HorizonUTM {

/**
 * @brief Data Transfer Object for test results
 * 
 * Used to display test results in UI
 */
struct TestResultDTO {
    // Test identification
    int testId;
    QString sampleName;
    QString operatorName;
    QString testMethod;
    
    // Timing
    QDateTime startTime;
    QDateTime endTime;
    qint64 durationSeconds;
    
    // Sample geometry
    double width;           // mm
    double thickness;       // mm
    double gaugeLength;     // mm
    double crossSection;    // mm²
    
    // Test parameters
    double speed;           // mm/min
    double temperature;     // °C
    
    // Mechanical properties
    double maxStress;           // MPa
    double yieldStress;         // MPa
    double ultimateStress;      // MPa
    double breakStress;         // MPa
    
    double maxStrain;           // %
    double yieldStrain;         // %
    double ultimateStrain;      // %
    double breakStrain;         // %
    
    double elasticModulus;      // GPa
    double elongationAtBreak;   // %
    
    // Statistics
    int dataPointCount;
    
    /**
     * @brief Default constructor
     */
    TestResultDTO()
        : testId(-1)
        , durationSeconds(0)
        , width(0.0)
        , thickness(0.0)
        , gaugeLength(0.0)
        , crossSection(0.0)
        , speed(0.0)
        , temperature(0.0)
        , maxStress(0.0)
        , yieldStress(0.0)
        , ultimateStress(0.0)
        , breakStress(0.0)
        , maxStrain(0.0)
        , yieldStrain(0.0)
        , ultimateStrain(0.0)
        , breakStrain(0.0)
        , elasticModulus(0.0)
        , elongationAtBreak(0.0)
        , dataPointCount(0)
    {}
    
    /**
     * @brief Create from Test entity
     */
    static TestResultDTO fromTest(const class Test& test);
    
    /**
     * @brief Format duration as string
     */
    QString getDurationString() const {
        int minutes = durationSeconds / 60;
        int seconds = durationSeconds % 60;
        return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
    }
};

} // namespace HorizonUTM
