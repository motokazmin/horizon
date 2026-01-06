#include "Sample.h"

namespace HorizonUTM {

Sample::Sample()
    : m_id(-1)
    , m_name("")
    , m_width(10.0)
    , m_thickness(4.0)
    , m_gaugeLength(50.0)
    , m_testMethod("ISO 527-2")
    , m_operatorName("")
    , m_status(SampleStatus::Ready)
    , m_createdAt(QDateTime::currentDateTime())
    , m_notes("")
{
}

Sample::Sample(int id)
    : Sample()
{
    m_id = id;
}

Sample::Sample(const QString& name, double width, double thickness, double gaugeLength)
    : m_id(-1)
    , m_name(name)
    , m_width(width)
    , m_thickness(thickness)
    , m_gaugeLength(gaugeLength)
    , m_testMethod("ISO 527-2")
    , m_operatorName("")
    , m_status(SampleStatus::Ready)
    , m_createdAt(QDateTime::currentDateTime())
    , m_notes("")
{
}

bool Sample::isValid() const {
    if (m_name.isEmpty()) return false;
    if (m_width <= 0 || m_thickness <= 0 || m_gaugeLength <= 0) return false;
    if (m_testMethod.isEmpty()) return false;
    
    return true;
}

QString Sample::getDescription() const {
    return QString("%1 (%2x%3x%4 mm)")
        .arg(m_name)
        .arg(m_width, 0, 'f', 1)
        .arg(m_thickness, 0, 'f', 1)
        .arg(m_gaugeLength, 0, 'f', 1);
}

} // namespace HorizonUTM
