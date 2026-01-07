#include "Test.h"

namespace HorizonUTM {

Test::Test()
    : m_id(-1)
    , m_sampleName("")
    , m_operatorName("")
    , m_testMethod("ISO 527-2")
    , m_startTime(QDateTime())
    , m_endTime(QDateTime())
    , m_status(TestStatus::Ready)
    , m_width(10.0)
    , m_thickness(4.0)
    , m_gaugeLength(50.0)
    , m_speed(5.0)
    , m_forceLimit(10000.0)
    , m_temperature(23.0)
    , m_notes("")
{
}

Test::Test(int id)
    : Test()
{
    m_id = id;
}

void Test::addDataPoint(const SensorData& data) {
    m_data.append(data);
}

void Test::clearData() {
    m_data.clear();
}

bool Test::isValid() const {
    // Check required fields
    if (m_sampleName.isEmpty()) return false;
    if (m_operatorName.isEmpty()) return false;
    if (m_testMethod.isEmpty()) return false;
    
    // Check dimensions
    if (m_width <= 0 || m_thickness <= 0 || m_gaugeLength <= 0) return false;
    
    // Check test parameters
    if (m_speed <= 0 || m_forceLimit <= 0) return false;
    
    return true;
}

qint64 Test::getDuration() const {
    if (!m_startTime.isValid()) return 0;
    if (!m_endTime.isValid()) return 0;
    
    return m_startTime.secsTo(m_endTime);
}

QVector<QPair<double, double>> Test::getStressStrainData() const {
    QVector<QPair<double, double>> result;
    result.reserve(m_data.size());
    
    for (const SensorData& data : m_data) {
        result.append(qMakePair(data.strain, data.stress));
    }
    
    return result;
}

} // namespace HorizonUTM
