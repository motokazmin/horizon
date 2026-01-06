#pragma once

#include <QObject>
#include <QString>
#include "domain/value_objects/SensorData.h"
#include "domain/value_objects/MachineState.h"

namespace HorizonUTM {

/**
 * @brief Interface for Universal Testing Machine (UTM) hardware drivers
 * 
 * This interface abstracts the hardware layer, allowing different UTM implementations
 * (MockUTMDriver, TiniusOlsenDriver, etc.) to be used interchangeably.
 */
class IUTMDriver : public QObject {
    Q_OBJECT

public:
    virtual ~IUTMDriver() = default;
    
    /**
     * @brief Connect to the UTM hardware
     * @param connectionString Port name, IP address, or connection string
     * @return true if connection successful
     */
    virtual bool connect(const QString& connectionString) = 0;
    
    /**
     * @brief Disconnect from the UTM hardware
     */
    virtual void disconnect() = 0;
    
    /**
     * @brief Check if currently connected to hardware
     */
    virtual bool isConnected() const = 0;
    
    /**
     * @brief Get current machine state
     */
    virtual MachineState getState() const = 0;
    
    /**
     * @brief Start a tensile test
     * @param speedMmPerMin Crosshead speed in mm/min
     * @param forceLimitN Maximum force limit in Newtons
     * @return true if test started successfully
     */
    virtual bool startTest(double speedMmPerMin, double forceLimitN) = 0;
    
    /**
     * @brief Stop the current test
     */
    virtual bool stopTest() = 0;
    
    /**
     * @brief Pause the current test
     */
    virtual bool pauseTest() = 0;
    
    /**
     * @brief Resume a paused test
     */
    virtual bool resumeTest() = 0;
    
    /**
     * @brief Set the crosshead speed
     * @param mmPerMin Speed in mm/min
     */
    virtual bool setSpeed(double mmPerMin) = 0;
    
    /**
     * @brief Zero all sensors (force, extension)
     */
    virtual bool zeroSensors() = 0;

signals:
    /**
     * @brief Emitted when connection is established
     */
    void connected();
    
    /**
     * @brief Emitted when disconnected from hardware
     */
    void disconnected();
    
    /**
     * @brief Emitted when machine state changes
     */
    void stateChanged(MachineState state);
    
    /**
     * @brief Emitted when new sensor data is received
     * Typically at 10-100 Hz during a test
     */
    void sensorDataReceived(SensorData data);
    
    /**
     * @brief Emitted when test completes naturally (break or limit reached)
     */
    void testCompleted();
    
    /**
     * @brief Emitted when an error occurs
     */
    void errorOccurred(QString error);
};

} // namespace HorizonUTM
