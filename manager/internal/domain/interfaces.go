package domain

import (
	"context"
	"time"
)

type HistoryModel struct {
	ID      int
	Script  string
	Created time.Time
}

type ProtocolModel struct {
	ID       int
	Type     string
	Settings string
	Created  time.Time
	Updated  time.Time
}

type SettingsModel struct {
	ID            int
	Active        bool
	CloudProtocol string
	EdgeProtocol  string
	Created       time.Time
	Updated       time.Time
}

type UserModel struct {
	ID       int
	UserName string
	Password string
	Created  time.Time
	Updated  time.Time
}

type HistoryRepository interface {
	GetHistory(ctx context.Context) (*HistoryModel, error)
	UpdateHistory(ctx context.Context, s *HistoryModel) error
}

type ProtocolRepository interface {
	GetProtocol(ctx context.Context) (*ProtocolModel, error)
	UpdateProtocol(ctx context.Context, s *ProtocolModel) error
}

type SettingsRepository interface {
	GetSettings(ctx context.Context) (*SettingsModel, error)
	UpdateSettings(ctx context.Context, s *SettingsModel) error
}

type UserRepository interface {
	GetUser(ctx context.Context) (*UserModel, error)
	UpdateUser(ctx context.Context, s *UserModel) error
}

type SystemController interface {
	StartService(ctx context.Context, serviceName string) error
	StopService(ctx context.Context, serviceName string) error
	RestartService(ctx context.Context, serviceName string) error
	GetServiceStatus(ctx context.Context, serviceName string) (string, error)
}
