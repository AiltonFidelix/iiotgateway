package routes

import (
	"fmt"
	"net/http"
)

func PostLogin(w http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(w, "PostLogin\n")
}
