CREATE TABLE protocols (
    id INTEGER PRIMARY KEY,
    type VARCHAR(30) NOT NULL,                    -- Type of the protocol
    settings TEXT,                                -- Will be a JSON with settings
    created TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);