package routes

import (
	"fmt"
	"net/http"
)

func PostCommand(w http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(w, "PostCommand\n")
}
