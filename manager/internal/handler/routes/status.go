package routes

import (
	"fmt"
	"net/http"
)

func GetStatus(w http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(w, "GetStatus\n")
}
