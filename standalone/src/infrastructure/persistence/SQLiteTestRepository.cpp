#include "SQLiteTestRepository.h"
#include "DatabaseManager.h"
#include "core/Logger.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace HorizonUTM {

SQLiteTestRepository::SQLiteTestRepository() {
    LOG_INFO("SQLiteTestRepository created");
}

QSqlDatabase SQLiteTestRepository::getDatabase() const {
    return DatabaseManager::instance().database();
}

// ==================== TEST OPERATIONS ====================

bool SQLiteTestRepository::saveTest(const Test& test) {
    int testId = test.getId();
    
    // If test has no ID, insert new
    if (testId <= 0) {
        if (!insertTest(test, testId)) {
            return false;
        }
        const_cast<Test&>(test).setId(testId);
    } else {
        // Update existing
        if (!updateTestInDb(test)) {
            return false;
        }
    }
    
    // Save data points
    if (!test.getData().isEmpty()) {
        if (!saveDataPoints(testId, test.getData())) {
            LOG_ERROR("Failed to save data points");
            return false;
        }
    }
    
    LOG_INFO(QString("Test saved: ID=%1, Sample=%2")
        .arg(testId).arg(test.getSampleName()));
    
    return true;
}

bool SQLiteTestRepository::insertTest(const Test& test, int& outId) {
    QSqlQuery query(getDatabase());
    
    query.prepare(R"(
        INSERT INTO tests (
            sample_name, operator_name, test_method,
            width, thickness, gauge_length,
            speed, force_limit, temperature,
            status, start_time, end_time,
            max_stress, yield_stress, ultimate_stress, break_stress,
            elastic_modulus, elongation_at_break,
            notes
        ) VALUES (
            :sample_name, :operator_name, :test_method,
            :width, :thickness, :gauge_length,
            :speed, :force_limit, :temperature,
            :status, :start_time, :end_time,
            :max_stress, :yield_stress, :ultimate_stress, :break_stress,
            :elastic_modulus, :elongation_at_break,
            :notes
        )
    )");
    
    query.bindValue(":sample_name", test.getSampleName());
    query.bindValue(":operator_name", test.getOperatorName());
    query.bindValue(":test_method", test.getTestMethod());
    query.bindValue(":width", test.getWidth());
    query.bindValue(":thickness", test.getThickness());
    query.bindValue(":gauge_length", test.getGaugeLength());
    query.bindValue(":speed", test.getSpeed());
    query.bindValue(":force_limit", test.getForceLimit());
    query.bindValue(":temperature", test.getTemperature());
    query.bindValue(":status", testStatusToString(test.getStatus()));
    query.bindValue(":start_time", test.getStartTime());
    query.bindValue(":end_time", test.getEndTime());
    
    const TestResult& result = test.getResult();
    query.bindValue(":max_stress", result.maxStress);
    query.bindValue(":yield_stress", result.yieldStress);
    query.bindValue(":ultimate_stress", result.ultimateStress);
    query.bindValue(":break_stress", result.breakStress);
    query.bindValue(":elastic_modulus", result.elasticModulus);
    query.bindValue(":elongation_at_break", result.elongationAtBreak);
    query.bindValue(":notes", test.getNotes());
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to insert test: %1").arg(query.lastError().text()));
        return false;
    }
    
    outId = query.lastInsertId().toInt();
    return true;
}

bool SQLiteTestRepository::updateTest(const Test& test) {
    return saveTest(test);
}

bool SQLiteTestRepository::updateTestInDb(const Test& test) {
    QSqlQuery query(getDatabase());
    
    query.prepare(R"(
        UPDATE tests SET
            sample_name = :sample_name,
            operator_name = :operator_name,
            test_method = :test_method,
            width = :width,
            thickness = :thickness,
            gauge_length = :gauge_length,
            speed = :speed,
            force_limit = :force_limit,
            temperature = :temperature,
            status = :status,
            start_time = :start_time,
            end_time = :end_time,
            max_stress = :max_stress,
            yield_stress = :yield_stress,
            ultimate_stress = :ultimate_stress,
            break_stress = :break_stress,
            elastic_modulus = :elastic_modulus,
            elongation_at_break = :elongation_at_break,
            notes = :notes
        WHERE id = :id
    )");
    
    query.bindValue(":id", test.getId());
    query.bindValue(":sample_name", test.getSampleName());
    query.bindValue(":operator_name", test.getOperatorName());
    query.bindValue(":test_method", test.getTestMethod());
    query.bindValue(":width", test.getWidth());
    query.bindValue(":thickness", test.getThickness());
    query.bindValue(":gauge_length", test.getGaugeLength());
    query.bindValue(":speed", test.getSpeed());
    query.bindValue(":force_limit", test.getForceLimit());
    query.bindValue(":temperature", test.getTemperature());
    query.bindValue(":status", testStatusToString(test.getStatus()));
    query.bindValue(":start_time", test.getStartTime());
    query.bindValue(":end_time", test.getEndTime());
    
    const TestResult& result = test.getResult();
    query.bindValue(":max_stress", result.maxStress);
    query.bindValue(":yield_stress", result.yieldStress);
    query.bindValue(":ultimate_stress", result.ultimateStress);
    query.bindValue(":break_stress", result.breakStress);
    query.bindValue(":elastic_modulus", result.elasticModulus);
    query.bindValue(":elongation_at_break", result.elongationAtBreak);
    query.bindValue(":notes", test.getNotes());
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to update test: %1").arg(query.lastError().text()));
        return false;
    }
    
    return true;
}

