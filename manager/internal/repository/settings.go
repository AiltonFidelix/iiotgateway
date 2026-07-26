package repository

import (
	"context"
	"database/sql"
	"manager/internal/domain"
)

type SettingsSQLiteRepository struct {
	db *sql.DB
}

func NewSettingsRepository(db *sql.DB) domain.SettingsRepository {
	return &SettingsSQLiteRepository{db: db}
}

func (r *SettingsSQLiteRepository) GetSettings(ctx context.Context) (*domain.SettingsModel, error) {
	// TODO Implement
	return &domain.SettingsModel{}, nil
}

func (r *SettingsSQLiteRepository) UpdateSettings(ctx context.Context, s *domain.SettingsModel) error {
	// TODO Implement
	return nil
}
