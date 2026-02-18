*This project has been created as part of the 42 curriculum by ael-gady, adechaji.*

# ft_irc

## Description

**ft_irc** is an IRC (Internet Relay Chat) server written in C++98, compliant with RFC 1459. The goal of the project is to build a fully functional IRC server from scratch using low-level Unix socket programming, handling multiple clients simultaneously through a single-threaded, event-driven architecture based on `poll()`.

The server accepts connections from standard IRC clients (such as nc, limeChat, ...), authenticates users with a password, and supports real-time channel-based and private messaging.

### Key Features

- **TCP socket server** using `socket()`, `bind()`, `listen()`, `accept()`
- **Non-blocking I/O** with `fcntl()` and `O_NONBLOCK`
- **Event-driven multiplexing** via `poll()` — single thread handles all clients
- **RFC 1459 compliant** IRC numeric replies
- **User authentication** with password, nickname, and username registration (`PASS`, `NICK`, `USER`)
- **Channel management** — create, join, leave, and manage channels
- **Channel operator privileges** — kick, invite, set topic, configure channel modes
- **Supported IRC commands**: `PASS`, `NICK`, `USER`, `JOIN`, `PRIVMSG`, `KICK`, `INVITE`, `TOPIC`, `MODE`, `QUIT`, `PING/PONG`
- **Channel modes**: invite-only (`+i`), topic restriction (`+t`), channel key (`+k`), user limit (`+l`), operator privilege (`+o`)
- **Bonus: MovieBot** — an autonomous IRC bot that connects to the server and responds to commands like `!quote`, `!help`, `!suggest`, and `!info`, pulling data from CSV-based movie and quote databases

---

## Architecture Overview

```
             ┌──────────────┐
             │    Server    │
             │──────────────│
             │ - serv_fd    │
             │ - port       │
             │ - password   │
             │              │
             │ clients map  │──────┐
             │ channels map │───┐  │
             └──────────────┘   │  │
                                │  │
                                ▼  ▼
                       ┌──────────┐   ┌──────────┐
                       │  Client  │   │  Client  │
                       │──────────│   │──────────│
                       │ fd       │   │ fd       │
                       │ nick     │   │ nick     │
                       │ user     │   │ user     │
                       │ buffer   │   │ buffer   │
                       └──────────┘   └──────────┘
                             ▲             ▲
                             │ references  │
                      ┌──────────────────────────┐
                      │         Channel          │
                      │──────────────────────────│
                      │ name (#general)          │
                      │ topic                    │
                      │ members: Client*         │
                      │ operators: Client*       │
                      └──────────────────────────┘
```

The server uses three core classes:

| Class     | Responsibility                                                            |
|-----------|---------------------------------------------------------------------------|
| `Server`  | Manages the listening socket, `poll()` loop, client/channel maps          |
| `Client`  | Represents a connected user (fd, nickname, username, auth state, buffer)  |
| `Channel` | Represents an IRC channel (members, operators, modes, topic)              |
-----------------------------------------------------------------------------------------
---

## Instructions

### Requirements

- A C++98 compatible compiler (e.g., `c++`)
- A POSIX-compliant system (Linux / macOS)
- `make`

### Compilation

```bash
# Build the IRC server
make

# Build the bonus MovieBot
make bonus
```

This produces two binaries:
- `ircserv` — the IRC server
- `MoviesBot` — the bonus IRC bot

### Running the Server

```bash
./ircserv <port> <password>
```

- **port**: TCP port number (1024–65535)
- **password**: connection password (no spaces)

Example:

```bash
./ircserv 6667 secret123
```

### Connecting with an IRC Client

Using **nc** (netcat) for testing:

```bash
nc 127.0.0.1 6667
PASS secret123
NICK leo
USER leo 0 * :leo messi
JOIN #general
PRIVMSG #general :Hello everyone!
```

### Running the Bonus MovieBot

After starting the server, launch the bot in a separate terminal:

