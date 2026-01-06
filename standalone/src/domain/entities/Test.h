#pragma once

#include <QString>
#include <QDateTime>
#include <QVector>
#include "domain/value_objects/SensorData.h"
#include "domain/value_objects/TestResult.h"

namespace HorizonUTM {

/**
 * @brief Test status enumeration
 */
enum class TestStatus {
    Ready,      ///< Test ready to start
    Running,    ///< Test in progress
    Paused,     ///< Test paused
    Completed,  ///< Test completed successfully
    Failed,     ///< Test failed
    Stopped     ///< Test stopped by user
};

/**
 * @brief Convert TestStatus to string
 */
inline QString testStatusToString(TestStatus status) {
    switch (status) {
        case TestStatus::Ready:     return "Ready";
        case TestStatus::Running:   return "Running";
        case TestStatus::Paused:    return "Paused";
        case TestStatus::Completed: return "Completed";
        case TestStatus::Failed:    return "Failed";
        case TestStatus::Stopped:   return "Stopped";
        default:                    return "Unknown";
    }
}

/**
 * @brief Test entity - represents a tensile test
 * 
 * Domain entity containing all information about a test including
 * parameters, sample information, collected data, and results.
 */
class Test {
public:
    /**
     * @brief Constructor for new test
     */
    Test();
    
    /**
     * @brief Constructor with ID (for loading from database)
     */
    explicit Test(int id);
    
    /**
     * @brief Destructor
     */
    ~Test() = default;
    
    // Getters
    int getId() const { return m_id; }
    QString getSampleName() const { return m_sampleName; }
    QString getOperatorName() const { return m_operatorName; }
    QString getTestMethod() const { return m_testMethod; }
    QDateTime getStartTime() const { return m_startTime; }
    QDateTime getEndTime() const { return m_endTime; }
    TestStatus getStatus() const { return m_status; }
    
    // Sample dimensions
    double getWidth() const { return m_width; }
    double getThickness() const { return m_thickness; }
    double getGaugeLength() const { return m_gaugeLength; }
    double getCrossSectionArea() const { return m_width * m_thickness; }
    
    // Test parameters
    double getSpeed() const { return m_speed; }
    double getForceLimit() const { return m_forceLimit; }
    double getTemperature() const { return m_temperature; }
    
    // Data and results
    const QVector<SensorData>& getData() const { return m_data; }
    const TestResult& getResult() const { return m_result; }
    
    QString getNotes() const { return m_notes; }
    
    // Setters
    void setId(int id) { m_id = id; }
    void setSampleName(const QString& name) { m_sampleName = name; }
    void setOperatorName(const QString& name) { m_operatorName = name; }
    void setTestMethod(const QString& method) { m_testMethod = method; }
    void setStartTime(const QDateTime& time) { m_startTime = time; }
    void setEndTime(const QDateTime& time) { m_endTime = time; }
    void setStatus(TestStatus status) { m_status = status; }
    
    // Sample dimensions
    void setWidth(double width) { m_width = width; }
    void setThickness(double thickness) { m_thickness = thickness; }
    void setGaugeLength(double length) { m_gaugeLength = length; }
    
    // Test parameters
    void setSpeed(double speed) { m_speed = speed; }
    void setForceLimit(double limit) { m_forceLimit = limit; }
    void setTemperature(double temp) { m_temperature = temp; }
    
    // Data management
    void addDataPoint(const SensorData& data);
    void clearData();
    void setData(const QVector<SensorData>& data) { m_data = data; }
    
    // Results
    void setResult(const TestResult& result) { m_result = result; }
    
    void setNotes(const QString& notes) { m_notes = notes; }
    
    /**
     * @brief Validate test parameters
     * @return true if all parameters are valid
     */
    bool isValid() const;
    
    /**
     * @brief Get duration of test
     * @return Duration in seconds
     */
    qint64 getDuration() const;
    
    /**
     * @brief Get number of data points collected
     */
    int getDataPointCount() const { return m_data.size(); }

private:
    // Identification
    int m_id;
    QString m_sampleName;
    QString m_operatorName;
    QString m_testMethod;
    
    // Timing
    QDateTime m_startTime;
    QDateTime m_endTime;
    TestStatus m_status;
    
    // Sample geometry
    double m_width;         // mm
    double m_thickness;     // mm
    double m_gaugeLength;   // mm
    
    // Test parameters
    double m_speed;         // mm/min
    double m_forceLimit;    // N
    double m_temperature;   // °C
    
    // Data
    QVector<SensorData> m_data;
    TestResult m_result;
    
    // Metadata
    QString m_notes;
};

} // namespace HorizonUTM
