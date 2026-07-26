package routes

import (
	"fmt"
	"net/http"
)

func GetCommunication(w http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(w, "GetCommunication\n")
}

func PostCommunication(w http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(w, "PostCommunication\n")
}
