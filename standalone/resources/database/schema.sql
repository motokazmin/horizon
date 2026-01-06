-- Horizon UTM Database Schema
-- SQLite3

-- Tests table
CREATE TABLE IF NOT EXISTS tests (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    sample_name TEXT NOT NULL,
    operator_name TEXT NOT NULL,
    test_method TEXT NOT NULL,
    start_time INTEGER NOT NULL,
    end_time INTEGER,
    status TEXT NOT NULL CHECK(status IN ('running', 'completed', 'failed', 'stopped')),
    
    -- Sample dimensions
    width REAL NOT NULL,
    thickness REAL NOT NULL,
    gauge_length REAL NOT NULL,
    cross_section_area REAL NOT NULL,
    
    -- Test parameters
    speed REAL NOT NULL,
    force_limit REAL NOT NULL,
    temperature REAL,
    
    -- Results
    max_stress REAL,
    max_strain REAL,
    yield_stress REAL,
    yield_strain REAL,
    ultimate_stress REAL,
    ultimate_strain REAL,
    break_stress REAL,
    break_strain REAL,
    elastic_modulus REAL,
    elongation_at_break REAL,
    
    -- Metadata
    created_at INTEGER NOT NULL DEFAULT (strftime('%s', 'now') * 1000),
    updated_at INTEGER NOT NULL DEFAULT (strftime('%s', 'now') * 1000),
    notes TEXT
);

-- Test data points table (sensor readings)
CREATE TABLE IF NOT EXISTS test_data_points (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    test_id INTEGER NOT NULL,
    timestamp INTEGER NOT NULL,
    force REAL NOT NULL,
    extension REAL NOT NULL,
    stress REAL NOT NULL,
    strain REAL NOT NULL,
    temperature REAL,
    
    FOREIGN KEY (test_id) REFERENCES tests(id) ON DELETE CASCADE
);

-- Samples table (pre-defined samples in queue)
CREATE TABLE IF NOT EXISTS samples (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    width REAL NOT NULL,
    thickness REAL NOT NULL,
    gauge_length REAL NOT NULL,
    test_method TEXT NOT NULL,
    operator_name TEXT NOT NULL,
    status TEXT NOT NULL CHECK(status IN ('ready', 'in_progress', 'completed', 'failed')),
    created_at INTEGER NOT NULL DEFAULT (strftime('%s', 'now') * 1000),
    notes TEXT
);

-- Create indexes for better performance
CREATE INDEX IF NOT EXISTS idx_tests_status ON tests(status);
CREATE INDEX IF NOT EXISTS idx_tests_start_time ON tests(start_time);
CREATE INDEX IF NOT EXISTS idx_test_data_points_test_id ON test_data_points(test_id);
CREATE INDEX IF NOT EXISTS idx_test_data_points_timestamp ON test_data_points(timestamp);
CREATE INDEX IF NOT EXISTS idx_samples_status ON samples(status);

-- Trigger to update updated_at timestamp
CREATE TRIGGER IF NOT EXISTS update_tests_timestamp 
AFTER UPDATE ON tests
BEGIN
    UPDATE tests SET updated_at = (strftime('%s', 'now') * 1000) WHERE id = NEW.id;
END;
