package service

import (
	"context"
	"errors"
	"manager/internal/domain"
)

type LoginService struct {
	userRepository domain.UserRepository
}

func NewLoginService(userRepository domain.UserRepository) *LoginService {
	return &LoginService{userRepository: userRepository}
}

func (s *LoginService) CheckCredentials(ctx context.Context, user *domain.UserModel) error {

	userAdmin, err := s.userRepository.GetUser(ctx)

	if err != nil {
		return err
	}

	if userAdmin.Name != user.Name || userAdmin.Password != user.Password {
		return errors.New("Wrong credentials!")
	}

	return nil
}
