#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char serverIP[16];
    char message[1024];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Set up server address
    std::cout << "Enter the server IP address: ";
    std::cin >> serverIP;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(3005);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, serverIP, &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid IP address." << std::endl;
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return 1;
    }

    // Send message to the server
    std::cout << "Enter the message to send: ";
    std::cin.ignore(); // Ignore the newline character from previous input
    std::cin.getline(message, sizeof(message));

    ssize_t bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent < 0) {
        std::cerr << "Failed to send data to the server." << std::endl;
        return 1;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}


