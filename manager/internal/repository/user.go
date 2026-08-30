package repository

import (
	"context"
	"database/sql"
	"errors"
	"manager/internal/domain"
)

const (
	queryGetUser = `SELECT username, password FROM users WHERE id = 1`
)

type UserSQLiteRepository struct {
	db *sql.DB
}

func NewUserRepository(db *sql.DB) domain.UserRepository {
	return &UserSQLiteRepository{db: db}
}

func (r *UserSQLiteRepository) GetUser(ctx context.Context) (*domain.UserModel, error) {

	var user domain.UserModel

	err := r.db.QueryRowContext(ctx, queryGetUser).Scan(
		&user.Name,
		&user.Password,
	)

	if err != nil {

		if errors.Is(err, sql.ErrNoRows) {
			return nil, errors.New("User not found")
		}

		return nil, err
	}

	return &user, nil
}
