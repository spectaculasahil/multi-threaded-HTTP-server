#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>

#define PORT 8080
#define BACKLOG 10
#define BUFFER_SIZE 1024
#define MAX_PATH 256

// Structure to pass client info to threads
typedef struct {
    int client_fd;
} client_info_t;

// Send HTTP response
void send_response(int client_fd, const char *status, const char *content_type, const char *body, size_t body_len) {
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE,
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n",
             status, content_type, body_len);
    write(client_fd, response, strlen(response));
    if (body_len > 0) {
        write(client_fd, body, body_len);
    }
}

// Serve a file from the current directory
int serve_file(int client_fd, const char *path) {
    char full_path[MAX_PATH];
    snprintf(full_path, MAX_PATH, ".%s", path); // Serve from current directory

    int fd = open(full_path, O_RDONLY);
    if (fd < 0) {
        send_response(client_fd, "404 Not Found", "text/plain", "File not found", 13);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    char header[BUFFER_SIZE];
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Connection: close\r\n"
             "\r\n");
    write(client_fd, header, strlen(header));

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        write(client_fd, buffer, bytes_read);
    }
    close(fd);
    return 0;
}

// Thread function to handle client requests
void *handle_client(void *arg) {
    client_info_t *info = (client_info_t *)arg;
    int client_fd = info->client_fd;
    free(info);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (bytes_read <= 0) {
        close(client_fd);
        return NULL;
    }
    buffer[bytes_read] = '\0';

    // Parse GET request
    char method[16], path[MAX_PATH], protocol[16];
    if (sscanf(buffer, "%s %s %s", method, path, protocol) != 3) {
        send_response(client_fd, "400 Bad Request", "text/plain", "Invalid request", 14);
        close(client_fd);
        return NULL;
    }

    if (strcmp(method, "GET") != 0) {
        send_response(client_fd, "405 Method Not Allowed", "text/plain", "Only GET supported", 18);
        close(client_fd);
        return NULL;
    }

    // Handle root path or serve file
    if (strcmp(path, "/") == 0) {
        send_response(client_fd, "200 OK", "text/plain", "Hello, World!", 13);
    } else {
        serve_file(client_fd, path);
    }

    close(client_fd);
    return NULL;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Allow port reuse
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen for connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept loop
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        // Create thread for client
        pthread_t thread;
        client_info_t *info = malloc(sizeof(client_info_t));
        info->client_fd = client_fd;

        if (pthread_create(&thread, NULL, handle_client, info) != 0) {
            perror("Thread creation failed");
            close(client_fd);
            free(info);
            continue;
        }

        pthread_detach(thread); // Detach thread to clean up automatically
    }

    close(server_fd);
    return 0;
}