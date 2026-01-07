#include "MockUTMDriver.h"
#include "core/Logger.h"
#include <QtMath>
#include <QRandomGenerator>
#include <QThread>
#include <QDebug>
#include <QTimer>

namespace HorizonUTM {

MockUTMDriver::MockUTMDriver(QObject* parent)
    : IUTMDriver(parent)
    , m_connected(false)
    , m_state(MachineState::Disconnected)
    , m_speed(5.0)
    , m_forceLimit(10000.0)
    , m_crossSection(40.0)      // 10mm x 4mm = 40 mm²
    , m_gaugeLength(50.0)       // 50 mm
    , m_timer(new QTimer(this))
    , m_testStartTime(0)
    , m_currentTime(0.0)
    , m_currentExtension(0.0)
    , m_currentStrain(0.0)
    , m_currentStress(0.0)
    , m_currentForce(0.0)
    , m_elasticModulus(3.0)     // 3 GPa (typical for plastics)
    , m_yieldStress(50.0)       // 50 MPa
    , m_ultimateStress(60.0)    // 60 MPa
    , m_breakStrain(8.0)        // 8% elongation at break
    , m_samplingRateHz(100)     // 100 Hz
    , m_dataPointCount(0)
{
    QObject::connect(m_timer, &QTimer::timeout, this, &MockUTMDriver::generateDataPoint);

    LOG_INFO("MockUTMDriver created");
}

MockUTMDriver::~MockUTMDriver() {
    if (m_connected) {
        disconnect();
    }
    LOG_INFO("MockUTMDriver destroyed");
}

bool MockUTMDriver::connect(const QString& connectionString) {
    if (m_connected) {
        LOG_WARNING("MockUTMDriver already connected");
        return true;
    }

    LOG_INFO(QString("MockUTMDriver connecting: %1").arg(connectionString));

    m_connected = true;
    m_state = MachineState::Idle;

    qDebug() << "=== MockUTMDriver: About to emit signals ===";
    qDebug() << "=== this pointer:" << this;

    emit connected();
    emit stateChanged(m_state);

    qDebug() << "=== MockUTMDriver: Signals emitted ===";

    LOG_INFO("MockUTMDriver connected successfully");
    return true;
}

bool MockUTMDriver::disconnect() {
    if (!m_connected) {
        return true;
    }

    // Stop test if running
    if (m_state == MachineState::Running) {
        stopTest();
    }

    m_connected = false;
    m_state = MachineState::Disconnected;

    emit disconnected();
    emit stateChanged(m_state);

    LOG_INFO("MockUTMDriver disconnected");
    return true;
}

bool MockUTMDriver::isConnected() const {
    return m_connected;
}

MachineState MockUTMDriver::getState() const {
    return m_state;
}

bool MockUTMDriver::startTest(double speedMmPerMin, double forceLimitN) {
    if (!m_connected) {
        LOG_ERROR("Cannot start test: not connected");
        emit errorOccurred("Not connected");
        return false;
    }

    if (m_state == MachineState::Running) {
        LOG_WARNING("Test already running");
        return false;
    }

    m_speed = speedMmPerMin;
    m_forceLimit = forceLimitN;

    resetSimulation();

    m_state = MachineState::Running;
    m_testStartTime = QDateTime::currentMSecsSinceEpoch();

    // Start data generation at 100 Hz
    m_timer->start(static_cast<int>(1000 / m_samplingRateHz));

    emit stateChanged(m_state);

    LOG_INFO(QString("Test started: speed=%1 mm/min, limit=%2 N")
        .arg(m_speed).arg(m_forceLimit));

    return true;
}

bool MockUTMDriver::stopTest() {
    if (m_state != MachineState::Running && m_state != MachineState::Paused) {
        return false;
    }

    m_timer->stop();
    m_state = MachineState::Stopping;
    emit stateChanged(m_state);

    // Simulate stop delay
    QThread::msleep(50);

    m_state = MachineState::Idle;
    emit stateChanged(m_state);
    emit testCompleted();

    LOG_INFO(QString("Test stopped at %1% strain, %2 data points")
        .arg(m_currentStrain, 0, 'f', 2).arg(m_dataPointCount));

    return true;
}

bool MockUTMDriver::pauseTest() {
    if (m_state != MachineState::Running) {
        return false;
    }

    m_timer->stop();
    m_state = MachineState::Paused;
    emit stateChanged(m_state);

    LOG_INFO("Test paused");
    return true;
}

bool MockUTMDriver::resumeTest() {
    if (m_state != MachineState::Paused) {
        return false;
    }

    m_timer->start(1000 / m_samplingRateHz);
    m_state = MachineState::Running;
    emit stateChanged(m_state);

    LOG_INFO("Test resumed");
    return true;
}

bool MockUTMDriver::setSpeed(double speedMmPerMin) {
    if (speedMmPerMin <= 0 || speedMmPerMin > 500) {
        LOG_ERROR(QString("Invalid speed: %1 mm/min").arg(speedMmPerMin, 0, 'f', 2));
        return false;
    }

    m_speed = speedMmPerMin;
    LOG_DEBUG(QString("Speed set to %1 mm/min").arg(m_speed, 0, 'f', 2));
    return true;
}

double MockUTMDriver::getSpeed() const {
    return m_speed;
}

bool MockUTMDriver::zero() {
    if (m_state == MachineState::Running) {
        LOG_WARNING("Cannot zero while test is running");
        return false;
    }

    resetSimulation();
    LOG_INFO("Sensors zeroed");
    return true;
}

SensorData MockUTMDriver::getCurrentData() const {
    SensorData data;
    data.timestamp = QDateTime::currentMSecsSinceEpoch();
    data.force = m_currentForce;
    data.extension = m_currentExtension;
    data.stress = m_currentStress;
    data.strain = m_currentStrain;
    data.temperature = 23.0; // Constant room temperature
    return data;
}

void MockUTMDriver::generateDataPoint() {
    // Calculate elapsed time
    qint64 currentMs = QDateTime::currentMSecsSinceEpoch();
    m_currentTime = (currentMs - m_testStartTime) / 1000.0; // seconds

    // Calculate extension based on speed
    m_currentExtension = (m_speed / 60.0) * m_currentTime; // mm

    // Calculate strain
    m_currentStrain = (m_currentExtension / m_gaugeLength) * 100.0; // %

    // Check if test should complete (material break)
    if (m_currentStrain >= m_breakStrain) {
        stopTest();
        return;
    }

    // Simulate stress-strain curve
    m_currentStress = simulateStressCurve(m_currentStrain);

    // Calculate force
    m_currentForce = stressToForce(m_currentStress);

    // Check force limit
    if (m_currentForce >= m_forceLimit) {
        LOG_WARNING(QString("Force limit reached: %.0f N").arg(m_currentForce));
        stopTest();
        return;
    }

    // Create and emit sensor data
    SensorData data = getCurrentData();
    emit sensorDataReceived(data);

    m_dataPointCount++;

    // Log every 100 points
    if (m_dataPointCount % 100 == 0) {
        LOG_DEBUG(QString("Data point %1: Strain=%2%, Stress=%3 MPa, Force=%4 N")
            .arg(m_dataPointCount).arg(m_currentStrain, 0, 'f', 3).arg(m_currentStress, 0, 'f', 2).arg(m_currentForce, 0, 'f', 0));
    }
}

double MockUTMDriver::simulateStressCurve(double strain) {
    // Realistic stress-strain curve for plastic material
    // Phases: elastic → yield → plastic deformation → necking → break

    if (strain <= 0) return 0.0;

    // Add small random noise (±0.5%)
    double noise = (QRandomGenerator::global()->bounded(200) - 100) / 10000.0;

    // Phase 1: Linear elastic region (0 to ~0.5% strain)
    double elasticLimit = 0.5; // % strain
    if (strain <= elasticLimit) {
        // Stress = E * strain
        double stress = (m_elasticModulus * 1000.0) * (strain / 100.0); // Convert GPa to MPa
        return stress * (1.0 + noise);
    }

    // Phase 2: Yield point (~0.5% to 1.5% strain)
    double yieldEnd = 1.5;
    if (strain <= yieldEnd) {
        // Transition from elastic to plastic
        double t = (strain - elasticLimit) / (yieldEnd - elasticLimit);
        double stress = m_elasticModulus * 1000.0 * (elasticLimit / 100.0) +
                       (m_yieldStress - m_elasticModulus * 1000.0 * (elasticLimit / 100.0)) * t;
        return stress * (1.0 + noise);
    }

    // Phase 3: Plastic deformation (1.5% to 5% strain)
    double plasticEnd = 5.0;
    if (strain <= plasticEnd) {
        // Gradual increase to ultimate stress
        double t = (strain - yieldEnd) / (plasticEnd - yieldEnd);
        double stress = m_yieldStress + (m_ultimateStress - m_yieldStress) * t;
        return stress * (1.0 + noise);
    }

    // Phase 4: Necking and approaching break (5% to break)
    double t = (strain - plasticEnd) / (m_breakStrain - plasticEnd);
    // Stress decreases slightly after ultimate
    double stress = m_ultimateStress * (1.0 - 0.2 * t); // 20% decrease before break
    return stress * (1.0 + noise);
}

double MockUTMDriver::stressToForce(double stress) {
    // Force = Stress * Area
    return stress * m_crossSection;
}

double MockUTMDriver::strainToExtension(double strain) {
    // Extension = Strain * GaugeLength / 100
    return (strain / 100.0) * m_gaugeLength;
}

void MockUTMDriver::resetSimulation() {
    m_currentTime = 0.0;
    m_currentExtension = 0.0;
    m_currentStrain = 0.0;
    m_currentStress = 0.0;
    m_currentForce = 0.0;
    m_dataPointCount = 0;
    m_testStartTime = 0;
}

} // namespace HorizonUTM
// Phase 3 implementation