bool SQLiteTestRepository::deleteTest(int testId) {
    QSqlQuery query(getDatabase());
    query.prepare("DELETE FROM tests WHERE id = :id");
    query.bindValue(":id", testId);
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to delete test: %1").arg(query.lastError().text()));
        return false;
    }
    
    LOG_INFO(QString("Test deleted: ID=%1").arg(testId));
    return true;
}

Test SQLiteTestRepository::getTest(int testId) {
    QSqlQuery query(getDatabase());
    query.prepare("SELECT * FROM tests WHERE id = :id");
    query.bindValue(":id", testId);
    
    if (!query.exec() || !query.next()) {
        LOG_ERROR(QString("Failed to get test ID=%1").arg(testId));
        return Test();
    }
    
    Test test = testFromQuery(query);
    
    // Load data points
    QVector<SensorData> data = getDataPoints(testId);
    test.setData(data);
    
    return test;
}

QVector<Test> SQLiteTestRepository::getAllTests() {
    QVector<Test> tests;
    
    QSqlQuery query(getDatabase());
    if (!query.exec("SELECT * FROM tests ORDER BY created_at DESC")) {
        LOG_ERROR(QString("Failed to get all tests: %1").arg(query.lastError().text()));
        return tests;
    }
    
    while (query.next()) {
        tests.append(testFromQuery(query));
    }
    
    LOG_DEBUG(QString("Retrieved %1 tests").arg(tests.size()));
    return tests;
}

QVector<Test> SQLiteTestRepository::getTestsByStatus(TestStatus status) {
    QVector<Test> tests;
    
    QSqlQuery query(getDatabase());
    query.prepare("SELECT * FROM tests WHERE status = :status ORDER BY created_at DESC");
    query.bindValue(":status", testStatusToString(status));
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to get tests by status: %1").arg(query.lastError().text()));
        return tests;
    }
    
    while (query.next()) {
        tests.append(testFromQuery(query));
    }
    
    return tests;
}

QVector<Test> SQLiteTestRepository::getTestsByDateRange(const QDateTime& start, const QDateTime& end) {
    QVector<Test> tests;
    
    QSqlQuery query(getDatabase());
    query.prepare("SELECT * FROM tests WHERE start_time BETWEEN :start AND :end ORDER BY start_time DESC");
    query.bindValue(":start", start);
    query.bindValue(":end", end);
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to get tests by date range: %1").arg(query.lastError().text()));
        return tests;
    }
    
    while (query.next()) {
        tests.append(testFromQuery(query));
    }
    
    return tests;
}

Test SQLiteTestRepository::testFromQuery(const QSqlQuery& query) {
    Test test(query.value("id").toInt());
    
    test.setSampleName(query.value("sample_name").toString());
    test.setOperatorName(query.value("operator_name").toString());
    test.setTestMethod(query.value("test_method").toString());
    test.setWidth(query.value("width").toDouble());
    test.setThickness(query.value("thickness").toDouble());
    test.setGaugeLength(query.value("gauge_length").toDouble());
    test.setSpeed(query.value("speed").toDouble());
    test.setForceLimit(query.value("force_limit").toDouble());
    test.setTemperature(query.value("temperature").toDouble());
    
    QString statusStr = query.value("status").toString();
    if (statusStr == "Ready") test.setStatus(TestStatus::Ready);
    else if (statusStr == "Running") test.setStatus(TestStatus::Running);
    else if (statusStr == "Paused") test.setStatus(TestStatus::Paused);
    else if (statusStr == "Completed") test.setStatus(TestStatus::Completed);
    else if (statusStr == "Failed") test.setStatus(TestStatus::Failed);
    else if (statusStr == "Stopped") test.setStatus(TestStatus::Stopped);
    
    test.setStartTime(query.value("start_time").toDateTime());
    test.setEndTime(query.value("end_time").toDateTime());
    
    TestResult result;
    result.maxStress = query.value("max_stress").toDouble();
    result.yieldStress = query.value("yield_stress").toDouble();
    result.ultimateStress = query.value("ultimate_stress").toDouble();
    result.breakStress = query.value("break_stress").toDouble();
    result.elasticModulus = query.value("elastic_modulus").toDouble();
    result.elongationAtBreak = query.value("elongation_at_break").toDouble();
    test.setResult(result);
    
    test.setNotes(query.value("notes").toString());
    
    return test;
}

