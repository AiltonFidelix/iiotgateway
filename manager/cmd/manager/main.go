package main

import (
	"database/sql"
	"fmt"
	"log"
	"net/http"

	"manager/internal/config"
	"manager/internal/handler"
	"manager/internal/repository"
	"manager/internal/service"

	_ "modernc.org/sqlite"
)

func enableCORS(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {

		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE")
		w.Header().Set("Access-Control-Allow-Headers", "Accept, Content-Type, Content-Length, Accept-Encoding, X-CSRF-Token, Authorization")

		if r.Method == http.MethodOptions {
			w.WriteHeader(http.StatusOK)
			return
		}

		next.ServeHTTP(w, r)
	})
}

func main() {
	db, err := sql.Open(config.DB_TYPE, config.DB_NAME)

	if err != nil {
		log.Fatal(err)
	}

	defer db.Close()

	mux := http.NewServeMux()

	userRepository := repository.NewUserRepository(db)
	loginService := service.NewLoginService(userRepository)
	loginHandler := handler.NewLoginHandler(loginService)
	loginHandler.RegisterRoutes(mux)

	protocolRepository := repository.NewProtocolRepository(db)
	managerHandler := handler.NewManagerHandler()
	managerHandler.SetProtocolRepository(protocolRepository)

	managerHandler.RegisterRoutes(mux)

	portStr := fmt.Sprintf(":%d", config.SERVER_PORT)

	log.Printf("Starting manager version %v on port %v\n", config.VERSION, portStr)
	log.Fatal(http.ListenAndServe(portStr, enableCORS(mux)))
}
