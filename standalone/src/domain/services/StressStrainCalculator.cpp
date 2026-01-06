#include "StressStrainCalculator.h"
#include <QtMath>
#include <algorithm>

namespace HorizonUTM {

double StressStrainCalculator::calculateStress(double force, double area) {
    if (area <= 0) return 0.0;
    // Force (N) / Area (mm²) = MPa
    return force / area;
}

double StressStrainCalculator::calculateStrain(double extension, double gaugeLength) {
    if (gaugeLength <= 0) return 0.0;
    // (Extension / Gauge Length) * 100 = %
    return (extension / gaugeLength) * 100.0;
}

TestResult StressStrainCalculator::calculateResults(const QVector<SensorData>& data, 
                                                     double area, 
                                                     double gaugeLength) {
    TestResult result;
    
    if (data.isEmpty() || area <= 0 || gaugeLength <= 0) {
        return result;
    }
    
    // Max stress and strain
    result.maxStress = findMaxStress(data);
    result.maxStrain = findStrainAtMaxStress(data);
    
    // Yield stress (0.2% offset)
    result.yieldStress = calculateYieldStress(data, 0.2);
    
    // Find yield strain
    for (const auto& point : data) {
        if (point.stress >= result.yieldStress) {
            result.yieldStrain = point.strain;
            break;
        }
    }
    
    // Ultimate tensile strength
    result.ultimateStress = findUltimateTensileStrength(data);
    
    // Find ultimate strain
    for (const auto& point : data) {
        if (point.stress >= result.ultimateStress) {
            result.ultimateStrain = point.strain;
            break;
        }
    }
    
    // Break stress and strain (last point)
    if (!data.isEmpty()) {
        result.breakStress = data.last().stress;
        result.breakStrain = data.last().strain;
    }
    
    // Elastic modulus
    result.elasticModulus = calculateElasticModulus(data);
    
    // Elongation at break
    result.elongationAtBreak = calculateElongationAtBreak(data, gaugeLength);
    
    return result;
}

double StressStrainCalculator::findMaxStress(const QVector<SensorData>& data) {
    if (data.isEmpty()) return 0.0;
    
    double maxStress = 0.0;
    for (const auto& point : data) {
        if (point.stress > maxStress) {
            maxStress = point.stress;
        }
    }
    return maxStress;
}

double StressStrainCalculator::findStrainAtMaxStress(const QVector<SensorData>& data) {
    if (data.isEmpty()) return 0.0;
    
    double maxStress = 0.0;
    double strainAtMax = 0.0;
    
    for (const auto& point : data) {
        if (point.stress > maxStress) {
            maxStress = point.stress;
            strainAtMax = point.strain;
        }
    }
    return strainAtMax;
}

double StressStrainCalculator::calculateYieldStress(const QVector<SensorData>& data, 
                                                     double offsetPercent) {
    if (data.size() < 10) return 0.0;
    
    // Calculate elastic modulus first
    double modulus = calculateElasticModulus(data);
    if (modulus <= 0) return 0.0;
    
    // Find 0.2% offset line: stress = modulus * (strain - 0.2)
    // Find where actual curve intersects this line
    
    for (int i = 1; i < data.size(); ++i) {
        double offsetStress = (modulus * 1000.0) * ((data[i].strain - offsetPercent) / 100.0);
        
        if (data[i].stress >= offsetStress && offsetStress > 0) {
            return data[i].stress;
        }
    }
    
    // If no yield found, return 0
    return 0.0;
}

double StressStrainCalculator::calculateElasticModulus(const QVector<SensorData>& data) {
    if (data.size() < 10) return 0.0;
    
    // Find linear region (typically first 20-30% of data before yield)
    int startIdx, endIdx;
    if (!findLinearRegion(data, startIdx, endIdx)) {
        return 0.0;
    }
    
    // Extract strain and stress for linear region
    QVector<double> strains, stresses;
    for (int i = startIdx; i <= endIdx && i < data.size(); ++i) {
        strains.append(data[i].strain / 100.0); // Convert % to fraction
        stresses.append(data[i].stress);
    }
    
    // Perform linear regression
    double slope, intercept;
    linearRegression(strains, stresses, slope, intercept);
    
    // Slope is in MPa per strain fraction
    // Convert to GPa: divide by 1000
    return slope / 1000.0;
}

double StressStrainCalculator::findUltimateTensileStrength(const QVector<SensorData>& data) {
    // Ultimate tensile strength is the maximum stress
    return findMaxStress(data);
}

double StressStrainCalculator::calculateElongationAtBreak(const QVector<SensorData>& data, 
                                                          double gaugeLength) {
    if (data.isEmpty() || gaugeLength <= 0) return 0.0;
    
    // Elongation at break is the final strain
    return data.last().strain;
}

bool StressStrainCalculator::findLinearRegion(const QVector<SensorData>& data, 
                                              int& startIdx, 
                                              int& endIdx) {
    if (data.size() < 10) return false;
    
    // Start from point where stress is significant (skip initial noise)
    startIdx = 0;
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].stress > 1.0) { // 1 MPa threshold
            startIdx = i;
            break;
        }
    }
    
    // Linear region typically ends at ~0.5% strain or 30% of max stress
    double maxStress = findMaxStress(data);
    double thresholdStress = maxStress * 0.3;
    
    endIdx = startIdx;
    for (int i = startIdx; i < data.size(); ++i) {
        if (data[i].strain > 0.5 || data[i].stress > thresholdStress) {
            endIdx = i;
            break;
        }
    }
    
    // Need at least 5 points for good regression
    return (endIdx - startIdx) >= 5;
}

void StressStrainCalculator::linearRegression(const QVector<double>& xData, 
                                              const QVector<double>& yData,
                                              double& slope, 
                                              double& intercept) {
    int n = qMin(xData.size(), yData.size());
    if (n < 2) {
        slope = 0;
        intercept = 0;
        return;
    }
    
    // Calculate means
    double xMean = 0, yMean = 0;
    for (int i = 0; i < n; ++i) {
        xMean += xData[i];
        yMean += yData[i];
    }
    xMean /= n;
    yMean /= n;
    
    // Calculate slope and intercept
    double numerator = 0, denominator = 0;
    for (int i = 0; i < n; ++i) {
        numerator += (xData[i] - xMean) * (yData[i] - yMean);
        denominator += (xData[i] - xMean) * (xData[i] - xMean);
    }
    
    if (denominator != 0) {
        slope = numerator / denominator;
        intercept = yMean - slope * xMean;
    } else {
        slope = 0;
        intercept = yMean;
    }
}

} // namespace HorizonUTM
