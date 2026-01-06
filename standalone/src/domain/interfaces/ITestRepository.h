#pragma once

#include <QVector>
#include <memory>
#include "domain/entities/Test.h"
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
    
    /**
     * @brief Save a new test to the repository
     * @param test Test entity to save
     * @return true if save successful
     */
    virtual bool saveTest(const Test& test) = 0;
    
    /**
     * @brief Update an existing test
     * @param test Test entity with updated data
     * @return true if update successful
     */
    virtual bool updateTest(const Test& test) = 0;
    
    /**
     * @brief Retrieve a test by ID
     * @param testId Unique test identifier
     * @return Test entity or nullptr if not found
     */
    virtual std::unique_ptr<Test> getTest(int testId) = 0;
    
    /**
     * @brief Get all tests from repository
     * @return Vector of all tests
     */
    virtual QVector<Test> getAllTests() = 0;
    
    /**
     * @brief Save sensor data points for a test
     * @param testId Test identifier
     * @param data Vector of sensor data points
     * @return true if save successful
     */
    virtual bool saveDataPoints(int testId, const QVector<SensorData>& data) = 0;
    
    /**
     * @brief Retrieve all data points for a test
     * @param testId Test identifier
     * @return Vector of sensor data points
     */
    virtual QVector<SensorData> getDataPoints(int testId) = 0;
    
    /**
     * @brief Delete a test and its data points
     * @param testId Test identifier
     * @return true if deletion successful
     */
    virtual bool deleteTest(int testId) = 0;
};

} // namespace HorizonUTM
