Migrating a C++ monolith to a Go microservice is a great move for an IIoT gateway. Go’s concurrency model, low memory footprint, and robust standard library make it highly suited for this environment.

To meet your requirements—specifically the need to mock the SQLite DB and system calls (`systemctl`) for unit testing—you should use a **Layered Architecture** heavily relying on **Interfaces (Ports and Adapters)**.

Here is the best way to architect this application.

## 1. Directory Structure

Adopt a standard Go project layout. By keeping your business logic in the `internal` directory, you prevent other projects from importing your private gateway code.

```text
iiot-gateway/
├── cmd/
│   └── server/
│       └── main.go           # Application entrypoint, Dependency Injection (wiring)
├── internal/
│   ├── config/               # Environment variables, startup config
│   ├── domain/               # Structs and Interfaces (Models)
│   ├── handler/              # HTTP handlers (Controller layer)
│   ├── repository/           # SQLite DB implementations
│   ├── service/              # Core business logic
│   └── sysctrl/              # Systemctl command implementations
├── go.mod
└── go.sum
```

---

## 2. The Core Concept: Interfaces

To mock your SQLite database and `systemctl` calls, your business logic (`service`) and HTTP endpoints (`handler`) must never depend on the concrete implementations directly. They must depend on interfaces.

### Define your Interfaces (`internal/domain/interfaces.go`)

```go
package domain

import "context"

// Settings represents your DB schema
type Settings struct {
	ID         int
	NetworkIP  string
	MacAddress string
}

// Repository handles database operations
type SettingsRepository interface {
	GetSettings(ctx context.Context) (*Settings, error)
	UpdateSettings(ctx context.Context, s *Settings) error
}

// SystemController handles OS-level commands (systemctl)
type SystemController interface {
	RestartService(ctx context.Context, serviceName string) error
	GetServiceStatus(ctx context.Context, serviceName string) (string, error)
}

```

---

## 3. Implement the Concrete Layers

### The Database Adapter (`internal/repository/sqlite.go`)

