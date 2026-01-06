#pragma once

#include <QString>
#include <QMap>

namespace HorizonUTM {

/**
 * @brief Test method entity - encapsulates testing standard parameters
 * 
 * Contains configuration for different testing standards like ISO 527-2, ASTM D638
 */
class TestMethod {
public:
    /**
     * @brief Default constructor
     */
    TestMethod();
    
    /**
     * @brief Constructor with method name
     */
    explicit TestMethod(const QString& name);
    
    ~TestMethod() = default;
    
    // Getters
    QString getName() const { return m_name; }
    QString getDescription() const { return m_description; }
    double getDefaultSpeed() const { return m_defaultSpeed; }
    double getDefaultForceLimit() const { return m_defaultForceLimit; }
    double getDefaultGaugeLength() const { return m_defaultGaugeLength; }
    double getYieldOffsetPercent() const { return m_yieldOffsetPercent; }
    
    // Setters
    void setName(const QString& name) { m_name = name; }
    void setDescription(const QString& desc) { m_description = desc; }
    void setDefaultSpeed(double speed) { m_defaultSpeed = speed; }
    void setDefaultForceLimit(double limit) { m_defaultForceLimit = limit; }
    void setDefaultGaugeLength(double length) { m_defaultGaugeLength = length; }
    void setYieldOffsetPercent(double percent) { m_yieldOffsetPercent = percent; }
    
    /**
     * @brief Get available test methods
     */
    static QStringList getAvailableMethods();
    
    /**
     * @brief Create test method by name
     */
    static TestMethod createMethod(const QString& name);
    
    /**
     * @brief Check if method is valid
     */
    bool isValid() const;

private:
    QString m_name;
    QString m_description;
    
    // Default parameters
    double m_defaultSpeed;         // mm/min
    double m_defaultForceLimit;    // N
    double m_defaultGaugeLength;   // mm
    
    // Calculation parameters
    double m_yieldOffsetPercent;   // % for yield stress calculation
    
    /**
     * @brief Initialize ISO 527-2 parameters
     */
    void initializeISO527_2();
    
    /**
     * @brief Initialize ASTM D638 parameters
     */
    void initializeASTM_D638();
};

} // namespace HorizonUTM
