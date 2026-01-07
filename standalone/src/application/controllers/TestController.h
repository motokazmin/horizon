#pragma once

#include <QObject>
#include <QVector>
#include <memory>
#include "domain/entities/Test.h"
#include "domain/entities/Sample.h"
#include "domain/interfaces/ITestRepository.h"
#include "domain/services/StressStrainCalculator.h"
#include "domain/services/TestMethodValidator.h"

namespace HorizonUTM {

/**
 * @brief Main controller for test management
 * 
 * Coordinates test lifecycle, data collection, and calculation
 */
class TestController : public QObject {
    Q_OBJECT

public:
    explicit TestController(ITestRepository* repository, QObject* parent = nullptr);
    ~TestController() override = default;
    
    /**
     * @brief Create a new test
     * @return Test with default parameters
     */
    Test createNewTest();
    
    /**
     * @brief Validate test parameters before starting
     * @return Validation result
     */
    TestMethodValidator::ValidationResult validateTest(const Test& test) const;
    
    /**
     * @brief Save test to repository
     */
    bool saveTest(Test& test);
    
    /**
     * @brief Update existing test
     */
    bool updateTest(const Test& test);
    
    /**
     * @brief Delete test
     */
    bool deleteTest(int testId);
    
    /**
     * @brief Get test by ID
     */
    Test getTest(int testId) const;
    
    /**
     * @brief Get all tests
     */
    QVector<Test> getAllTests() const;
    
    /**
     * @brief Get tests by status
     */
    QVector<Test> getTestsByStatus(TestStatus status) const;
    
    /**
     * @brief Process new sensor data point
     * @param test Test to update
     * @param data Sensor data
     */
    void processSensorData(Test& test, const SensorData& data);
    
    /**
     * @brief Calculate final test results
     * @param test Test to calculate results for
     * @return Calculated results
     */
    TestResult calculateResults(const Test& test);
    
    /**
     * @brief Complete test and calculate final results
     * @param test Test to complete
     */
    void completeTest(Test& test);
    
    // Sample queue management
    bool saveSample(Sample& sample);
    bool updateSample(const Sample& sample);
    bool deleteSample(int sampleId);
    Sample getSample(int sampleId) const;
    QVector<Sample> getAllSamples() const;
    QVector<Sample> getSamplesByStatus(SampleStatus status) const;
    
    /**
     * @brief Get statistics
     */
    int getTestCount() const;
    int getCompletedTestCount() const;

signals:
    /**
     * @brief Emitted when test is saved
     */
    void testSaved(int testId);
    
    /**
     * @brief Emitted when test is updated
     */
    void testUpdated(int testId);
    
    /**
     * @brief Emitted when test is deleted
     */
    void testDeleted(int testId);
    
    /**
     * @brief Emitted when test results are calculated
     */
    void resultsCalculated(const TestResult& results);

private:
    ITestRepository* m_repository;
};

} // namespace HorizonUTM
