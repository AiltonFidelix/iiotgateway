package main

import (
	"database/sql"
	"log"
	"net/http"

	"manager/internal/config"
	"manager/internal/handler"

	_ "modernc.org/sqlite"
)

func main() {
	db, err := sql.Open(config.DB_TYPE, config.DB_NAME)

	if err != nil {
		log.Fatal(err)
	}

	defer db.Close()

	managerHandler := handler.NewManagerHandler()

	mux := http.NewServeMux()
	managerHandler.RegisterRoutes(mux)

	log.Printf("Starting manager version %v on port :%v\n", config.VERSION, config.SERVER_PORT)
	log.Fatal(http.ListenAndServe(":8080", mux))
}
