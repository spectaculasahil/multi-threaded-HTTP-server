Multi-Threaded HTTP Server in C

This project implements a simple multi-threaded HTTP server in C, designed to handle multiple client connections concurrently using POSIX threads (pthreads). The server listens on a specified port (default: 8080), processes basic HTTP GET requests, and serves either a fixed response or static files (e.g., HTML) from the local directory. It is inspired by projects like orlovD/HTTPServer and focuses on demonstrating socket programming, multi-threading, and basic HTTP protocol handling on Linux.

Features





Concurrent Connections: Handles multiple clients simultaneously by spawning a new thread for each connection.



HTTP GET Support: Parses basic GET requests and responds with:





A fixed "Hello, World!" response for the root path (/).



Static files (e.g., index.html) for other valid paths.



POSIX Compliance: Uses standard Linux socket APIs and pthreads for portability across Unix-like systems.



Simple and Lightweight: Minimal implementation suitable for learning and prototyping.

Technologies





Language: C



Libraries: POSIX threads (pthread), Linux socket APIs (sys/socket.h, arpa/inet.h)



Platform: Linux/Unix-like systems (e.g., Ubuntu, macOS, WSL2)



Compiler: gcc with -pthread flag

Project Structure

http_server_project/
├── http_server.c    # Main server implementation
├── index.html       # Optional: Example static file for testing
├── README.md        # This file

Prerequisites





Operating System: Linux or Unix-like (e.g., Ubuntu, macOS, WSL2 on Windows)



Tools:





gcc (GNU Compiler Collection)



libc6-dev (for socket and pthread headers)



Optional: curl or a web browser for testing

Install dependencies on Ubuntu/Debian:

sudo apt-get update
sudo apt-get install build-essential libc6-dev

Setup Instructions





Clone the Repository:

git clone <repository-url>
cd http_server_project



Create Optional Test File: Create an index.html file in the project directory to test file serving:

<!DOCTYPE html>
<html>
<head>
    <title>Test Page</title>
</head>
<body>
    <h1>Welcome to My HTTP Server</h1>
    <p>This is a test page served by our multi-threaded HTTP server.</p>
</body>
</html>



Compile the Code:

gcc -o http_server http_server.c -pthread



Run the Server:

./http_server

The server will listen on port 8080 and print:

Server listening on port 8080...

Usage





Test with a Browser:





Open http://localhost:8080 to see "Hello, World!".



Open http://localhost:8080/index.html to serve the index.html file (if created).



Test with curl:

curl http://localhost:8080
curl http://localhost:8080/index.html



Test Concurrency:





Open multiple browser tabs or run multiple curl commands simultaneously to verify concurrent handling.

How It Works





Socket Setup: Creates a TCP socket, binds to port 8080, and listens for connections.



Multi-Threading: Spawns a new pthread for each client connection to handle requests concurrently.



HTTP Handling:





Parses GET requests (e.g., GET /index.html HTTP/1.1).



Responds with HTTP/1.1-compliant responses (status, headers, body).



Serves files from the current directory or a fixed response for the root path.



Thread Management: Detaches threads to clean up automatically after handling clients.