```bash
./MoviesBot <hostname> <port> <password>
```

Example:

```bash
./MoviesBot 127.0.0.1 6667 secret123
```

Then send private messages to MovieBot from any connected client:

```
PRIVMSG MovieBot :!help
PRIVMSG MovieBot :!quote
PRIVMSG MovieBot :!suggest
PRIVMSG MovieBot :!info <movie title>
```

### Cleanup

```bash
make clean    # Remove object files
make fclean   # Remove object files and binaries
make re       # Full rebuild
```

---

## Supported IRC Commands

| Command   | Description                          | Example                             |
|-----------|--------------------------------------|-------------------------------------|
| `PASS`    | Set connection password              | `PASS secret123`                    |
| `NICK`    | Set or change nickname               | `NICK john`                         |
| `USER`    | Set username                         | `USER john 0 * :John Doe`           |
| `JOIN`    | Join a channel                       | `JOIN #general`                     |
| `PRIVMSG` | Send a message (channel or private)  | `PRIVMSG #general :Hello!`          |
| `KICK`    | Kick a user (operator only)          | `KICK #general john :Spam`          |
| `MODE`    | Set channel mode (operator only)     | `MODE #general +i`                  |
| `TOPIC`   | Set/view channel topic (operator)    | `TOPIC #general :New topic`         |
| `INVITE`  | Invite a user to channel (operator)  | `INVITE john #private`              |
| `QUIT`    | Disconnect from server               | `QUIT :Goodbye!`                    |

### Channel Modes

| Mode | Flag | Description                                   |
|------|------|-----------------------------------------------|
| Invite-only        | `+i` | Only invited users can join     |
| Topic restriction  | `+t` | Only operators can change topic |
| Channel key        | `+k` | Requires a password to join     |
| User limit         | `+l` | Limits the number of members    |
| Operator privilege  | `+o` | Grant/revoke operator status   |

---

## Server Flow

```
START SERVER

  create listening socket
  bind it to port
  mark it as listening

  WHILE server is running:

      wait until at least one socket has an event  (poll)

      FOR each socket that has an event:

          IF socket == listening_socket:
              new_client_fd = accept connection
              create client object
              start watching this fd

          ELSE:
              bytes = read from client_fd

              IF bytes == 0:
                  client disconnected → remove & close

              ELSE:
                  store data in client buffer
                  if complete message exists:
                      parse and execute command

  END WHILE
```

---

## Sequence Diagrams

### 1. Client Connection and Authentication Flow

```
Client                  IRC Server              Channel
  |                          |                      |
  |----(1) Connect TCP------>|                      |
  |                          |                      |
  |<---(2) Connection ACK----|                      |
  |                          |                      |
  |----(3) PASS password---->|                      |
  |                          |                      |
  |----(4) NICK nickname---->|                      |
  |                          |                      |
  |                          |--[Validate Nick]     |
  |                          |                      |
  |----(5) USER username---->|                      |
  |                          |                      |
  |                          |--[Authenticate]      |
  |                          |                      |
  |<---(6) RPL_WELCOME-------|                      |
  |                          |                      |
  |<---(7) RPL_MOTD----------|                      |
  |                          |                      |
```

### 2. Join Channel Flow

```
Client                  IRC Server              Channel
  |                          |                      |
  |----(1) JOIN #channel---->|                      |
  |                          |                      |
  |                          |--[Check if exists]-->|
  |                          |                      |
  |                          |<--[Channel found]----|
  |                          |                      |
  |                          |--[Check password]--->|
  |                          |                      |
  |                          |--[Check modes]------>|
  |                          |                      |
  |                          |--[Add user]--------->|
  |                          |                      |
  |<---(2) RPL_TOPIC---------|                      |
  |                          |                      |
  |<---(3) RPL_NAMREPLY------|                      |
  |                          |                      |
  |<---(4) RPL_ENDOFNAMES----|                      |
  |                          |                      |
  |                          |--[Broadcast JOIN]--->|
  |                          |                      |
  |<---(5) :user JOIN #ch----|                      |
  |                          |                      |
```

