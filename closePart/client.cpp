#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include "../includes/Server.hpp"

#define BUFFER_SIZE 512

class SimpleIRCClient {
private:
    int sock_fd;
    
public:
    SimpleIRCClient(const char* server_ip, int port) {
        // Create socket
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd == -1)
            throw std::runtime_error("socket() failed");
        
        // Connect to server
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, server_ip, &addr.sin_addr) <= 0)
            throw std::runtime_error("Invalid address");
        
        if (connect(sock_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
            throw std::runtime_error("connect() failed");
        
        LOG(CLIENT, "Connected to server at " << server_ip << ":" << port);
    }
     
    void sendCommand(const std::string& cmd) {
        std::string full_cmd = cmd + "\r\n";
        ssize_t sent = send(sock_fd, full_cmd.c_str(), full_cmd.length(), 0);
        
        if (sent == -1) {
            LOG(ERROR, "send() failed");
        } else {
            LOG(INFO, "[SENT] " << cmd);
        }
    }
    
    void receiveResponse() {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        
        ssize_t bytes = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes > 0) {
            LOG(INFO, "[RECV] " << buffer);
        } else if (bytes == 0) {
            LOG(DISCONNECT, "Server disconnected");
        } else {
            LOG(ERROR, "recv() failed");
        }
    }
    
    void runTests() {
        LOG(CLIENT, "\n=== Starting IRC Tests ===\n");
        
        // Test 1: NICK command
        LOG(CLIENT, "Test 1: Setting nickname");
        sendCommand("NICK testuser");
        receiveResponse();
        
        sleep(10);
        
        // Test 2: USER command
        LOG(CLIENT, "\nTest 2: User registration");
        sendCommand("USER testuser 0 * :Test User leeeeeeeeehwak");
        receiveResponse();
        
        sleep(10);
        
        // Test 3: PING/PONG
        LOG(CLIENT, "\nTest 3: PING/PONG");
        sendCommand("PING :test123");
        receiveResponse();
        
        sleep(10);
        
        // Test 4: PRIVMSG
        LOG(CLIENT, "\nTest 4: Private message");
        sendCommand("PRIVMSG #channel :Hello, World!");
        receiveResponse();
        
        sleep(10);
        
        // Test 5: Multiple commands at once (buffer test)
        LOG(CLIENT, "\nTest 5: Multiple commands");
        sendCommand("NICK user2");
        sendCommand("PING :test456");
        receiveResponse();
        receiveResponse();
        
        sleep(10);
        
        // Test 6: QUIT
        LOG(CLIENT, "\nTest 6: Disconnecting");
        sendCommand("QUIT :Goodbye!");
        receiveResponse();
        
        LOG(CLIENT, "\n=== Tests Complete ===\n");
    }

    ~SimpleIRCClient() {
        close(sock_fd);
    }
};

int main() {
    try {
        SimpleIRCClient client("127.0.0.1", 6667);
        
        // Receive welcome message
        client.receiveResponse();
        
        // Run automated tests
        client.runTests();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}