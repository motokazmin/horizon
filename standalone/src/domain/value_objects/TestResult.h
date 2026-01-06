#pragma once

#include <QString>

namespace HorizonUTM {

/**
 * @brief Value object containing calculated test results
 * 
 * Immutable object with all key mechanical properties calculated
 * from the stress-strain curve
 */
struct TestResult {
    double maxStress;           ///< Maximum stress (MPa)
    double maxStrain;           ///< Strain at maximum stress (%)
    double yieldStress;         ///< Yield stress (MPa) - 0.2% offset
    double yieldStrain;         ///< Strain at yield (%)
    double ultimateStress;      ///< Ultimate tensile strength (MPa)
    double ultimateStrain;      ///< Strain at ultimate stress (%)
    double breakStress;         ///< Stress at break (MPa)
    double breakStrain;         ///< Strain at break (%)
    double elasticModulus;      ///< Young's modulus (GPa)
    double elongationAtBreak;   ///< Elongation at break (%)
    
    TestResult()
        : maxStress(0.0)
        , maxStrain(0.0)
        , yieldStress(0.0)
        , yieldStrain(0.0)
        , ultimateStress(0.0)
        , ultimateStrain(0.0)
        , breakStress(0.0)
        , breakStrain(0.0)
        , elasticModulus(0.0)
        , elongationAtBreak(0.0)
    {}
    
    /**
     * @brief Check if results are valid (at least max stress calculated)
     */
    bool isValid() const {
        return maxStress > 0.0;
    }
    
    /**
     * @brief Get formatted string of all results
     */
    QString toString() const {
        return QString(
            "Max Stress: %1 MPa\n"
            "Yield Stress: %2 MPa\n"
            "Ultimate Stress: %3 MPa\n"
            "Elastic Modulus: %4 GPa\n"
            "Elongation at Break: %5 %"
        ).arg(maxStress, 0, 'f', 2)
         .arg(yieldStress, 0, 'f', 2)
         .arg(ultimateStress, 0, 'f', 2)
         .arg(elasticModulus, 0, 'f', 2)
         .arg(elongationAtBreak, 0, 'f', 2);
    }
};

} // namespace HorizonUTM
