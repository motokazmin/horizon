#pragma once

#include <QVector>
#include "domain/value_objects/SensorData.h"
#include "domain/value_objects/TestResult.h"

namespace HorizonUTM {

/**
 * @brief Service for calculating stress, strain, and mechanical properties
 * 
 * Implements calculations according to ISO 527 and ASTM D638 standards
 */
class StressStrainCalculator {
public:
    /**
     * @brief Calculate stress from force and cross-section area
     * @param force Force in Newtons
     * @param area Cross-section area in mm²
     * @return Stress in MPa
     */
    static double calculateStress(double force, double area);
    
    /**
     * @brief Calculate strain from extension and gauge length
     * @param extension Extension in mm
     * @param gaugeLength Original gauge length in mm
     * @return Strain in %
     */
    static double calculateStrain(double extension, double gaugeLength);
    
    /**
     * @brief Calculate full test results from sensor data
     * @param data Vector of sensor data points
     * @param area Cross-section area in mm²
     * @param gaugeLength Gauge length in mm
     * @return TestResult with all calculated properties
     */
    static TestResult calculateResults(const QVector<SensorData>& data, 
                                       double area, 
                                       double gaugeLength);
    
    /**
     * @brief Find maximum stress in data
     */
    static double findMaxStress(const QVector<SensorData>& data);
    
    /**
     * @brief Find strain at maximum stress
     */
    static double findStrainAtMaxStress(const QVector<SensorData>& data);
    
    /**
     * @brief Calculate yield stress using 0.2% offset method
     * @param data Sensor data
     * @param offsetPercent Offset percentage (typically 0.2%)
     * @return Yield stress in MPa
     */
    static double calculateYieldStress(const QVector<SensorData>& data, 
                                       double offsetPercent = 0.2);
    
    /**
     * @brief Calculate elastic modulus (Young's modulus)
     * @param data Sensor data
     * @return Elastic modulus in GPa
     */
    static double calculateElasticModulus(const QVector<SensorData>& data);
    
    /**
     * @brief Find ultimate tensile strength (max stress before break)
     */
    static double findUltimateTensileStrength(const QVector<SensorData>& data);
    
    /**
     * @brief Calculate elongation at break
     * @param data Sensor data
     * @param gaugeLength Original gauge length in mm
     * @return Elongation at break in %
     */
    static double calculateElongationAtBreak(const QVector<SensorData>& data, 
                                             double gaugeLength);

private:
    /**
     * @brief Find linear region for modulus calculation
     * @param data Sensor data
     * @param startIdx Output: start index of linear region
     * @param endIdx Output: end index of linear region
     * @return true if linear region found
     */
    static bool findLinearRegion(const QVector<SensorData>& data, 
                                 int& startIdx, 
                                 int& endIdx);
    
    /**
     * @brief Perform linear regression
     * @param xData X values
     * @param yData Y values
     * @param slope Output: slope of line
     * @param intercept Output: y-intercept
     */
    static void linearRegression(const QVector<double>& xData, 
                                 const QVector<double>& yData,
                                 double& slope, 
                                 double& intercept);
};

} // namespace HorizonUTM
