package handler

import (
	"manager/internal/domain"
	"manager/internal/handler/routes"
	"net/http"
)

type ManagerHandler struct {
	// TODO add repos interface
	// TODO add sysctrl interface
	userRepository     domain.UserRepository
	protocolRepository domain.ProtocolRepository
	systemController   domain.SystemController
}

func NewManagerHandler() *ManagerHandler {
	return &ManagerHandler{}
}

func (h *ManagerHandler) SetUserRepository(respository domain.UserRepository) {
	h.userRepository = respository
}

func (h *ManagerHandler) SetProtocolRepository(respository domain.ProtocolRepository) {
	h.protocolRepository = respository
}

func (h *ManagerHandler) RegisterRoutes(mux *http.ServeMux) {
	mux.HandleFunc("POST /iiotgateway/login", h.postLogin)
	mux.HandleFunc("GET /iiotgateway/status", h.GetStatus)
	mux.HandleFunc("POST /iiotgateway/command", h.PostCommand)
	mux.HandleFunc("GET /iiotgateway/protocol", h.GetProtocol)
	mux.HandleFunc("POST /iiotgateway/protocol", h.PostProtocol)
}

func (h *ManagerHandler) postLogin(w http.ResponseWriter, req *http.Request) {
	routes.PostLogin(w, req, h.userRepository)
}

func (h *ManagerHandler) GetStatus(w http.ResponseWriter, req *http.Request) {
	routes.GetStatus(w, req, h.systemController)
}

func (h *ManagerHandler) PostCommand(w http.ResponseWriter, req *http.Request) {
	routes.PostCommand(w, req)
}

func (h *ManagerHandler) GetProtocol(w http.ResponseWriter, req *http.Request) {
	routes.GetProtocol(w, req, h.protocolRepository)
}

func (h *ManagerHandler) PostProtocol(w http.ResponseWriter, req *http.Request) {
	routes.PostProtocol(w, req, h.protocolRepository)
}
