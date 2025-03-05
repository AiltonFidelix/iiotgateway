CREATE TABLE protocols (
    id INTEGER PRIMARY KEY,
    name VARCHAR(30) NOT NULL,                    -- Name of the protocol
    settings TEXT,                                -- Will be a JSON with settings
    created TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);