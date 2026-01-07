#include "TestController.h"
#include "core/Logger.h"

namespace HorizonUTM {

TestController::TestController(ITestRepository* repository, QObject* parent)
    : QObject(parent)
    , m_repository(repository)
{
    LOG_INFO("TestController created");
}

Test TestController::createNewTest() {
    Test test;
    test.setStatus(TestStatus::Ready);
    test.setTestMethod("ISO 527-2");
    test.setSpeed(5.0);
    test.setForceLimit(10000.0);
    test.setTemperature(23.0);
    
    LOG_INFO("New test created");
    return test;
}

TestMethodValidator::ValidationResult TestController::validateTest(const Test& test) const {
    return TestMethodValidator::validateTestParameters(
        test.getTestMethod(),
        test.getWidth(),
        test.getThickness(),
        test.getGaugeLength(),
        test.getSpeed()
    );
}

bool TestController::saveTest(Test& test) {
    if (!test.isValid()) {
        LOG_ERROR("Cannot save invalid test");
        return false;
    }
    
    bool success = m_repository->saveTest(test);
    
    if (success) {
        emit testSaved(test.getId());
        LOG_INFO(QString("Test saved: ID=%1, Sample=%2")
            .arg(test.getId()).arg(test.getSampleName()));
    } else {
        LOG_ERROR("Failed to save test");
    }
    
    return success;
}

bool TestController::updateTest(const Test& test) {
    bool success = m_repository->updateTest(test);
    
    if (success) {
        emit testUpdated(test.getId());
        LOG_INFO(QString("Test updated: ID=%1").arg(test.getId()));
    } else {
        LOG_ERROR(QString("Failed to update test ID=%1").arg(test.getId()));
    }
    
    return success;
}

bool TestController::deleteTest(int testId) {
    bool success = m_repository->deleteTest(testId);
    
    if (success) {
        emit testDeleted(testId);
        LOG_INFO(QString("Test deleted: ID=%1").arg(testId));
    } else {
        LOG_ERROR(QString("Failed to delete test ID=%1").arg(testId));
    }
    
    return success;
}

Test TestController::getTest(int testId) {
    return m_repository->getTest(testId);
}

QVector<Test> TestController::getAllTests() {
    return m_repository->getAllTests();
}

QVector<Test> TestController::getTestsByStatus(TestStatus status) {
    return m_repository->getTestsByStatus(status);
}

void TestController::processSensorData(Test& test, const SensorData& data) {
    // Add data point to test
    test.addDataPoint(data);
    
    // Log periodically (every 100 points)
    if (test.getDataPointCount() % 100 == 0) {
        LOG_DEBUG(QString("Processed %1 data points for test ID=%2")
            .arg(test.getDataPointCount()).arg(test.getId()));
    }
}

TestResult TestController::calculateResults(const Test& test) {
    if (test.getData().isEmpty()) {
        LOG_WARNING("Cannot calculate results: no data");
        return TestResult();
    }
    
    TestResult result = StressStrainCalculator::calculateResults(
        test.getData(),
        test.getCrossSectionArea(),
        test.getGaugeLength()
    );
    
    LOG_INFO(QString("Results calculated: Max Stress=%.2f MPa, E=%.2f GPa")
        .arg(result.maxStress).arg(result.elasticModulus));
    
    emit resultsCalculated(result);
    
    return result;
}

void TestController::completeTest(Test& test) {
    // Calculate final results
    TestResult result = calculateResults(test);
    test.setResult(result);
    
    // Set completion time
    test.setEndTime(QDateTime::currentDateTime());
    test.setStatus(TestStatus::Completed);
    
    // Save to database
    updateTest(test);
    
    LOG_INFO(QString("Test completed: ID=%1, Duration=%2s")
        .arg(test.getId()).arg(test.getDuration()));
}

// Sample management

bool TestController::saveSample(Sample& sample) {
    if (!sample.isValid()) {
        LOG_ERROR("Cannot save invalid sample");
        return false;
    }
    
    bool success = m_repository->saveSample(sample);
    
    if (success) {
        LOG_INFO(QString("Sample saved: ID=%1, Name=%2")
            .arg(sample.getId()).arg(sample.getName()));
    }
    
    return success;
}

bool TestController::updateSample(const Sample& sample) {
    return m_repository->updateSample(sample);
}

bool TestController::deleteSample(int sampleId) {
    return m_repository->deleteSample(sampleId);
}

Sample TestController::getSample(int sampleId) {
    return m_repository->getSample(sampleId);
}

QVector<Sample> TestController::getAllSamples() {
    return m_repository->getAllSamples();
}

QVector<Sample> TestController::getSamplesByStatus(SampleStatus status) {
    return m_repository->getSamplesByStatus(status);
}

// Statistics

int TestController::getTestCount() {
    return m_repository->getTestCount();
}

int TestController::getCompletedTestCount() {
    return m_repository->getTestCountByStatus(TestStatus::Completed);
}

} // namespace HorizonUTM
