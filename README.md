# Multi-Threaded HTTP Server in C

This project implements a simple multi-threaded HTTP server in C, designed to handle multiple client connections concurrently using POSIX threads (pthreads). The server listens on a specified port (default: `8080`), processes basic HTTP GET requests, and serves either a fixed response or static files (e.g., HTML) from the local directory.

> Inspired by projects like [orlovD/HTTPServer](https://github.com/orlovD/HTTPServer), this project demonstrates socket programming, multi-threading, and basic HTTP protocol handling on Linux.

---

## 🔧 Features

- **Concurrent Connections**: Handles multiple clients simultaneously by spawning a new thread for each connection.
- **HTTP GET Support**:
  - Returns a fixed `"Hello, World!"` response for the root path (`/`)
  - Serves static files (e.g., `index.html`) for valid file paths
- **POSIX Compliance**: Uses standard Linux socket APIs and pthreads for portability across Unix-like systems.
- **Simple and Lightweight**: Minimal implementation ideal for learning and prototyping.

---

## 🛠 Technologies

- **Language**: C
- **Libraries**: POSIX threads (`pthread`), Linux socket APIs (`sys/socket.h`, `arpa/inet.h`)
- **Platform**: Linux/Unix-like systems (e.g., Ubuntu, macOS, WSL2)
- **Compiler**: `gcc` with `-pthread` flag

---

## ✅ Prerequisites

- **Operating System**: Linux or Unix-like (e.g., Ubuntu, macOS, WSL2)
- **Tools**:
  - `gcc` (GNU Compiler Collection)
  - `libc6-dev` (for socket and pthread headers)
  - `curl` or a web browser (for testing)


