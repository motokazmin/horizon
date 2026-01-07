#include "HardwareController.h"
#include "TestController.h"
#include "core/Logger.h"
#include <QDebug>

namespace HorizonUTM {

HardwareController::HardwareController(IUTMDriver* driver, TestController* testController, QObject* parent)
    : QObject(parent)
    , m_driver(driver)
    , m_testController(testController)
    , m_currentTest(nullptr)
    , m_testInProgress(false)
{
    qDebug() << "=== HardwareController: Connecting signals ===";
    qDebug() << "=== Driver pointer:" << m_driver;
    qDebug() << "=== Driver metaObject:" << m_driver->metaObject()->className();
    qDebug() << "=== This pointer:" << this;

    // Connect driver signals - using old syntax for compatibility
    bool ok1 = connect(m_driver, SIGNAL(connected()), this, SLOT(onDriverConnected()));
    qDebug() << "=== Connected 'connected' signal (old syntax):" << ok1;

    bool ok2 = connect(m_driver, SIGNAL(disconnected()), this, SLOT(onDriverDisconnected()));
    qDebug() << "=== Connected 'disconnected' signal:" << ok2;

    bool ok3 = connect(m_driver, SIGNAL(stateChanged(HorizonUTM::MachineState)),
                      this, SLOT(onStateChanged(HorizonUTM::MachineState)));
    qDebug() << "=== Connected 'stateChanged' signal:" << ok3;

    connect(m_driver, SIGNAL(sensorDataReceived(HorizonUTM::SensorData)),
            this, SLOT(onSensorDataReceived(HorizonUTM::SensorData)));
    connect(m_driver, SIGNAL(testCompleted()), this, SLOT(onTestCompleted()));
    connect(m_driver, SIGNAL(errorOccurred(QString)), this, SLOT(onErrorOccurred(QString)));

    LOG_INFO("HardwareController created");
}

HardwareController::~HardwareController() {
    if (m_testInProgress) {
        stopTest();
    }

    if (m_driver->isConnected()) {
        m_driver->disconnect();
    }

    delete m_currentTest;
}

bool HardwareController::connectToHardware(const QString& connectionString) {
    if (m_driver->isConnected()) {
        LOG_WARNING("Already connected to hardware");
        return true;
    }

    LOG_INFO(QString("Connecting to hardware: %1").arg(connectionString));
    return m_driver->connect(connectionString);
}

bool HardwareController::disconnectFromHardware() {
    if (!m_driver->isConnected()) {
        return true;
    }

    if (m_testInProgress) {
        LOG_WARNING("Stopping test before disconnect");
        stopTest();
    }

    LOG_INFO("Disconnecting from hardware");
    return m_driver->disconnect();
}

bool HardwareController::isConnected() const {
    return m_driver->isConnected();
}

MachineState HardwareController::getMachineState() const {
    return m_driver->getState();
}

bool HardwareController::startTest(Test& test) {
    if (!m_driver->isConnected()) {
        LOG_ERROR("Cannot start test: not connected to hardware");
        emit errorOccurred("Not connected to hardware");
        return false;
    }

    if (m_testInProgress) {
        LOG_ERROR("Cannot start test: another test is in progress");
        emit errorOccurred("Test already in progress");
        return false;
    }

    if (!test.isValid()) {
        LOG_ERROR("Cannot start test: invalid test parameters");
        emit errorOccurred("Invalid test parameters");
        return false;
    }

    // Save test to get ID
    if (!m_testController->saveTest(test)) {
        LOG_ERROR("Failed to save test");
        emit errorOccurred("Failed to save test");
        return false;
    }

    // Store current test
    m_currentTest = new Test(test);
    m_currentTest->setStatus(TestStatus::Running);
    m_currentTest->setStartTime(QDateTime::currentDateTime());
    m_currentTest->clearData(); // Clear any existing data

    // Start hardware test
    if (!m_driver->startTest(test.getSpeed(), test.getForceLimit())) {
        LOG_ERROR("Failed to start hardware test");
        delete m_currentTest;
        m_currentTest = nullptr;
        emit errorOccurred("Failed to start hardware");
        return false;
    }

    m_testInProgress = true;

    LOG_INFO(QString("Test started: ID=%1, Sample=%2")
        .arg(m_currentTest->getId()).arg(m_currentTest->getSampleName()));

    emit testStarted(m_currentTest->getId());

    return true;
}

bool HardwareController::stopTest() {
    if (!m_testInProgress || !m_currentTest) {
        LOG_WARNING("No test in progress");
        return false;
    }

    LOG_INFO(QString("Stopping test ID=%1").arg(m_currentTest->getId()));

    // Stop hardware
    m_driver->stopTest();

    // Update test status
    m_currentTest->setStatus(TestStatus::Stopped);
    m_currentTest->setEndTime(QDateTime::currentDateTime());

    // Calculate results if we have data
    if (m_currentTest->getDataPointCount() > 0) {
        m_testController->completeTest(*m_currentTest);
    }

    int testId = m_currentTest->getId();

    // Cleanup
    delete m_currentTest;
    m_currentTest = nullptr;
    m_testInProgress = false;

    emit testStopped(testId);

    return true;
}

bool HardwareController::pauseTest() {
    if (!m_testInProgress) {
        return false;
    }

    if (m_driver->pauseTest()) {
        m_currentTest->setStatus(TestStatus::Paused);
        LOG_INFO(QString("Test paused: ID=%1").arg(m_currentTest->getId()));
        return true;
    }

    return false;
}

bool HardwareController::resumeTest() {
    if (!m_testInProgress) {
        return false;
    }

    if (m_driver->resumeTest()) {
        m_currentTest->setStatus(TestStatus::Running);
        LOG_INFO(QString("Test resumed: ID=%1").arg(m_currentTest->getId()));
        return true;
    }

    return false;
}

bool HardwareController::zeroSensors() {
    if (!m_driver->isConnected()) {
        LOG_ERROR("Cannot zero sensors: not connected");
        return false;
    }

    if (m_testInProgress) {
        LOG_WARNING("Cannot zero sensors during test");
        return false;
    }

    return m_driver->zero();
}

bool HardwareController::isTestRunning() const {
    return m_testInProgress;
}

// Private slots

void HardwareController::onDriverConnected() {
    qDebug() << "=== HardwareController::onDriverConnected SLOT CALLED ===";
    LOG_INFO("Driver connected");
    qDebug() << "=== HardwareController: BEFORE emit hardwareConnected() ===";
    emit hardwareConnected();
    qDebug() << "=== HardwareController: AFTER emit hardwareConnected() ===";
}

void HardwareController::onDriverDisconnected() {
    LOG_INFO("Driver disconnected");

    // If test was in progress, stop it
    if (m_testInProgress) {
        stopTest();
    }

    emit hardwareDisconnected();
}

void HardwareController::onStateChanged(MachineState state) {
    LOG_DEBUG(QString("Machine state changed: %1").arg(static_cast<int>(state)));
    emit machineStateChanged(state);
}

void HardwareController::onSensorDataReceived(SensorData data) {
    if (!m_testInProgress || !m_currentTest) {
        return;
    }

    // Process data through test controller
    m_testController->processSensorData(*m_currentTest, data);

    // Forward to UI
    emit sensorDataReceived(data);
}

void HardwareController::onTestCompleted() {
    if (!m_testInProgress || !m_currentTest) {
        return;
    }

    LOG_INFO(QString("Test completed: ID=%1").arg(m_currentTest->getId()));

    // Update test status
    m_currentTest->setStatus(TestStatus::Completed);
    m_currentTest->setEndTime(QDateTime::currentDateTime());

    // Calculate final results
    m_testController->completeTest(*m_currentTest);

    int testId = m_currentTest->getId();

    // Cleanup
    delete m_currentTest;
    m_currentTest = nullptr;
    m_testInProgress = false;

    emit testCompleted(testId);
}

void HardwareController::onErrorOccurred(const QString& error) {
    LOG_ERROR(QString("Hardware error: %1").arg(error));

    // If test in progress, mark as failed
    if (m_testInProgress && m_currentTest) {
        m_currentTest->setStatus(TestStatus::Failed);
        m_testController->updateTest(*m_currentTest);

        delete m_currentTest;
        m_currentTest = nullptr;
        m_testInProgress = false;
    }

    emit errorOccurred(error);
}

} // namespace HorizonUTM
