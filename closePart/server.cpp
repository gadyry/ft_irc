#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>

#define BUFFER_SIZE 512

class SimpleIRCServer {
private:
    int serv_fd;
    std::vector<pollfd> fds_sentinels;
    std::map<int, std::string> client_buffers; // partial messages per client
    
public:
    SimpleIRCServer(int port) {
        // Create socket
        serv_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (serv_fd == -1)
            throw std::runtime_error("socket() failed");
        
        // Set socket options (reuse address)
        int opt = 1;
        setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        // Bind
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        
        if (bind(serv_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
            throw std::runtime_error("bind() failed");
        
        // Listen
        if (listen(serv_fd, 10) == -1)
            throw std::runtime_error("listen() failed");
        
        // Add server fd to poll
        pollfd server_poll;
        server_poll.fd = serv_fd;
        server_poll.events = POLLIN;
        server_poll.revents = 0;
        fds_sentinels.push_back(server_poll);
        
        std::cout << "Server listening on port " << port << std::endl;
    }
    
    void addClient() {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(serv_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            std::cerr << "accept() failed" << std::endl;
            return;
        }
        
        pollfd client_poll;
        client_poll.fd = client_fd;
        client_poll.events = POLLIN;
        client_poll.revents = 0;
        fds_sentinels.push_back(client_poll);
        
        client_buffers[client_fd] = ""; // Initialize buffer for this client
        
        std::cout << "New client connected: fd=" << client_fd << std::endl;
        
        // Send welcome message
        std::string welcome = ":server 001 * :Welcome to IRC!\r\n";
        send(client_fd, welcome.c_str(), welcome.length(), 0);
    }
    
    void receiveData(int client_fd) {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        
        ssize_t bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes <= 0) {
            // Client disconnected or error
            if (bytes == 0)
                std::cout << "Client disconnected: fd=" << client_fd << std::endl;
            else
                std::cerr << "recv() error on fd=" << client_fd << std::endl;
            
            removeClient(client_fd);
            return;
        }
        
        // Add received data to client's buffer
        client_buffers[client_fd] += std::string(buffer, bytes);
        
        // Process complete commands (IRC commands end with \r\n)
        processCommands(client_fd);
    }
    
    void processCommands(int client_fd) {
        std::string& buffer = client_buffers[client_fd];
        size_t pos;

        // Find complete commands (ending with \r\n)
        while ((pos = buffer.find("\r\n")) != std::string::npos) {
            std::string command = buffer.substr(0, pos);
            buffer.erase(0, pos + 2); // Remove processed command
            
            if (!command.empty()) {
                std::cout << "Client " << client_fd << " sent: " << command << std::endl;
                handleCommand(client_fd, command);
            }
        }
    }
    
    void handleCommand(int client_fd, const std::string& cmd) {
        // Simple command parsing
        if (cmd.find("NICK") == 0) {
            std::string response = ":server 001 * :Nickname accepted\r\n";
            send(client_fd, response.c_str(), response.length(), 0);
        }
        else if (cmd.find("USER") == 0) {
            std::string response = ":server 002 * :User registered\r\n";
            send(client_fd, response.c_str(), response.length(), 0);
        }
        else if (cmd.find("PING") == 0) {
            std::string response = "PONG " + cmd.substr(5) + "\r\n";
            send(client_fd, response.c_str(), response.length(), 0);
        }
        else if (cmd.find("PRIVMSG") == 0) {
            std::cout << "Message: " << cmd << std::endl;
            // Echo back
            std::string response = ":server PRIVMSG :Message received\r\n";
            send(client_fd, response.c_str(), response.length(), 0);
        }
        else if (cmd.find("QUIT") == 0) {
            std::string response = ":server ERROR :Closing connection\r\n";
            send(client_fd, response.c_str(), response.length(), 0);
            removeClient(client_fd);
        }
    }
    
    void removeClient(int client_fd) {
        close(client_fd);
        client_buffers.erase(client_fd);
        
        for (size_t i = 0; i < fds_sentinels.size(); i++) {
            if (fds_sentinels[i].fd == client_fd) {
                fds_sentinels.erase(fds_sentinels.begin() + i);
                break;
            }
        }
    }
    
    void run() {
        while (true) {
            if (poll(&fds_sentinels[0], fds_sentinels.size(), -1) == -1)
                throw std::runtime_error("poll() failed");
            
            for (size_t i = 0; i < fds_sentinels.size(); i++) {
                if (fds_sentinels[i].revents & POLLIN) {
                    if (fds_sentinels[i].fd == serv_fd)
                        addClient();
                    else
                        receiveData(fds_sentinels[i].fd);
                }
            }
        }
    }

        ~SimpleIRCServer()
        {
            close(serv_fd);
        }
};

int main()
{
    try
    {
        SimpleIRCServer server(6667);
        server.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}