### 3. Send Message to Channel Flow

```
Client A                IRC Server              Channel               Client B
  |                          |                      |                      |
  |-(1) PRIVMSG #chan :msg-->|                      |                      |
  |                          |                      |                      |
  |                          |--[Validate user]---->|                      |
  |                          |                      |                      |
  |                          |--[Check perms]------>|                      |
  |                          |                      |                      |
  |                          |--[Get members]------>|                      |
  |                          |                      |                      |
  |                          |<-[Member list]-------|                      |
  |                          |                      |                      |
  |                          |--[Broadcast msg]--------------------------->|
  |                          |                      |                      |
  |                          |                      |   <-(2) :A PRIVMSG...|
  |                          |                      |                      |
```

### 4. Private Message Flow

```
Client A                IRC Server              Client B
  |                          |                      |
  |-(1) PRIVMSG nick :msg--->|                      |
  |                          |                      |
  |                          |--[Find user]         |
  |                          |                      |
  |                          |--[Validate sender]   |
  |                          |                      |
  |                          |-(2) :A PRIVMSG...--->|
  |                          |                      |
  |<---(3) Message sent------|                      |
  |                          |                      |
```

### 5. Operator Kick User Flow

```
Operator              IRC Server              Channel               Kicked User
  |                          |                      |                      |
  |-(1) KICK #ch user :msg-->|                      |                      |
  |                          |                      |                      |
  |                          |--[Check op status]-->|                      |
  |                          |                      |                      |
  |                          |<-[Operator: YES]-----|                      |
  |                          |                      |                      |
  |                          |--[Remove user]------>|                      |
  |                          |                      |                      |
  |                          |--[Broadcast KICK]-------------------------->|
  |                          |                      |                      |
  |<---(2) :op KICK #ch user-|                      | <-(3) :op KICK...    |
  |                          |                      |                      |
  |                          |                      |   (User disconnected)|
  |                          |                      |                      |
```

### 6. Set Channel Mode Flow

```
Operator              IRC Server              Channel
  |                          |                      |
  |-(1) MODE #ch +i--------->|                      |
  |                          |                      |
  |                          |--[Check op perms]--->|
  |                          |                      |
  |                          |<-[Operator: YES]-----|
  |                          |                      |
  |                          |--[Set mode +i]------>|
  |                          |                      |
  |                          |<-[Mode updated]------|
  |                          |                      |
  |<---(2) MODE #ch +i-------|                      |
  |                          |                      |
  |                          |--[Broadcast change]->|
  |                          |                      |
```

### 7. Disconnect Flow

```
Client                  IRC Server              Channel
  |                          |                      |
  |----(1) QUIT :reason----->|                      |
  |                          |                      |
  |                          |--[Get channels]----->|
  |                          |                      |
  |                          |--[Remove from all]-->|
  |                          |                      |
  |                          |--[Broadcast QUIT]--->|
  |                          |                      |
  |<---(2) ERROR: Closing----|                      |
  |                          |                      |
  |----[Close connection]----|                      |
  |                          |                      |
```

### 8. Invite User to Channel Flow

```
Operator              IRC Server        Invited User        Channel
  |                          |                 |                 |
  |-(1) INVITE user #ch----->|                 |                 |
  |                          |                 |                 |
  |                          |-[Check op]----->|                 |
  |                          |                 |                 |
  |                          |-[Check mode]------------------->  |
  |                          |                 |                 |
  |                          |<-[Invite-only]--------------------|
  |                          |                 |                 |
  |                          |-(2) :op INVITE->|                 |
  |                          |                 |                 |
  |<---(3) RPL_INVITING------|                 |                 |
  |                          |                 |                 |
  |                          |                 |-(4) JOIN #ch--->|
  |                          |                 |                 |
```

---

## MovieBot Sequence Diagrams

### 1. Bot Startup & Connection

