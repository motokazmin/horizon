#pragma once

#include <QString>
#include <QStringList>

namespace HorizonUTM {

/**
 * @brief Service for validating test parameters according to standards
 */
class TestMethodValidator {
public:
    /**
     * @brief Validation result structure
     */
    struct ValidationResult {
        bool isValid;
        QStringList errors;
        QStringList warnings;
        
        ValidationResult() : isValid(true) {}
        
        void addError(const QString& error) {
            errors.append(error);
            isValid = false;
        }
        
        void addWarning(const QString& warning) {
            warnings.append(warning);
        }
        
        bool hasErrors() const { return !errors.isEmpty(); }
        bool hasWarnings() const { return !warnings.isEmpty(); }
    };
    
    /**
     * @brief Validate test parameters for ISO 527-2
     */
    static ValidationResult validateISO527_2(double width, 
                                             double thickness,
                                             double gaugeLength,
                                             double speed);
    
    /**
     * @brief Validate test parameters for ASTM D638
     */
    static ValidationResult validateASTM_D638(double width, 
                                              double thickness,
                                              double gaugeLength,
                                              double speed);
    
    /**
     * @brief Validate test parameters for any method
     */
    static ValidationResult validateTestParameters(const QString& method,
                                                   double width,
                                                   double thickness,
                                                   double gaugeLength,
                                                   double speed);
    
    /**
     * @brief Check if sample dimensions are valid
     */
    static bool isValidDimensions(double width, double thickness, double gaugeLength);
    
    /**
     * @brief Check if test speed is within acceptable range
     */
    static bool isValidSpeed(double speed, double minSpeed = 0.1, double maxSpeed = 500.0);
    
    /**
     * @brief Get recommended speed for method
     */
    static double getRecommendedSpeed(const QString& method);
    
    /**
     * @brief Get recommended gauge length for method
     */
    static double getRecommendedGaugeLength(const QString& method);
};

} // namespace HorizonUTM
