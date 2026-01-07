#include "TestResultDTO.h"
#include "domain/entities/Test.h"

namespace HorizonUTM {

TestResultDTO TestResultDTO::fromTest(const Test& test) {
    TestResultDTO dto;
    
    // Identification
    dto.testId = test.getId();
    dto.sampleName = test.getSampleName();
    dto.operatorName = test.getOperatorName();
    dto.testMethod = test.getTestMethod();
    
    // Timing
    dto.startTime = test.getStartTime();
    dto.endTime = test.getEndTime();
    dto.durationSeconds = test.getDuration();
    
    // Geometry
    dto.width = test.getWidth();
    dto.thickness = test.getThickness();
    dto.gaugeLength = test.getGaugeLength();
    dto.crossSection = test.getCrossSectionArea();
    
    // Parameters
    dto.speed = test.getSpeed();
    dto.temperature = test.getTemperature();
    
    // Results
    const TestResult& result = test.getResult();
    dto.maxStress = result.maxStress;
    dto.yieldStress = result.yieldStress;
    dto.ultimateStress = result.ultimateStress;
    dto.breakStress = result.breakStress;
    
    dto.maxStrain = result.maxStrain;
    dto.yieldStrain = result.yieldStrain;
    dto.ultimateStrain = result.ultimateStrain;
    dto.breakStrain = result.breakStrain;
    
    dto.elasticModulus = result.elasticModulus;
    dto.elongationAtBreak = result.elongationAtBreak;
    
    // Statistics
    dto.dataPointCount = test.getDataPointCount();
    
    return dto;
}

} // namespace HorizonUTM
