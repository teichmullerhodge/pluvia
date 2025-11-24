CREATE INDEX IF NOT EXISTS idx_cities_name ON cities(name);
CREATE INDEX IF NOT EXISTS idx_cities_admin1_code ON cities(admin1_code);
CREATE UNIQUE INDEX IF NOT EXISTS idx_states_admin1_code ON states(admin1_code);
