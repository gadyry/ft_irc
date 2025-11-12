```
┌────────────────────────┐
│        Server          │
├────────────────────────┤
│ - port : int           │
│ - password : string    │
│ - listener_fd : int    │
│ - poll_fds : vector<pollfd> │
│ - clients : map<int, Client> │
│ - channels : map<string, Channel> │
├────────────────────────┤
│ + start()              │
│ + acceptClient()       │
│ + receiveMessage(fd)   │
│ + handleCommand(fd, msg)│
│ + sendReply(fd, msg)   │
│ + removeClient(fd)     │
└───────────┬────────────┘
            │ 1..*
            │ owns
            ▼
┌────────────────────────┐
│        Client          │
├────────────────────────┤
│ - fd : int             │
│ - nickname : string    │
│ - username : string    │
│ - realname : string    │
│ - authenticated : bool │
│ - joinedChannels : set<string> │
├────────────────────────┤
│ + send(msg)            │
│ + receive()            │
│ + isRegistered()       │
│ + joinChannel(name)    │
│ + leaveChannel(name)   │
└───────────┬────────────┘
            │ belongs to many
            ▼
┌────────────────────────┐
│        Channel         │
├────────────────────────┤
│ - name : string        │
│ - topic : string       │
│ - users : set<Client*> │
│ - operators : set<Client*> │
├────────────────────────┤
│ + addUser(Client*)     │
│ + removeUser(Client*)  │
│ + broadcast(msg, sender)│
│ + setTopic(topic)      │
└────────────────────────┘

┌────────────────────────┐
│     CommandHandler     │
├────────────────────────┤
│ - server : Server*     │
├────────────────────────┤
│ + execute(cmd, client, args) │
│ + cmdPASS(...)         │
│ + cmdNICK(...)         │
│ + cmdUSER(...)         │
│ + cmdJOIN(...)         │
│ + cmdPART(...)         │
│ + cmdPRIVMSG(...)      │
│ + cmdPING(...)         │
│ + cmdQUIT(...)         │
└────────────────────────┘
```

```
[Client fd=4] ---> "JOIN #general"
       ↓
Server receives line
       ↓
Server → CommandHandler::cmdJOIN()
       ↓
CommandHandler calls server.getChannel("#general").addUser(client)
       ↓
Channel broadcasts "david has joined #general"
       ↓
Server sends the message to all channel members
```