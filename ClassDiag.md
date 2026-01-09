```

+----------------+
|    Server      |
+----------------+
| int             port
| std::string     password
| int             server_fd
| std::vector<pollfd> poll_fds
| std::map<int, Client*> clients   // fd → Client
| std::map<std::string, Channel*> channels
+----------------+
| void start()
| void initSocket()
| void eventLoop()
| void acceptClient()
| void disconnectClient(int fd)
| void handleMessage(int fd)
| void sendToClient(int fd, const std::string &msg)
+----------------+


+----------------+
|    Client      |
+----------------+
| int              fd
| std::string      nick
| std::string      user
| std::string      host
| bool             registered
| std::string      buffer        // accumulate partial lines
+----------------+
| + void send(const std::string& msg)
+----------------+

```