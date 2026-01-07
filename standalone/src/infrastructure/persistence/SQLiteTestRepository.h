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
    
    Test getTest(int testId) const override;
    QVector<Test> getAllTests() const override;
    QVector<Test> getTestsByStatus(TestStatus status) const override;
    QVector<Test> getTestsByDateRange(const QDateTime& start, const QDateTime& end) const override;
    
    // Data points operations
    bool saveDataPoints(int testId, const QVector<SensorData>& data) override;
    QVector<SensorData> getDataPoints(int testId) const override;
    bool deleteDataPoints(int testId) override;
    
    // Sample queue operations
    bool saveSample(const Sample& sample) override;
    bool updateSample(const Sample& sample) override;
    bool deleteSample(int sampleId) override;
    
    Sample getSample(int sampleId) const override;
    QVector<Sample> getAllSamples() const override;
    QVector<Sample> getSamplesByStatus(SampleStatus status) const override;
    
    // Statistics
    int getTestCount() const override;
    int getTestCountByStatus(TestStatus status) const override;

private:
    /**
     * @brief Get database connection
     */
    QSqlDatabase getDatabase() const;
    
    /**
     * @brief Convert Test entity to database row
     */
    bool insertTest(const Test& test, int& outId) const;
    
    /**
     * @brief Update existing test in database
     */
    bool updateTestInDb(const Test& test) const;
    
    /**
     * @brief Convert database row to Test entity
     */
    Test testFromQuery(const QSqlQuery& query) const;
    
    /**
     * @brief Convert Sample entity to database row
     */
    bool insertSample(const Sample& sample, int& outId) const;
    
    /**
     * @brief Convert database row to Sample entity
     */
    Sample sampleFromQuery(const QSqlQuery& query) const;
};

} // namespace HorizonUTM
