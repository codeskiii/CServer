#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HTTPServerHelpers.c"

#pragma comment(lib, "ws2_32.lib") // Link with Ws2_32.lib

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024

void handle_client(SOCKET client_socket);
char *read_file(const char *path, long *out_size);
void route_request(const char *path, char **response, long *file_size);

int main() {
    WSADATA wsaData;
    SOCKET listen_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    // Create a socket for listening for incoming connections
    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == INVALID_SOCKET) {
        printf("Socket creation failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    server_addr.sin_port = htons(DEFAULT_PORT);

    // Bind the socket
    if (bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is listening on port %d...\n", DEFAULT_PORT);
    printf("Server is listening on http://0.0.0.0:%d\n", DEFAULT_PORT);

    // Main loop to accept and handle client connections
    while (1) {
        client_socket = accept(listen_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed with error: %d\n", WSAGetLastError());
            closesocket(listen_socket);
            WSACleanup();
            return 1;
        }

        handle_client(client_socket);
    }

    // Clean up
    closesocket(listen_socket);
    WSACleanup();

    return 0;
}

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;
    char method[BUFFER_SIZE];
    char path[BUFFER_SIZE];
    char *response;
    long file_size;

    // Receive data from the client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        return;
    }

    // Null-terminate the received data
    buffer[bytes_received] = '\0';

    // Parse the HTTP request
    sscanf(buffer, "%s %s", method, path);
    printf("Request: %s %s\n", method, path);

    // Only handle GET requests
    if (strcmp(method, "GET") == 0) {
        // Route the request to the appropriate handler
        route_request(path, &response, &file_size);

        if (response) {
            // Send HTTP response headers
            char headers[BUFFER_SIZE];
            snprintf(headers, sizeof(headers),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: %ld\r\n"
                     "\r\n", file_size);
            send(client_socket, headers, strlen(headers), 0);

            // Send the file content
            send(client_socket, response, file_size, 0);
            free(response);
        } else {
            // File not found, send 404 response
            const char *not_found_response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 9\r\n"
                "\r\n"
                "Not Found";
            send(client_socket, not_found_response, strlen(not_found_response), 0);
        }
    } else {
        // Method not allowed, send 405 response
        const char *method_not_allowed_response =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 18\r\n"
            "\r\n"
            "Method Not Allowed";
        send(client_socket, method_not_allowed_response, strlen(method_not_allowed_response), 0);
    }

    // Clean up
    closesocket(client_socket);
}

void route_request(const char *path, char **response, long *file_size) {
    if strcmp(path, "/") == 0 || strcmp(path, 'leaderboard') {
        *response = read_file("index.html", file_size);
    } else if (strcmp(path, "profile")) {
        /* BLUEPRINT LADING HERE */
    } else {
        /* LOAD ERROR PAGE HERE */
    }

    // OLD -> Handle different routes
    /*
    if (strcmp(path, "/") == 0 || strcmp(path, "/home") == 0) {
        // Serve index.html or home.html
        *response = read_file("index.html", file_size);
    } else if (strcmp(path, "/about") == 0) {
        // Serve about.html
        *response = read_file("about.html", file_size);
    } else {
        // Path not found
        *response = NULL;
    }
    */
}