// ==================== DATA POINTS OPERATIONS ====================

bool SQLiteTestRepository::saveDataPoints(int testId, const QVector<SensorData>& data) {
    if (data.isEmpty()) {
        return true;
    }
    
    QSqlDatabase db = getDatabase();
    db.transaction();
    
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO test_data_points (
            test_id, timestamp, time_seconds,
            force, extension, stress, strain, temperature
        ) VALUES (
            :test_id, :timestamp, :time_seconds,
            :force, :extension, :stress, :strain, :temperature
        )
    )");
    
    qint64 startTime = data.first().timestamp;
    
    for (const auto& point : data) {
        query.bindValue(":test_id", testId);
        query.bindValue(":timestamp", point.timestamp);
        query.bindValue(":time_seconds", (point.timestamp - startTime) / 1000.0);
        query.bindValue(":force", point.force);
        query.bindValue(":extension", point.extension);
        query.bindValue(":stress", point.stress);
        query.bindValue(":strain", point.strain);
        query.bindValue(":temperature", point.temperature);
        
        if (!query.exec()) {
            LOG_ERROR(QString("Failed to save data point: %1").arg(query.lastError().text()));
            db.rollback();
            return false;
        }
    }
    
    db.commit();
    LOG_DEBUG(QString("Saved %1 data points for test ID=%2").arg(data.size()).arg(testId));
    
    return true;
}

QVector<SensorData> SQLiteTestRepository::getDataPoints(int testId) {
    QVector<SensorData> data;
    
    QSqlQuery query(getDatabase());
    query.prepare("SELECT * FROM test_data_points WHERE test_id = :test_id ORDER BY timestamp");
    query.bindValue(":test_id", testId);
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to get data points: %1").arg(query.lastError().text()));
        return data;
    }
    
    while (query.next()) {
        SensorData point;
        point.timestamp = query.value("timestamp").toLongLong();
        point.force = query.value("force").toDouble();
        point.extension = query.value("extension").toDouble();
        point.stress = query.value("stress").toDouble();
        point.strain = query.value("strain").toDouble();
        point.temperature = query.value("temperature").toDouble();
        
        data.append(point);
    }
    
    return data;
}

bool SQLiteTestRepository::deleteDataPoints(int testId) {
    QSqlQuery query(getDatabase());
    query.prepare("DELETE FROM test_data_points WHERE test_id = :test_id");
    query.bindValue(":test_id", testId);
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to delete data points: %1").arg(query.lastError().text()));
        return false;
    }
    
    return true;
}

// ==================== SAMPLE OPERATIONS ====================

bool SQLiteTestRepository::saveSample(const Sample& sample) {
    int sampleId = sample.getId();
    
    if (sampleId <= 0) {
        return insertSample(sample, sampleId);
    } else {
        return updateSample(sample);
    }
}

bool SQLiteTestRepository::insertSample(const Sample& sample, int& outId) {
    QSqlQuery query(getDatabase());
    
    query.prepare(R"(
        INSERT INTO samples (
            name, width, thickness, gauge_length,
            test_method, operator_name, status, notes
        ) VALUES (
            :name, :width, :thickness, :gauge_length,
            :test_method, :operator_name, :status, :notes
        )
    )");
    
    query.bindValue(":name", sample.getName());
    query.bindValue(":width", sample.getWidth());
    query.bindValue(":thickness", sample.getThickness());
    query.bindValue(":gauge_length", sample.getGaugeLength());
    query.bindValue(":test_method", sample.getTestMethod());
    query.bindValue(":operator_name", sample.getOperatorName());
    query.bindValue(":status", sampleStatusToString(sample.getStatus()));
    query.bindValue(":notes", sample.getNotes());
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to insert sample: %1").arg(query.lastError().text()));
        return false;
    }
    
    outId = query.lastInsertId().toInt();
    const_cast<Sample&>(sample).setId(outId);
    
    LOG_INFO(QString("Sample saved: ID=%1, Name=%2").arg(outId).arg(sample.getName()));
    return true;
}

