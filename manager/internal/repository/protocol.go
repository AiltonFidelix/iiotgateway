package repository

import (
	"context"
	"database/sql"
	"errors"
	"manager/internal/domain"
	"time"
)

const (
	queryGetProtocol    = `SELECT type, settings FROM protocols WHERE type = ?`
	queryInsertProtocol = `INSERT INTO protocols (id, type, settings, created, updated) VALUES (NULL, ?, ?, ?, ?)`
	queryUpdateProtocol = `UPDATE protocols SET settings = ?, updated = ? WHERE type = ?`
)

type ProtocolSQLiteRepository struct {
	db *sql.DB
}

func NewProtocolRepository(db *sql.DB) domain.ProtocolRepository {
	return &ProtocolSQLiteRepository{db: db}
}

func (r *ProtocolSQLiteRepository) GetProtocol(ctx context.Context, protocolType string) (*domain.ProtocolModel, error) {
	var protocol domain.ProtocolModel

	err := r.db.QueryRowContext(ctx, queryGetProtocol, protocolType).Scan(
		&protocol.Type,
		&protocol.Settings,
	)

	if err != nil {

		if errors.Is(err, sql.ErrNoRows) {
			return nil, errors.New("Protocol not found")
		}

		return nil, err
	}

	return &protocol, nil
}

func (r *ProtocolSQLiteRepository) InsertProtocol(ctx context.Context, protocol *domain.ProtocolModel) error {

	currentTime := time.Now().Format("2006-01-02 15:04:05")

	_, err := r.db.ExecContext(ctx, queryInsertProtocol, protocol.Type, protocol.Settings, currentTime, currentTime)

	return err
}

func (r *ProtocolSQLiteRepository) UpdateProtocol(ctx context.Context, protocol *domain.ProtocolModel) error {

	currentTime := time.Now().Format("2006-01-02 15:04:05")

	_, err := r.db.ExecContext(ctx, queryUpdateProtocol, protocol.Settings, currentTime, protocol.Type)

	return err
}
