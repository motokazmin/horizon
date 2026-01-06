#include "TestMethodValidator.h"

namespace HorizonUTM {

TestMethodValidator::ValidationResult TestMethodValidator::validateISO527_2(
    double width, 
    double thickness,
    double gaugeLength,
    double speed) 
{
    ValidationResult result;
    
    // ISO 527-2 specific requirements
    
    // Width: typically 10 ± 0.2 mm for Type 1A specimens
    if (width < 9.5 || width > 10.5) {
        result.addWarning(QString("Width %.2f mm is outside typical range (10 ± 0.5 mm) for ISO 527-2").arg(width));
    }
    
    // Thickness: typically 4 ± 0.2 mm
    if (thickness < 3.5 || thickness > 4.5) {
        result.addWarning(QString("Thickness %.2f mm is outside typical range (4 ± 0.5 mm) for ISO 527-2").arg(thickness));
    }
    
    // Gauge length: typically 50 mm for Type 1A
    if (gaugeLength < 45 || gaugeLength > 55) {
        result.addWarning(QString("Gauge length %.1f mm is outside typical range (50 ± 5 mm) for ISO 527-2").arg(gaugeLength));
    }
    
    // Speed: ISO 527-2 specifies different speeds for different materials
    // Typical: 1-50 mm/min depending on material
    if (speed < 1.0) {
        result.addWarning(QString("Speed %.2f mm/min may be too slow for ISO 527-2").arg(speed));
    }
    if (speed > 50.0) {
        result.addWarning(QString("Speed %.2f mm/min may be too fast for ISO 527-2").arg(speed));
    }
    
    // Recommended speed: 5 mm/min for many plastics
    if (speed < 4.0 || speed > 6.0) {
        result.addWarning(QString("Recommended speed for many plastics is 5 mm/min"));
    }
    
    return result;
}

TestMethodValidator::ValidationResult TestMethodValidator::validateASTM_D638(
    double width, 
    double thickness,
    double gaugeLength,
    double speed) 
{
    ValidationResult result;
    
    // ASTM D638 Type I specimen requirements
    
    // Width of narrow section: 13 ± 0.5 mm
    if (width < 12.0 || width > 14.0) {
        result.addWarning(QString("Width %.2f mm is outside typical range (13 ± 1 mm) for ASTM D638 Type I").arg(width));
    }
    
    // Thickness: typically 3.2 mm (1/8 inch) but can vary
    if (thickness < 2.0 || thickness > 15.0) {
        result.addWarning(QString("Thickness %.2f mm is outside typical range for ASTM D638").arg(thickness));
    }
    
    // Gauge length: 50 mm for Type I
    if (gaugeLength < 45 || gaugeLength > 55) {
        result.addWarning(QString("Gauge length %.1f mm is outside typical range (50 ± 5 mm) for ASTM D638").arg(gaugeLength));
    }
    
    // Speed: ASTM D638 specifies 5 mm/min for many materials
    if (speed < 1.0) {
        result.addWarning(QString("Speed %.2f mm/min may be too slow for ASTM D638").arg(speed));
    }
    if (speed > 50.0) {
        result.addWarning(QString("Speed %.2f mm/min may be too fast for ASTM D638").arg(speed));
    }
    
    return result;
}

TestMethodValidator::ValidationResult TestMethodValidator::validateTestParameters(
    const QString& method,
    double width,
    double thickness,
    double gaugeLength,
    double speed) 
{
    ValidationResult result;
    
    // Basic sanity checks
    if (!isValidDimensions(width, thickness, gaugeLength)) {
        result.addError("Invalid sample dimensions");
        return result;
    }
    
    if (!isValidSpeed(speed)) {
        result.addError(QString("Invalid speed: %.2f mm/min").arg(speed));
        return result;
    }
    
    // Method-specific validation
    if (method == "ISO 527-2") {
        return validateISO527_2(width, thickness, gaugeLength, speed);
    } else if (method == "ASTM D638") {
        return validateASTM_D638(width, thickness, gaugeLength, speed);
    } else {
        result.addWarning(QString("Unknown test method: %1").arg(method));
    }
    
    return result;
}

bool TestMethodValidator::isValidDimensions(double width, double thickness, double gaugeLength) {
    if (width <= 0 || width > 100) return false;  // Max 100mm width
    if (thickness <= 0 || thickness > 50) return false;  // Max 50mm thickness
    if (gaugeLength <= 0 || gaugeLength > 500) return false;  // Max 500mm gauge length
    
    return true;
}

bool TestMethodValidator::isValidSpeed(double speed, double minSpeed, double maxSpeed) {
    return speed >= minSpeed && speed <= maxSpeed;
}

double TestMethodValidator::getRecommendedSpeed(const QString& method) {
    if (method == "ISO 527-2") {
        return 5.0;  // 5 mm/min for many plastics
    } else if (method == "ASTM D638") {
        return 5.0;  // 5 mm/min typical
    }
    return 5.0;  // Default
}

double TestMethodValidator::getRecommendedGaugeLength(const QString& method) {
    if (method == "ISO 527-2") {
        return 50.0;  // 50 mm for Type 1A
    } else if (method == "ASTM D638") {
        return 50.0;  // 50 mm for Type I
    }
    return 50.0;  // Default
}

} // namespace HorizonUTM