bool SQLiteTestRepository::updateSample(const Sample& sample) {
    QSqlQuery query(getDatabase());
    
    query.prepare(R"(
        UPDATE samples SET
            name = :name,
            width = :width,
            thickness = :thickness,
            gauge_length = :gauge_length,
            test_method = :test_method,
            operator_name = :operator_name,
            status = :status,
            notes = :notes
        WHERE id = :id
    )");
    
    query.bindValue(":id", sample.getId());
    query.bindValue(":name", sample.getName());
    query.bindValue(":width", sample.getWidth());
    query.bindValue(":thickness", sample.getThickness());
    query.bindValue(":gauge_length", sample.getGaugeLength());
    query.bindValue(":test_method", sample.getTestMethod());
    query.bindValue(":operator_name", sample.getOperatorName());
    query.bindValue(":status", sampleStatusToString(sample.getStatus()));
    query.bindValue(":notes", sample.getNotes());
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to update sample: %1").arg(query.lastError().text()));
        return false;
    }
    
    return true;
}

bool SQLiteTestRepository::deleteSample(int sampleId) {
    QSqlQuery query(getDatabase());
    query.prepare("DELETE FROM samples WHERE id = :id");
    query.bindValue(":id", sampleId);
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to delete sample: %1").arg(query.lastError().text()));
        return false;
    }
    
    LOG_INFO(QString("Sample deleted: ID=%1").arg(sampleId));
    return true;
}

Sample SQLiteTestRepository::getSample(int sampleId) {
    QSqlQuery query(getDatabase());
    query.prepare("SELECT * FROM samples WHERE id = :id");
    query.bindValue(":id", sampleId);
    
    if (!query.exec() || !query.next()) {
        LOG_ERROR(QString("Failed to get sample ID=%1").arg(sampleId));
        return Sample();
    }
    
    return sampleFromQuery(query);
}

QVector<Sample> SQLiteTestRepository::getAllSamples() {
    QVector<Sample> samples;
    
    QSqlQuery query(getDatabase());
    if (!query.exec("SELECT * FROM samples ORDER BY created_at DESC")) {
        LOG_ERROR(QString("Failed to get all samples: %1").arg(query.lastError().text()));
        return samples;
    }
    
    while (query.next()) {
        samples.append(sampleFromQuery(query));
    }
    
    return samples;
}

QVector<Sample> SQLiteTestRepository::getSamplesByStatus(SampleStatus status) {
    QVector<Sample> samples;
    
    QSqlQuery query(getDatabase());
    query.prepare("SELECT * FROM samples WHERE status = :status ORDER BY created_at DESC");
    query.bindValue(":status", sampleStatusToString(status));
    
    if (!query.exec()) {
        LOG_ERROR(QString("Failed to get samples by status: %1").arg(query.lastError().text()));
        return samples;
    }
    
    while (query.next()) {
        samples.append(sampleFromQuery(query));
    }
    
    return samples;
}

Sample SQLiteTestRepository::sampleFromQuery(const QSqlQuery& query) {
    Sample sample(query.value("id").toInt());
    
    sample.setName(query.value("name").toString());
    sample.setWidth(query.value("width").toDouble());
    sample.setThickness(query.value("thickness").toDouble());
    sample.setGaugeLength(query.value("gauge_length").toDouble());
    sample.setTestMethod(query.value("test_method").toString());
    sample.setOperatorName(query.value("operator_name").toString());
    
    QString statusStr = query.value("status").toString();
    if (statusStr == "Ready") sample.setStatus(SampleStatus::Ready);
    else if (statusStr == "In Progress") sample.setStatus(SampleStatus::InProgress);
    else if (statusStr == "Completed") sample.setStatus(SampleStatus::Completed);
    else if (statusStr == "Failed") sample.setStatus(SampleStatus::Failed);
    
    sample.setCreatedAt(query.value("created_at").toDateTime());
    sample.setNotes(query.value("notes").toString());
    
    return sample;
}

// ==================== STATISTICS ====================

int SQLiteTestRepository::getTestCount() {
    QSqlQuery query(getDatabase());
    if (!query.exec("SELECT COUNT(*) FROM tests")) {
        return 0;
    }
    
    return query.next() ? query.value(0).toInt() : 0;
}

int SQLiteTestRepository::getTestCountByStatus(TestStatus status) {
    QSqlQuery query(getDatabase());
    query.prepare("SELECT COUNT(*) FROM tests WHERE status = :status");
    query.bindValue(":status", testStatusToString(status));
    
    if (!query.exec() || !query.next()) {
        return 0;
    }
    
    return query.value(0).toInt();
}

} // namespace HorizonUTM