Implement the DB logic using a SQLite driver (e.g., `modernc.org/sqlite` is highly recommended for IIoT over `mattn/go-sqlite3` because it doesn't require CGO, making cross-compilation for edge devices much easier).

```go
package repository

import (
	"database/sql"
	"context"
	"iiot-gateway/internal/domain"
)

type sqliteRepo struct {
	db *sql.DB
}

func NewSQLiteRepository(db *sql.DB) domain.SettingsRepository {
	return &sqliteRepo{db: db}
}

func (r *sqliteRepo) GetSettings(ctx context.Context) (*domain.Settings, error) {
	// Execute your SELECT query here
	return &domain.Settings{ID: 1, NetworkIP: "192.168.1.100"}, nil
}

func (r *sqliteRepo) UpdateSettings(ctx context.Context, s *domain.Settings) error {
	// Execute your UPDATE query here
	return nil
}
```

### The System Adapter (`internal/sysctrl/systemctl.go`)

Wrap the `os/exec` calls to `systemctl`. By hiding this behind an interface, you don't actually restart your dev machine or CI pipeline when running tests.

```go
package sysctrl

import (
	"context"
	"os/exec"
	"iiot-gateway/internal/domain"
)

type linuxSysController struct{}

func NewLinuxSystemController() domain.SystemController {
	return &linuxSysController{}
}

func (c *linuxSysController) RestartService(ctx context.Context, serviceName string) error {
	cmd := exec.CommandContext(ctx, "systemctl", "restart", serviceName)
	return cmd.Run()
}

func (c *linuxSysController) GetServiceStatus(ctx context.Context, serviceName string) (string, error) {
	cmd := exec.CommandContext(ctx, "systemctl", "is-active", serviceName)
	out, err := cmd.Output()
	return string(out), err
}
```

---

## 4. The Handler Layer (HTTP Routing)

If you are using Go 1.22+, the standard library `net/http` now natively supports HTTP method routing, meaning you don't even need a third-party framework like Gin or Chi.

Pass your interfaces into your handler struct.

```go
package handler

import (
	"net/http"
	"iiot-gateway/internal/domain"
)

type GatewayHandler struct {
	repo    domain.SettingsRepository
	sysCtrl domain.SystemController
}

func NewGatewayHandler(repo domain.SettingsRepository, sysCtrl domain.SystemController) *GatewayHandler {
	return &GatewayHandler{repo: repo, sysCtrl: sysCtrl}
}

func (h *GatewayHandler) RegisterRoutes(mux *http.ServeMux) {
	mux.HandleFunc("POST /iiotgateway/login", h.loginPost)
	mux.HandleFunc("GET /iiotgateway/status", h.statusGet)
	mux.HandleFunc("POST /iiotgateway/command", h.commandPost)
	mux.HandleFunc("GET /iiotgateway/communication", h.communicationGet)
	mux.HandleFunc("POST /iiotgateway/communication", h.communicationPost)
	mux.HandleFunc("GET /iiotgateway/network", h.networkGet)
	mux.HandleFunc("POST /iiotgateway/network", h.networkPost)
}

func (h *GatewayHandler) networkGet(w http.ResponseWriter, r *http.Request) {
	// Using the injected repository!
	settings, err := h.repo.GetSettings(r.Context())
	if err != nil {
		http.Error(w, "DB Error", http.StatusInternalServerError)
		return
	}
	// return settings as JSON...
}
```

---

## 5. Wiring it together in `main.go`

Your `main.go` acts as the dependency injection container. This is where you decide to use the *real* implementations.

```go
package main

import (
	"database/sql"
	"net/http"
	"log"

	_ "modernc.org/sqlite" // Pure Go SQLite driver

	"iiot-gateway/internal/handler"
	"iiot-gateway/internal/repository"
	"iiot-gateway/internal/sysctrl"
)

func main() {
	// 1. Init real Database
	db, err := sql.Open("sqlite", "./gateway.db")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	// 2. Init implementations
	repo := repository.NewSQLiteRepository(db)
	sysController := sysctrl.NewLinuxSystemController()

	// 3. Init Handlers with dependencies injected
	gatewayHandler := handler.NewGatewayHandler(repo, sysController)

	// 4. Setup Router (Go 1.22+)
	mux := http.NewServeMux()
	gatewayHandler.RegisterRoutes(mux)

	// 5. Start Server
	log.Println("Starting IIoT Gateway on :8080")
	log.Fatal(http.ListenAndServe(":8080", mux))
}
```

---

## 6. How to Unit Test (Mocking)

Because the HTTP handler relies on interfaces, you can easily mock the database and `systemctl` for your unit tests. You can write mocks manually, or use a tool like `uber-go/mock` (formerly `gomock`) or `testify/mock`.

Here is an example of a manual mock in your test file (`handler_test.go`):

```go
package handler

import (
	"net/http"
	"net/http/httptest"
	"testing"
	"context"
	"iiot-gateway/internal/domain"
)

// --- Mock DB ---
type mockSettingsRepo struct{}

func (m *mockSettingsRepo) GetSettings(ctx context.Context) (*domain.Settings, error) {
	return &domain.Settings{ID: 99, NetworkIP: "10.0.0.1"}, nil
}
func (m *mockSettingsRepo) UpdateSettings(ctx context.Context, s *domain.Settings) error {
	return nil
}

// --- Mock Systemctl ---
type mockSysCtrl struct{}

func (m *mockSysCtrl) RestartService(ctx context.Context, name string) error { return nil }
func (m *mockSysCtrl) GetServiceStatus(ctx context.Context, name string) (string, error) { return "active", nil }


func TestNetworkGet(t *testing.T) {
	// Inject the mocks
	h := NewGatewayHandler(&mockSettingsRepo{}, &mockSysCtrl{})

	req := httptest.NewRequest(http.MethodGet, "/iiotgateway/network", nil)
	w := httptest.NewRecorder()

	h.networkGet(w, req)

	res := w.Result()
	if res.StatusCode != http.StatusOK {
		t.Errorf("Expected status 200, got %d", res.StatusCode)
	}
	// Assert JSON payload matches "10.0.0.1"
}
```

---

In Go, it is highly recommended to **avoid** creating a single, massive interface for your entire database.

Instead, you should follow the **Interface Segregation Principle**: create small, focused interfaces for each distinct domain concept or table (often called an "Aggregate Root" in Domain-Driven Design).

If your handler only needs to read network settings, it shouldn't be forced to depend on a giant interface that also knows how to delete logs or update telemetry data. Keeping them separate makes your code easier to read and your unit tests much smaller, as you only need to mock the specific repositories a handler actually uses.

Here is how you handle multiple tables gracefully.

## 1. Define Segregated Interfaces

Keep your interfaces in the `domain` package, grouped logically.

```go
package domain

import "context"

// --- Settings Domain ---
type Settings struct {
	ID        int
	NetworkIP string
}

type SettingsRepository interface {
	GetSettings(ctx context.Context) (*Settings, error)
}

// --- Logs Domain ---
type LogEntry struct {
	ID      int
	Message string
}

type LogRepository interface {
	InsertLog(ctx context.Context, log *LogEntry) error
	GetRecentLogs(ctx context.Context, limit int) ([]*LogEntry, error)
}
```

## 2. Share the DB Connection in Implementations

You still only open **one** SQLite database connection (`*sql.DB`), but you pass that same connection pool to different concrete repository structs.

```go
package repository

import (
	"database/sql"
	"context"
	"iiot-gateway/internal/domain"
)

// --- Settings Implementation ---
type settingsSQLiteRepo struct {
	db *sql.DB
}

func NewSettingsRepository(db *sql.DB) domain.SettingsRepository {
	return &settingsSQLiteRepo{db: db}
}

func (r *settingsSQLiteRepo) GetSettings(ctx context.Context) (*domain.Settings, error) {
	// Query the "settings" table using r.db
	return nil, nil
}

// --- Logs Implementation ---
type logSQLiteRepo struct {
	db *sql.DB
}

func NewLogRepository(db *sql.DB) domain.LogRepository {
	return &logSQLiteRepo{db: db}
}

func (r *logSQLiteRepo) InsertLog(ctx context.Context, log *domain.LogEntry) error {
	// Query the "logs" table using r.db
	return nil
}
```

## 3. Compose the Handler

Inject only the specific repositories your handler needs. If your HTTP handler manages everything, it takes all of them. If you split your handlers up (e.g., `NetworkHandler`, `LoggingHandler`), they only take the repositories they care about.

```go
package handler

import (
	"net/http"
	"iiot-gateway/internal/domain"
)

type GatewayHandler struct {
	settingsRepo domain.SettingsRepository
	logRepo      domain.LogRepository
	sysCtrl      domain.SystemController
}

func NewGatewayHandler(
	settingsRepo domain.SettingsRepository, 
	logRepo domain.LogRepository, 
	sysCtrl domain.SystemController,
) *GatewayHandler {
	return &GatewayHandler{
		settingsRepo: settingsRepo,
		logRepo:      logRepo,
		sysCtrl:      sysCtrl,
	}
}
```

## 4. Wiring it in Main

In your `main.go`, you open the database connection once and fan it out to your repositories. Because `*sql.DB` in Go is a thread-safe connection pool, sharing it across multiple structs is the standard, safe way to operate.

```go
func main() {
	// 1. Open the connection pool exactly once
	db, err := sql.Open("sqlite", "./gateway.db")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	// 2. Instantiate individual repositories sharing the same DB pool
	settingsRepo := repository.NewSettingsRepository(db)
	logRepo := repository.NewLogRepository(db)
	sysController := sysctrl.NewLinuxSystemController()

	// 3. Inject them into the handler
	gatewayHandler := handler.NewGatewayHandler(settingsRepo, logRepo, sysController)
    
	// ... start server
}
```