#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[1024];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(3005);

    // Bind the socket to the specified IP and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind." << std::endl;
        return 1;
    }

    // Listen for incoming connections
    listen(serverSocket, 1);
    std::cout << "Server listening on port 3005..." << std::endl;

    // Accept a client connection
    socklen_t clientAddressSize = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept client connection." << std::endl;
        return 1;
    }

    // Receive and ignore client messages
    ssize_t bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        // Print the received data
        std::cout << "Received message from client: " << buffer << std::endl;
    }

    if (bytesRead < 0) {
        std::cerr << "Failed to receive data from the client." << std::endl;
        return 1;
    }

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
