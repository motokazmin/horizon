#include "TestMethod.h"

namespace HorizonUTM {

TestMethod::TestMethod()
    : m_name("ISO 527-2")
    , m_description("Plastics - Determination of tensile properties - Part 2: Test conditions for moulding and extrusion plastics")
    , m_defaultSpeed(5.0)
    , m_defaultForceLimit(10000.0)
    , m_defaultGaugeLength(50.0)
    , m_yieldOffsetPercent(0.2)
{
    initializeISO527_2();
}

TestMethod::TestMethod(const QString& name)
    : m_name(name)
    , m_defaultSpeed(5.0)
    , m_defaultForceLimit(10000.0)
    , m_defaultGaugeLength(50.0)
    , m_yieldOffsetPercent(0.2)
{
    if (name == "ISO 527-2") {
        initializeISO527_2();
    } else if (name == "ASTM D638") {
        initializeASTM_D638();
    } else {
        // Default to ISO 527-2
        initializeISO527_2();
    }
}

QStringList TestMethod::getAvailableMethods() {
    return QStringList() << "ISO 527-2" << "ASTM D638";
}

TestMethod TestMethod::createMethod(const QString& name) {
    return TestMethod(name);
}

bool TestMethod::isValid() const {
    if (m_name.isEmpty()) return false;
    if (m_defaultSpeed <= 0) return false;
    if (m_defaultForceLimit <= 0) return false;
    if (m_defaultGaugeLength <= 0) return false;
    
    return true;
}

void TestMethod::initializeISO527_2() {
    m_name = "ISO 527-2";
    m_description = "Plastics - Determination of tensile properties - Part 2";
    m_defaultSpeed = 5.0;           // 5 mm/min for ISO 527-2
    m_defaultForceLimit = 10000.0;  // 10 kN
    m_defaultGaugeLength = 50.0;    // 50 mm
    m_yieldOffsetPercent = 0.2;     // 0.2% offset for yield
}

void TestMethod::initializeASTM_D638() {
    m_name = "ASTM D638";
    m_description = "Standard Test Method for Tensile Properties of Plastics";
    m_defaultSpeed = 5.0;           // 5 mm/min (typical for Type I specimens)
    m_defaultForceLimit = 10000.0;  // 10 kN
    m_defaultGaugeLength = 50.0;    // 50 mm (Type I specimen)
    m_yieldOffsetPercent = 0.2;     // 0.2% offset
}

} // namespace HorizonUTM
