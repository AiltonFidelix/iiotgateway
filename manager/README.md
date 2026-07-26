# Manager

Manager application from the IIoTGateway platform.

Structure:

```text
manager/
├── cmd/
│   └── manager/
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

## Commands

Run:

```sh
go run cmd/manager/main.go
```

Build:

```sh
go build -o manager cmd/manager/main.go
```

## TODO

- Implement routes
- Implement repositories
- Implement system controller
- Add db migration
- Add unit tests