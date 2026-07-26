package handler

import (
	"manager/internal/handler/routes"
	"net/http"
)

type ManagerHandler struct {
	// TODO add repos interface
	// TODO add sysctrl interface
}

func NewManagerHandler() *ManagerHandler {
	return &ManagerHandler{}
}

func (h *ManagerHandler) RegisterRoutes(mux *http.ServeMux) {
	mux.HandleFunc("POST /iiotgateway/login", routes.PostLogin)
	mux.HandleFunc("GET /iiotgateway/status", routes.GetStatus)
	mux.HandleFunc("POST /iiotgateway/command", routes.PostCommand)
	mux.HandleFunc("GET /iiotgateway/communication", routes.GetCommunication)
	mux.HandleFunc("POST /iiotgateway/communication", routes.PostCommunication)
}
