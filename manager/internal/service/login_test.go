package service

import (
	"context"
	"errors"
	"manager/internal/domain"
	"testing"
)

type MockUserRepository struct {
	GetUserFn func(ctx context.Context) (*domain.UserModel, error)
}

func (m *MockUserRepository) GetUser(ctx context.Context) (*domain.UserModel, error) {

	if m.GetUserFn != nil {
		return m.GetUserFn(ctx)
	}

	return nil, errors.New("GetUserFn not implemented")
}

func TestLoginService_CheckCredentials(t *testing.T) {

	ctx := context.Background()

	t.Run("success - right credentials", func(t *testing.T) {

		mockRepo := &MockUserRepository{
			GetUserFn: func(ctx context.Context) (*domain.UserModel, error) {
				return &domain.UserModel{Name: "admin", Password: "somedummypassword"}, nil
			},
		}

		service := NewLoginService(mockRepo)

		err := service.CheckCredentials(ctx, &domain.UserModel{Name: "admin", Password: "somedummypassword"})

		if err != nil {
			t.Fatalf("expected no error, got %v", err)
		}
	})

	t.Run("failure - wrong credentials", func(t *testing.T) {

		mockRepo := &MockUserRepository{
			GetUserFn: func(ctx context.Context) (*domain.UserModel, error) {
				return &domain.UserModel{Name: "admin", Password: "somedummypassword"}, nil
			},
		}

		service := NewLoginService(mockRepo)

		err := service.CheckCredentials(ctx, &domain.UserModel{Name: "admin", Password: "somepassword"})

		if err == nil {
			t.Fatalf("expected error, got no error")
		}
	})
}
