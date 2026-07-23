package main

import (
	"fmt"

	"manager/internal/config"
)

func main() {
	fmt.Println("IIoTGateway manager prototype")
	fmt.Printf("Database type: %v, database name: %v\n", config.DB_TYPE, config.DB_NAME)
}
