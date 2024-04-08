#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int serverSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Set server address
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind socket to server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket." << std::endl;
        return 1;
    }

    std::cout << "Server started. Listening on port " << PORT << std::endl;

    while (true) {
        socklen_t clientAddressLength = sizeof(clientAddress);

        // Receive message from client
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (bytesRead < 0) {
            std::cerr << "Failed to receive message." << std::endl;
            continue;
        }

        // Print received message
        std::cout << "Received message from " << inet_ntoa(clientAddress.sin_addr) << ": " << buffer << std::endl;

        // Send message back to client
        ssize_t bytesSent = sendto(serverSocket, buffer, bytesRead, 0, (struct sockaddr*)&clientAddress, clientAddressLength);
        if (bytesSent < 0) {
            std::cerr << "Failed to send message." << std::endl;
            continue;
        }
    }

    // Close socket
    close(serverSocket);

    return 0;
}
