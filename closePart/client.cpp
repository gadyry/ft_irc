#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

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
        
        std::cout << "Connected to server at " << server_ip << ":" << port << std::endl;
    }
     
    void sendCommand(const std::string& cmd) {
        std::string full_cmd = cmd + "\r\n";
        ssize_t sent = send(sock_fd, full_cmd.c_str(), full_cmd.length(), 0);
        
        if (sent == -1) {
            std::cerr << "send() failed" << std::endl;
        } else {
            std::cout << "Sent: " << cmd << std::endl;
        }
    }
    
    void receiveResponse() {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        
        ssize_t bytes = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes > 0) {
            std::cout << "Received: " << buffer;
        } else if (bytes == 0) {
            std::cout << "Server disconnected" << std::endl;
        } else {
            std::cerr << "recv() failed" << std::endl;
        }
    }
    
    void runTests() {
        std::cout << "\n=== Starting IRC Tests ===\n" << std::endl;
        
        // Test 1: NICK command
        std::cout << "Test 1: Setting nickname" << std::endl;
        sendCommand("NICK testuser");
        receiveResponse();
        
        sleep(10);
        
        // Test 2: USER command
        std::cout << "\nTest 2: User registration" << std::endl;
        sendCommand("USER testuser 0 * :Test User leeeeeeeeehwak");
        receiveResponse();
        
        sleep(10);
        
        // Test 3: PING/PONG
        std::cout << "\nTest 3: PING/PONG" << std::endl;
        sendCommand("PING :test123");
        receiveResponse();
        
        sleep(10);
        
        // Test 4: PRIVMSG
        std::cout << "\nTest 4: Private message" << std::endl;
        sendCommand("PRIVMSG #channel :Hello, World!");
        receiveResponse();
        
        sleep(10);
        
        // Test 5: Multiple commands at once (buffer test)
        std::cout << "\nTest 5: Multiple commands" << std::endl;
        sendCommand("NICK user2");
        sendCommand("PING :test456");
        receiveResponse();
        receiveResponse();
        
        sleep(10);
        
        // Test 6: QUIT
        std::cout << "\nTest 6: Disconnecting" << std::endl;
        sendCommand("QUIT :Goodbye!");
        receiveResponse();
        
        std::cout << "\n=== Tests Complete ===\n" << std::endl;
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