```
┌─────┐        ┌────────┐        ┌────────┐
│ Bot │        │ Server │        │ User   │
└──┬──┘        └───┬────┘        └───┬────┘
   │               │                 │
   │ create()      │                 │
   ├──────────────>│                 │
   │               │                 │
   │ socket()      │                 │
   │───────┐       │                 │
   │       │       │                 │
   │<──────┘       │                 │
   │               │                 │
   │ connect()     │                 │
   │══════════════>│                 │
   │               │                 │
   │ [TCP connection established]    │
   │<══════════════│                 │
   │               │                 │
```

### 2. Bot Authentication Flow

```
┌─────┐        ┌────────┐
│ Bot │        │ Server │
└──┬──┘        └───┬────┘
   │               │
   │ PASS password │
   │──────────────>│
   │               │
   │               │ [validate password]
   │               │───────┐
   │               │       │
   │               │<──────┘
   │               │
   │ NICK MovieBot │
   │──────────────>│
   │               │
   │               │ [check nick available]
   │               │───────┐
   │               │       │
   │               │<──────┘
   │               │
   │ USER moviebot 0 * :Bot
   │──────────────>│
   │               │
   │               │ [check registration complete]
   │               │───────┐
   │               │       │
   │               │<──────┘
   │               │
   │ 001 Welcome   │
   │<──────────────│
   │               │
   │ [Bot is now registered!]
   │               │
```

### 3. Message Flow (User → Bot)

```
┌─────┐    ┌────────┐    ┌─────────────┐    ┌──────────┐
│User │    │ Server │    │     Bot     │    │ Commands │
└──┬──┘    └───┬────┘    └──────┬──────┘    └────┬─────┘
   │           │                 │                │
   │ PRIVMSG MovieBot :!quote    │                │
   │──────────>│                 │                │
   │           │                 │                │
   │           │ :alice!~a@host PRIVMSG MovieBot :!quote
   │           │────────────────>│                │
   │           │                 │                │
   │           │                 │ readData()     │
   │           │                 │───────┐        │
   │           │                 │       │        │
   │           │                 │<──────┘        │
   │           │                 │                │
   │           │                 │ parseLine()    │
   │           │                 │───────┐        │
   │           │                 │       │        │
   │           │                 │<──────┘        │
   │           │                 │                │
   │           │                 │ extract: !quote│
   │           │                 │───────┐        │
   │           │                 │       │        │
   │           │                 │<──────┘        │
   │           │                 │                │
   │           │                 │ handleQuote()  │
   │           │                 │───────────────>│
   │           │                 │                │
   │           │                 │   getRandomQuote()
   │           │                 │                │───┐
   │           │                 │                │   │
   │           │                 │<───────────────│<──┘
   │           │                 │                │
   │           │ PRIVMSG alice :"May the Force..."│
   │           │<────────────────│                │
   │           │                 │                │
   │ :MovieBot!~bot@host PRIVMSG alice :"May..."  │
   │<──────────│                 │                │
   │           │                 │                │
   │ [User sees quote!]          │                │
   │           │                 │                │
```

### 5. Complete Flow Example

