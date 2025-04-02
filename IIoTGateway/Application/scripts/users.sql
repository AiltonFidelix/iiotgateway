CREATE TABLE users (
    id INTEGER PRIMARY KEY,
    username TEXT,
    password TEXT,                          
    created TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO users (id, username, password, created, updated) VALUES (1, 'admin', '7a819edb7b83634df71702372cdbfbb7a66278cf', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);