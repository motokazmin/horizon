#pragma once

#include "domain/interfaces/ITestRepository.h"
#include "domain/entities/Test.h"
#include "domain/entities/Sample.h"
#include "domain/value_objects/SensorData.h"
#include <QSqlDatabase>

namespace HorizonUTM {

/**
 * @brief SQLite implementation of test repository
 */
class SQLiteTestRepository : public ITestRepository {
public:
    explicit SQLiteTestRepository();
    ~SQLiteTestRepository() override = default;
    
    // Test operations
    bool saveTest(const Test& test) override;
    bool updateTest(const Test& test) override;
    bool deleteTest(int testId) override;
    
    Test getTest(int testId) override;
    QVector<Test> getAllTests() override;
    QVector<Test> getTestsByStatus(TestStatus status) override;
    QVector<Test> getTestsByDateRange(const QDateTime& start, const QDateTime& end) override;
    
    // Data points operations
    bool saveDataPoints(int testId, const QVector<SensorData>& data) override;
    QVector<SensorData> getDataPoints(int testId) override;
    bool deleteDataPoints(int testId) override;
    
    // Sample queue operations
    bool saveSample(const Sample& sample) override;
    bool updateSample(const Sample& sample) override;
    bool deleteSample(int sampleId) override;
    
    Sample getSample(int sampleId) override;
    QVector<Sample> getAllSamples() override;
    QVector<Sample> getSamplesByStatus(SampleStatus status) override;
    
    // Statistics
    int getTestCount() override;
    int getTestCountByStatus(TestStatus status) override;

private:
    /**
     * @brief Get database connection
     */
    QSqlDatabase getDatabase() const;
    
    /**
     * @brief Convert Test entity to database row
     */
    bool insertTest(const Test& test, int& outId);
    
    /**
     * @brief Update existing test in database
     */
    bool updateTestInDb(const Test& test);
    
    /**
     * @brief Convert database row to Test entity
     */
    Test testFromQuery(const QSqlQuery& query);
    
    /**
     * @brief Convert Sample entity to database row
     */
    bool insertSample(const Sample& sample, int& outId);
    
    /**
     * @brief Convert database row to Sample entity
     */
    Sample sampleFromQuery(const QSqlQuery& query);
};

} // namespace HorizonUTM