```
TIME    USER                SERVER              BOT                 COMMANDS
│       │                   │                   │                   │
│       │                   │                   │ connect()         │
│       │                   │<══════════════════│                   │
│       │                   │                   │                   │
│       │                   │                   │ PASS secret       │
│       │                   │<──────────────────│                   │
│       │                   │                   │                   │
│       │                   │                   │ NICK MovieBot     │
│       │                   │<──────────────────│                   │
│       │                   │                   │                   │
│       │                   │                   │ USER moviebot...  │
│       │                   │<──────────────────│                   │
│       │                   │                   │                   │
│       │                   │ 001 Welcome       │                   │
│       │                   │──────────────────>│                   │
│       │                   │                   │                   │
│       │ PASS secret       │                   │                   │
│       │──────────────────>│                   │                   │
│       │                   │                   │                   │
│       │ NICK alice        │                   │                   │
│       │──────────────────>│                   │                   │
│       │                   │                   │                   │
│       │ USER alice...     │                   │                   │
│       │──────────────────>│                   │                   │
│       │                   │                   │                   │
│       │ 001 Welcome       │                   │                   │
│       │<──────────────────│                   │                   │
│       │                   │                   │                   │
│       │ PRIVMSG MovieBot :!quote              │                   │
│       │──────────────────>│                   │                   │
│       │                   │                   │                   │
│       │                   │ :alice...PRIVMSG MovieBot :!quote     │
│       │                   │──────────────────>│                   │
│       │                   │                   │                   │
│       │                   │                   │ parse: !quote     │
│       │                   │                   │──────────────────>│
│       │                   │                   │                   │
│       │                   │                   │ getRandomQuote()  │
│       │                   │                   │<──────────────────│
│       │                   │                   │                   │
│       │                   │ PRIVMSG alice :"May the Force..."     │
│       │                   │<──────────────────│                   │
│       │                   │                   │                   │
│       │ :MovieBot...PRIVMSG alice :"May..."   │                   │
│       │<──────────────────│                   │                   │
│       │                   │                   │                   │
│       │ [Sees quote!]     │                   │                   │
│       │                   │                   │                   │
▼       ▼                   ▼                   ▼                   ▼
```

---

## Technical Choices

- **C++98 standard** — as required by the 42 subject
- **`poll()` for I/O multiplexing** — chosen over `select()` for cleaner API and no `FD_SETSIZE` limitation; `epoll` was avoided to keep the project portable and within subject constraints
- **Non-blocking sockets via `fcntl()`** — ensures the server never freezes waiting on a single client
- **`std::map` for client/channel lookups** — O(log n) access by fd or channel name
- **Raw pointers with manual memory management** — no smart pointers (C++98 constraint)
- **CSV-based database for the bot** — lightweight, no external dependencies

---

## Resources

### Documentation & References

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812 — IRC Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [The Linux `poll()` man page](https://man7.org/linux/man-pages/man2/poll.2.html)
- [The Linux `socket()` man page](https://man7.org/linux/man-pages/man2/socket.2.html)
- [The Linux `fcntl()` man page](https://man7.org/linux/man-pages/man2/fcntl.2.html)
- [Modern IRC Documentation](https://modern.ircdocs.horse/)
- [The linux programming Interface](https://broman.dev/download/The%20Linux%20Programming%20Interface.pdf)

### AI Usage

AI tools (ChatGP, DeepSeek) were used in the following areas:

- **Research and learning**: AI was used to deepen our understanding of the concepts covered in *The Linux Programming Interface* by Michael Kerrisk — particularly around Unix socket internals, file descriptor management, the TCP 3-way handshake, `poll()`-based event-driven architecture, and non-blocking I/O with `fcntl()`. The deep-dive notes in the project's README.md were produced with AI assistance to clarify and expand on what we learned from the book.
- **Documentation**: assisting with the creation of technical reference material (sequence diagrams, class diagrams, use-case diagrams found in ClassDiag.md)
- **Bot database design**: AI helped evaluate the best approach for the MovieBot's data storage — comparing options like SQLite, JSON, and flat CSV files. We chose CSV as the most lightweight and dependency-free solution fitting the C++98 constraint, and AI assisted in structuring the data model (movies and quotes linked by ID).
- **Design thinking**: AI was used as a sounding board to think outside the box — exploring different approaches, questioning assumptions, and refining our architectural choices before writing code.
- **Critical thinking and self-assessment**: We regularly asked AI to quiz us with targeted questions on the topics we were studying (sockets, TCP, `poll()`, IRC protocol, etc.) to test our understanding, identify gaps in our knowledge, and develop a stronger critical sense rather than passively consuming information.
- **Code was written by the team members** — AI was not used to generate the code implementation, only for formatting print messages and generating the IRC numeric reply header macros
