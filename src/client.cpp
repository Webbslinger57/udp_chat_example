#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set up server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <= 0) {
        std::cerr << "Invalid server address" << std::endl;
        return 1;
    }

    // Start the chat loop
    std::string message;
    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        // Send the message to the server
        if (sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Failed to send message" << std::endl;
            return 1;
        }

        // Receive response from the server
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        socklen_t serverAddrLen = sizeof(serverAddr);
        ssize_t bytesRead = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&serverAddr, &serverAddrLen);
        if (bytesRead < 0) {
            std::cerr << "Failed to receive response" << std::endl;
            return 1;
        }

        // Print the response
        std::cout << "Server response: " << buffer << std::endl;
    }

    // Close the socket
    close(sockfd);

    return 0;
}
