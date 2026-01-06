#pragma once

#include <QString>
#include <QDateTime>

namespace HorizonUTM {

/**
 * @brief Sample status enumeration
 */
enum class SampleStatus {
    Ready,      ///< Ready for testing
    InProgress, ///< Currently being tested
    Completed,  ///< Test completed
    Failed      ///< Test failed
};

/**
 * @brief Convert SampleStatus to string
 */
inline QString sampleStatusToString(SampleStatus status) {
    switch (status) {
        case SampleStatus::Ready:      return "Ready";
        case SampleStatus::InProgress: return "In Progress";
        case SampleStatus::Completed:  return "Completed";
        case SampleStatus::Failed:     return "Failed";
        default:                       return "Unknown";
    }
}

/**
 * @brief Sample entity - represents a test specimen in the queue
 * 
 * Contains sample identification, geometry, and metadata
 */
class Sample {
public:
    /**
     * @brief Constructor for new sample
     */
    Sample();
    
    /**
     * @brief Constructor with ID
     */
    explicit Sample(int id);
    
    /**
     * @brief Constructor with parameters
     */
    Sample(const QString& name, double width, double thickness, double gaugeLength);
    
    ~Sample() = default;
    
    // Getters
    int getId() const { return m_id; }
    QString getName() const { return m_name; }
    double getWidth() const { return m_width; }
    double getThickness() const { return m_thickness; }
    double getGaugeLength() const { return m_gaugeLength; }
    QString getTestMethod() const { return m_testMethod; }
    QString getOperatorName() const { return m_operatorName; }
    SampleStatus getStatus() const { return m_status; }
    QDateTime getCreatedAt() const { return m_createdAt; }
    QString getNotes() const { return m_notes; }
    
    double getCrossSectionArea() const { return m_width * m_thickness; }
    
    // Setters
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setWidth(double width) { m_width = width; }
    void setThickness(double thickness) { m_thickness = thickness; }
    void setGaugeLength(double length) { m_gaugeLength = length; }
    void setTestMethod(const QString& method) { m_testMethod = method; }
    void setOperatorName(const QString& name) { m_operatorName = name; }
    void setStatus(SampleStatus status) { m_status = status; }
    void setCreatedAt(const QDateTime& time) { m_createdAt = time; }
    void setNotes(const QString& notes) { m_notes = notes; }
    
    /**
     * @brief Validate sample data
     */
    bool isValid() const;
    
    /**
     * @brief Get sample description
     */
    QString getDescription() const;

private:
    int m_id;
    QString m_name;
    
    // Geometry (mm)
    double m_width;
    double m_thickness;
    double m_gaugeLength;
    
    // Test info
    QString m_testMethod;
    QString m_operatorName;
    SampleStatus m_status;
    
    // Metadata
    QDateTime m_createdAt;
    QString m_notes;
};

} // namespace HorizonUTM
