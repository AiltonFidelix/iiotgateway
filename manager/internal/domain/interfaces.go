package domain

import (
	"context"
	"time"
)

type ResponseModel struct {
	Status  string `json:"status"`
	Message string `json:"message"`
}

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
	Name     string `json:"username"`
	Password string `json:"password"`
	Created  time.Time
	Updated  time.Time
}

type HistoryRepository interface {
	InsertHistory(ctx context.Context, history *HistoryModel) error
}

type ProtocolRepository interface {
	GetProtocol(ctx context.Context, protocolType string) (*ProtocolModel, error)
	InsertProtocol(ctx context.Context, protocol *ProtocolModel) error
	UpdateProtocol(ctx context.Context, protocol *ProtocolModel) error
}

type SettingsRepository interface {
	GetSettings(ctx context.Context) (*SettingsModel, error)
	UpdateSettings(ctx context.Context, settings *SettingsModel) error
}

type UserRepository interface {
	GetUser(ctx context.Context) (*UserModel, error)
}

type SystemController interface {
	StartService(ctx context.Context, serviceName string) error
	StopService(ctx context.Context, serviceName string) error
	RestartService(ctx context.Context, serviceName string) error
	GetServiceStatus(ctx context.Context, serviceName string) (string, error)
}
