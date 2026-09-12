package handler

import (
	"manager/internal/domain"
	"manager/internal/handler/routes"
	"net/http"
)

// TODO Divide into different handlers using each one service

type ManagerHandler struct {
	// TODO add repos interface
	// TODO add sysctrl interface
	protocolRepository domain.ProtocolRepository
	systemController   domain.SystemController
}

func NewManagerHandler() *ManagerHandler {
	return &ManagerHandler{}
}

func (h *ManagerHandler) SetProtocolRepository(respository domain.ProtocolRepository) {
	h.protocolRepository = respository
}

func (h *ManagerHandler) RegisterRoutes(mux *http.ServeMux) {
	mux.HandleFunc("GET /iiotgateway/status", h.GetStatus)
	mux.HandleFunc("POST /iiotgateway/command", h.PostCommand)
	mux.HandleFunc("GET /iiotgateway/protocol", h.GetProtocol)
	mux.HandleFunc("POST /iiotgateway/protocol", h.PostProtocol)
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
