package routes

import (
	"context"
	"encoding/json"
	"log"
	"manager/internal/domain"
	"net/http"
	"time"
)

func GetStatus(w http.ResponseWriter, req *http.Request, controller domain.SystemController) {

	// TODO Remove comment
	// if controller == nil {
	// 	log.Println("Get status failed, controller not valid!")
	// 	http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
	// 	return
	// }

	ctx := req.Context()

	ctx, cancel := context.WithTimeout(ctx, 5*time.Second)

	defer cancel()

	respData := domain.ResponseModel{
		Status:  "ok",      // error
		Message: "Stopped", // Running
	}

	// TODO Read gateway status using the controller

	err := json.NewEncoder(w).Encode(respData)

	if err != nil {
		log.Println(STATUS_FAILED_MESSAGE, err)
		http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
	}
}
