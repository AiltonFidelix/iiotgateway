package handler

import (
	"context"
	"encoding/json"
	"log"
	"manager/internal/domain"
	"manager/internal/service"
	"net/http"
	"time"
)

type LoginHandler struct {
	loginService *service.LoginService
}

func NewLoginHandler(loginService *service.LoginService) *LoginHandler {
	return &LoginHandler{loginService: loginService}
}

func (h *LoginHandler) RegisterRoutes(mux *http.ServeMux) {
	mux.HandleFunc("POST /iiotgateway/login", h.postLogin)
}

func (h *LoginHandler) postLogin(w http.ResponseWriter, req *http.Request) {

	var userRequest domain.UserModel

	err := json.NewDecoder(req.Body).Decode(&userRequest)

	if err != nil {
		log.Println(LOGIN_FAILED_MESSAGE, err)
		http.Error(w, "Invalid JSON", http.StatusBadRequest)
		return
	}

	ctx := req.Context()

	ctx, cancel := context.WithTimeout(ctx, 5*time.Second)

	defer cancel()

	respData := domain.ResponseModel{
		Status:  "ok",
		Message: "Login success!",
	}

	err = h.loginService.CheckCredentials(ctx, &userRequest)

	if err != nil {
		respData.Status = "error"
		respData.Message = err.Error()
		log.Printf("%v %v\n", LOGIN_FAILED_MESSAGE, respData.Message)
	}

	err = json.NewEncoder(w).Encode(respData)

	if err != nil {
		log.Println(LOGIN_FAILED_MESSAGE, err)
		http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
	}
}
