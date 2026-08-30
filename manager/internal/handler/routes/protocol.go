package routes

import (
	"context"
	"encoding/json"
	"log"
	"manager/internal/domain"
	"net/http"
	"strings"
	"time"
)

func GetProtocol(w http.ResponseWriter, req *http.Request, repository domain.ProtocolRepository) {

	if repository == nil {
		log.Println("Get protocol failed, repository not valid!")
		http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
		return
	}

	ctx := req.Context()

	ctx, cancel := context.WithTimeout(ctx, 5*time.Second)

	defer cancel()

	obj := make(map[string]any)

	for key, values := range req.URL.Query() {

		if strings.ToLower(key) != "protocol" {
			continue
		}

		for _, value := range values {
			protocolType := strings.ToUpper(value)
			protocol, err := repository.GetProtocol(ctx, protocolType)

			if err == nil {
				obj[protocol.Type] = json.RawMessage(protocol.Settings)
			}
		}
	}

	data, err := json.Marshal(obj)

	if err != nil {
		http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
		return
	}

	_, err = w.Write(data)

	if err != nil {
		log.Println(RESPONSE_FAILED_MESSAGE, err)
	}
}

func PostProtocol(w http.ResponseWriter, req *http.Request, repository domain.ProtocolRepository) {

	if repository == nil {
		log.Println("Login failed, repository not valid!")
		http.Error(w, INTERNAL_ERROR_MESSAGE, http.StatusInternalServerError)
		return
	}

	// TODO Implement
}
