package routes

import (
	"context"
	"encoding/json"
	"log"
	"manager/internal/domain"
	"net/http"
	"time"
)

func PostLogin(w http.ResponseWriter, req *http.Request, repository domain.UserRepository) {

	if repository == nil {
		log.Println("Login failed, repository not valid!")
		http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
		return
	}

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

	userAdmin, err := repository.GetUser(ctx)

	if err != nil {
		log.Println(LOGIN_FAILED_MESSAGE, err)
		http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
		return
	}

	respData := domain.ResponseModel{
		Status:  "ok",
		Message: "Login success!",
	}

	if userAdmin.Name != userRequest.Name || userAdmin.Password != userRequest.Password {
		respData.Status = "error"
		respData.Message = "Wrong credentials!"
		log.Printf("%v %v\n", LOGIN_FAILED_MESSAGE, respData.Message)
	}

	err = json.NewEncoder(w).Encode(respData)

	if err != nil {
		log.Println(LOGIN_FAILED_MESSAGE, err)
		http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
	}
}
