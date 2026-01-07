#pragma once

#include <QVector>
#include <QDateTime>
#include "domain/entities/Test.h"
#include "domain/entities/Sample.h"
#include "domain/value_objects/SensorData.h"

namespace HorizonUTM {

/**
 * @brief Interface for test data persistence
 * 
 * This interface abstracts the data access layer, allowing different
 * storage implementations (SQLite, PostgreSQL, etc.)
 */
class ITestRepository {
public:
    virtual ~ITestRepository() = default;
    
    // Test operations
    virtual bool saveTest(const Test& test) = 0;
    virtual bool updateTest(const Test& test) = 0;
    virtual bool deleteTest(int testId) = 0;
    
    virtual Test getTest(int testId) = 0;
    virtual QVector<Test> getAllTests() = 0;
    virtual QVector<Test> getTestsByStatus(TestStatus status) = 0;
    virtual QVector<Test> getTestsByDateRange(const QDateTime& start, const QDateTime& end) = 0;
    
    // Data points operations
    virtual bool saveDataPoints(int testId, const QVector<SensorData>& data) = 0;
    virtual QVector<SensorData> getDataPoints(int testId) = 0;
    virtual bool deleteDataPoints(int testId) = 0;
    
    // Sample queue operations
    virtual bool saveSample(const Sample& sample) = 0;
    virtual bool updateSample(const Sample& sample) = 0;
    virtual bool deleteSample(int sampleId) = 0;
    
    virtual Sample getSample(int sampleId) = 0;
    virtual QVector<Sample> getAllSamples() = 0;
    virtual QVector<Sample> getSamplesByStatus(SampleStatus status) = 0;
    
    // Statistics
    virtual int getTestCount() = 0;
    virtual int getTestCountByStatus(TestStatus status) = 0;
};

} // namespace HorizonUTM
