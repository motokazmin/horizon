#pragma once

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "domain/interfaces/IUTMDriver.h"
#include "domain/value_objects/SensorData.h"
#include "domain/value_objects/MachineState.h"

namespace HorizonUTM {

/**
 * @brief Mock UTM driver for testing without real hardware
 * 
 * Simulates realistic stress-strain behavior for tensile testing
 * Generates data points at specified sampling rate
 */
class MockUTMDriver : public IUTMDriver {
    Q_OBJECT

public:
    explicit MockUTMDriver(QObject* parent = nullptr);
    ~MockUTMDriver() override;

    // IUTMDriver interface implementation
    bool connect(const QString& connectionString) override;
    bool disconnect() override;
    bool isConnected() const override;
    
    MachineState getState() const override;
    
    bool startTest(double speedMmPerMin, double forceLimitN) override;
    bool stopTest() override;
    bool pauseTest() override;
    bool resumeTest() override;
    
    bool setSpeed(double speedMmPerMin) override;
    double getSpeed() const override;
    
    bool zero() override;
    
    SensorData getCurrentData() const override;

signals:
    void connected();
    void disconnected();
    void stateChanged(MachineState state);
    void sensorDataReceived(SensorData data);
    void testCompleted();
    void errorOccurred(const QString& error);

private slots:
    /**
     * @brief Generate next data point (called by timer)
     */
    void generateDataPoint();

private:
    /**
     * @brief Simulate realistic stress-strain curve
     * @param strain Current strain (%)
     * @return Stress (MPa)
     */
    double simulateStressCurve(double strain);
    
    /**
     * @brief Calculate force from stress
     * @param stress Stress in MPa
     * @return Force in N
     */
    double stressToForce(double stress);
    
    /**
     * @brief Calculate extension from strain
     * @param strain Strain in %
     * @return Extension in mm
     */
    double strainToExtension(double strain);
    
    /**
     * @brief Reset simulation state
     */
    void resetSimulation();

private:
    // Connection state
    bool m_connected;
    MachineState m_state;
    
    // Test parameters
    double m_speed;           // mm/min
    double m_forceLimit;      // N
    double m_crossSection;    // mm² (for simulation)
    double m_gaugeLength;     // mm (for simulation)
    
    // Simulation state
    QTimer* m_timer;
    qint64 m_testStartTime;   // ms since epoch
    double m_currentTime;     // seconds from test start
    double m_currentExtension; // mm
    double m_currentStrain;   // %
    double m_currentStress;   // MPa
    double m_currentForce;    // N
    
    // Simulation parameters (material properties)
    double m_elasticModulus;  // GPa
    double m_yieldStress;     // MPa
    double m_ultimateStress;  // MPa
    double m_breakStrain;     // %
    
    // Sampling
    int m_samplingRateHz;
    
    // Data point counter
    int m_dataPointCount;
};

} // namespace HorizonUTM
