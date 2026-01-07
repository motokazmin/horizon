#pragma once

#include <QObject>
#include <QTimer>
#include "domain/interfaces/IUTMDriver.h"
#include "domain/entities/Test.h"
#include "domain/value_objects/MachineState.h"
#include "domain/value_objects/SensorData.h"

namespace HorizonUTM {

class TestController;

/**
 * @brief Controller for hardware operations
 * 
 * Manages UTM driver and coordinates test execution
 */
class HardwareController : public QObject {
    Q_OBJECT

public:
    explicit HardwareController(IUTMDriver* driver, TestController* testController, QObject* parent = nullptr);
    ~HardwareController() override;
    
    /**
     * @brief Connect to hardware
     * @param connectionString Connection parameters
     * @return true if connected
     */
    bool connectToHardware(const QString& connectionString = "mock");
    
    /**
     * @brief Disconnect from hardware
     */
    bool disconnectFromHardware();
    
    /**
     * @brief Check if connected
     */
    bool isConnected() const;
    
    /**
     * @brief Get current machine state
     */
    MachineState getMachineState() const;
    
    /**
     * @brief Start test execution
     * @param test Test parameters
     * @return true if started
     */
    bool startTest(Test& test);
    
    /**
     * @brief Stop current test
     */
    bool stopTest();
    
    /**
     * @brief Pause current test
     */
    bool pauseTest();
    
    /**
     * @brief Resume paused test
     */
    bool resumeTest();
    
    /**
     * @brief Zero all sensors
     */
    bool zeroSensors();
    
    /**
     * @brief Get current test
     */
    Test* getCurrentTest() const { return m_currentTest; }
    
    /**
     * @brief Check if test is running
     */
    bool isTestRunning() const;

signals:
    /**
     * @brief Emitted when hardware connects
     */
    void hardwareConnected();
    
    /**
     * @brief Emitted when hardware disconnects
     */
    void hardwareDisconnected();
    
    /**
     * @brief Emitted when machine state changes
     */
    void machineStateChanged(MachineState state);
    
    /**
     * @brief Emitted when new sensor data arrives
     */
    void sensorDataReceived(SensorData data);
    
    /**
     * @brief Emitted when test starts
     */
    void testStarted(int testId);
    
    /**
     * @brief Emitted when test completes
     */
    void testCompleted(int testId);
    
    /**
     * @brief Emitted when test is stopped
     */
    void testStopped(int testId);
    
    /**
     * @brief Emitted on error
     */
    void errorOccurred(const QString& error);

private slots:
    /**
     * @brief Handle driver connection
     */
    void onDriverConnected();
    
    /**
     * @brief Handle driver disconnection
     */
    void onDriverDisconnected();
    
    /**
     * @brief Handle state changes
     */
    void onStateChanged(MachineState state);
    
    /**
     * @brief Handle sensor data
     */
    void onSensorDataReceived(SensorData data);
    
    /**
     * @brief Handle test completion
     */
    void onTestCompleted();
    
    /**
     * @brief Handle errors
     */
    void onErrorOccurred(const QString& error);

private:
    IUTMDriver* m_driver;
    TestController* m_testController;
    Test* m_currentTest;
    bool m_testInProgress;
};

} // namespace HorizonUTM
