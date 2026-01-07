#pragma once

#include <QString>

namespace HorizonUTM {

/**
 * @brief Data Transfer Object for test parameters
 * 
 * Used to transfer test configuration between UI and application layers
 */
struct TestParametersDTO {
    // Sample information
    QString sampleName;
    QString operatorName;
    QString testMethod;
    
    // Sample geometry (mm)
    double width;
    double thickness;
    double gaugeLength;
    
    // Test parameters
    double speed;           // mm/min
    double forceLimit;      // N
    double temperature;     // °C
    
    // Metadata
    QString notes;
    
    /**
     * @brief Default constructor with typical values
     */
    TestParametersDTO()
        : sampleName("")
        , operatorName("")
        , testMethod("ISO 527-2")
        , width(10.0)
        , thickness(4.0)
        , gaugeLength(50.0)
        , speed(5.0)
        , forceLimit(10000.0)
        , temperature(23.0)
        , notes("")
    {}
    
    /**
     * @brief Check if parameters are valid
     */
    bool isValid() const {
        return !sampleName.isEmpty() 
            && !operatorName.isEmpty()
            && width > 0 
            && thickness > 0 
            && gaugeLength > 0
            && speed > 0
            && forceLimit > 0;
    }
    
    /**
     * @brief Get cross-section area
     */
    double getCrossSectionArea() const {
        return width * thickness;
    }
};

} // namespace HorizonUTM
