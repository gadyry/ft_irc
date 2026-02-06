```
                ┌──────────────┐
                │    Server    │
                │──────────────│
                │ - serv_fd    │
                │ - port       │
                │ - password   │
                │              │
                │ owns         │
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
                                │ (NO OWNERS) │
                         ┌──────────────────────────┐
                         │         Channel          │
                         │──────────────────────────│
                         │ name (#general)          │
                         │ topic                    │
                         │ members: Client*         │
                         │ operators: Client*       │
                         └──────────────────────────┘

```

---

# FT_IRC Use Case Diagrams

## Diagram 1: Vertical Layout

```
═══════════════════════════════════════════════════════════════════════════════════
                           FT_IRC USE CASE DIAGRAM
═══════════════════════════════════════════════════════════════════════════════════


     Client                    ╔════════════════════════════════════════╗
       ○                       ║     IRC Server System                  ║
      /|\   ───«uses»──────>   ║                                        ║
      / \                      ║    ┌─────────────────┐                 ║
       │                       ║    │  Connect to     │                 ║
       │   ───«uses»──────>    ║    │    Server       │                 ║
       │                       ║    └─────────────────┘                 ║
       │                       ║                                        ║
       │   ───«uses»──────>    ║    ┌─────────────────┐                 ║
       │                       ║    │  Authenticate   │                 ║
       │                       ║    └─────────────────┘                 ║
       │                       ║                                        ║
       │   ───«uses»──────>    ║    ┌─────────────────┐                 ║
       │                       ║    │  Join Channel   │                 ║
       │                       ║    └─────────────────┘                 ║
       │                       ║                                        ║
       │   ───«uses»──────>    ║    ┌─────────────────┐                 ║
       │                       ║    │  Send Message   │                 ║
       │                       ║    └─────────────────┘                 ║
       │                       ║                                        ║
       │   ───«uses»──────>    ║    ┌─────────────────┐                 ║
       │                       ║    │ Private Message │                 ║
       │                       ║    └─────────────────┘                 ║
       │                       ║                                        ║
       │   ───«uses»──────>    ║    ┌─────────────────┐                 ║
       │                       ║    │ Leave Channel   │                 ║
       │                       ║    └─────────────────┘                 ║
       │                       ║                                        ║
       │   ───«uses»──────>    ║    ┌─────────────────┐                 ║
       │                       ║    │   Disconnect    │                 ║
       │                       ║    └─────────────────┘                 ║
       │                       ║                                        ║
       │                       ║                                        ║
       │                       ║    ┌─────────────────┐                 ║
       │                       ║    │   Kick User     │  <───«uses»────┐ ║
       │                       ║    └─────────────────┘                │ ║
       │                       ║                                       │ ║
       │                       ║    ┌─────────────────┐                │ ║
       │                       ║    │   Ban User      │  <───«uses»────┤ ║
       │                       ║    └─────────────────┘                │ ║
       │                       ║                                       │ ║
       │                       ║    ┌─────────────────┐                │ ║
       │                       ║    │ Set Channel     │  <───«uses»────┤ ║
       │                       ║    │     Mode        │                │ ║
       │                       ║    └─────────────────┘                │ ║
       │                       ║                                       │ ║
       │                       ║    ┌─────────────────┐                │ ║
       │                       ║    │ Manage Topic    │  <───«uses»────┤ ║
       │                       ║    └─────────────────┘                │ ║
       │                       ║                                       │ ║
       │                       ║    ┌─────────────────┐                │ ║
       │   «inherits»          ║    │  Invite User    │  <───«uses»────┘ ║
       └──────────────────────>║                                        ║
                               ║                                        ║
    Operator                   ╚════════════════════════════════════════╝
       ○
      /|\
      / \


═══════════════════════════════════════════════════════════════════════════════════
```

---

## Diagram 2: Horizontal Compact Layout

```
╔═══════════════════════════════════════════════════════════════════════════════════╗
║                          FT_IRC USE CASE DIAGRAM                                  ║
╚═══════════════════════════════════════════════════════════════════════════════════╝

                        ╔═══════════════════════════════════════╗
                        ║      IRC Server System                ║
   Client               ║                                       ║         Operator
     ○                  ║  (Connect)  (Join Ch)  (Send Msg)    ║             ○
    /|\                 ║     ○──       ○──       ○──           ║           /|\
    / \                 ║    / \       / \       / \            ║           / \
     │                  ║                                       ║            │
     ├──«uses»────────> ║  (Auth)   (Private)  (Leave)         ║ <──«uses»──┤
     ├──«uses»────────> ║    ○──      ○──       ○──            ║ <──«uses»──┤
     ├──«uses»────────> ║   / \      / \       / \             ║ <──«uses»──┤
     ├──«uses»────────> ║                                      ║ <──«uses»──┤
     ├──«uses»────────> ║  (Disconnect)                        ║ <──«uses»──┤
     ├──«uses»────────> ║     ○──                              ║            │
     └──«uses»────────> ║    / \                               ║            │
          │             ║                                      ║            │
          │             ║  [OPERATOR ONLY]                    ║            │
          │             ║  (Kick)  (Ban)  (Mode)  (Topic)     ║            │
          │             ║   ○──     ○──    ○──     ○──        ║            │
          └─«inherits»──║  / \     / \    / \     / \         ║────────────┘
                        ║                                     ║
                        ║  (Invite)                           ║
                        ║    ○──                              ║
                        ║   / \                               ║
                        ╚═════════════════════════════════════╝
```

---

## Legend

### Symbols

- **Actor (Stick Figure)**
  ```
    ○
   /|\
   / \
  ```

- **Use Case (Ellipse/Oval)**
  ```
  ┌─────────────┐
  │  Use Case   │
  └─────────────┘
  
  or
  
   ○──
  / \
  ```

- **System Boundary**
  ```
  ╔═════════════╗
  ║   System    ║
  ╚═════════════╝
  ```

- **Association (Uses Relationship)**
  ```
  ────────>
  ```

- **Generalization (Inheritance)**
  ```
  ─ ─ ─ ─ ─>
  ```

### Actors

1. **Client** - Regular user with basic IRC functionalities
2. **Operator** - Channel administrator (inherits from Client)

### Use Cases

#### Client Use Cases:
- Connect to Server
- Authenticate
- Join Channel
- Send Message
- Private Message
- Leave Channel
- Disconnect

#### Operator Use Cases (Additional):
- Kick User
- Ban User
- Set Channel Mode
- Manage Topic
- Invite User

### Relationships

- **Uses (Association)**: Connects actors to use cases they can perform
- **Inherits (Generalization)**: Operator inherits all Client capabilities plus additional administrative functions

---

## Notes

These diagrams represent the **ft_irc** project use case model, showing:
- Two types of actors (Client and Operator)
- Twelve use cases total (7 for Client, 5 additional for Operator)
- The inheritance relationship where Operator extends Client functionality
- All interactions contained within the IRC Server System boundary

---

# FT_IRC Sequence Diagrams

## 1. Client Connection and Authentication Flow

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

---

## 2. Join Channel Flow

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

---

## 3. Send Message to Channel Flow

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
  |                          |--[Broadcast msg]-------------------->------>|
  |                          |                      |                      |
  |                          |                      |   <-(2) :A PRIVMSG...|
  |                          |                      |                      |
```

---

## 4. Private Message Flow

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

---

## 5. Operator Kick User Flow

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
  |                          |--[Broadcast KICK]---------------->--------->|
  |                          |                      |                      |
  |<---(2) :op KICK #ch user-|                      | <-(3) :op KICK...    |
  |                          |                      |                      |
  |                          |                      |   (User disconnected)|
  |                          |                      |                      |
```

---

## 6. Set Channel Mode Flow

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

---

## 7. Disconnect Flow

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

---

## 8. Invite User to Channel Flow

```
Operator              IRC Server        Invited User        Channel
  |                          |                 |                 |
  |-(1) INVITE user #ch----->|                 |                 |
  |                          |                 |                 |
  |                          |-[Check op]----->|                 |
  |                          |                 |                 |
  |                          |-[Check mode]------------------->  |
  |                          |                 |                 |
  |                          |<-[Invite-only]------------------|  |
  |                          |                 |                 |
  |                          |-(2) :op INVITE->|                 |
  |                          |                 |                 |
  |<---(3) RPL_INVITING------|                 |                 |
  |                          |                 |                 |
  |                          |                 |-(4) JOIN #ch--->|
  |                          |                 |                 |
```

---

## Legend

### Symbols Used

```
|           = Lifeline (object/actor existence over time)
--->        = Synchronous message (request)
<---        = Return message (response)
--[text]    = Internal processing/action
:           = Message content separator
RPL_XXX     = Server reply code
#channel    = Channel name
```

### Message Types

- **Connection Messages**: `PASS`, `NICK`, `USER`
- **Channel Operations**: `JOIN`, `PART`, `KICK`, `MODE`, `TOPIC`, `INVITE`
- **Communication**: `PRIVMSG`, `NOTICE`
- **Disconnection**: `QUIT`
- **Server Replies**: `RPL_WELCOME`, `RPL_MOTD`, `RPL_TOPIC`, etc.

### Actors

1. **Client** - Regular IRC user
2. **Operator** - Channel administrator with special privileges
3. **IRC Server** - Central server managing connections and channels
4. **Channel** - Chat room object managing members and settings

---

## Notes

### Authentication Process
1. Client must send `PASS` before `NICK` and `USER`
2. Server validates password before accepting connection
3. Nickname must be unique on the server
4. Welcome messages sent after successful authentication

### Channel Operations
- Users must be in a channel to send messages to it
- Operators have additional privileges (KICK, BAN, MODE, TOPIC, INVITE)
- Channel modes control behavior (invite-only, topic-lock, password, etc.)

### Error Handling
- Invalid commands return error codes
- Permission denied for unauthorized operations
- Non-existent users/channels return appropriate errors

---
## Common IRC Commands (ft_irc)

| Command | Description | Example |
|---------|-------------|---------|
| `PASS` | Set connection password | `PASS secret123` |
| `NICK` | Set/change nickname | `NICK john` |
| `USER` | Set username | `USER john 0 * :John Doe` |
| `JOIN` | Join a channel | `JOIN #general` |
| `PART` | Leave a channel | `PART #general :Goodbye` |
| `PRIVMSG` | Send message | `PRIVMSG #general :Hello!` |
| `KICK` | Kick user (op only) | `KICK #general john :Spam` |
| `MODE` | Set channel mode (op) | `MODE #general +i` |
| `TOPIC` | Set channel topic (op) | `TOPIC #general :New topic` |
| `INVITE` | Invite user (op) | `INVITE john #private` |
| `QUIT` | Disconnect | `QUIT :Goodbye!` |
---

### CONCEPTION OF BOT PART :

# IRC Bot Sequence Diagram

## 1️⃣ Bot Startup & Connection
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
# 2️⃣ Authentication Flow
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
# 3️⃣ Message Flow (User → Bot)
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
   │           │ PRIVMSG alice :"May the Force..." │
   │           │<────────────────│                │
   │           │                 │                │
   │ :MovieBot!~bot@host PRIVMSG alice :"May..." │
   │<──────────│                 │                │
   │           │                 │                │
   │ [User sees quote!]          │                │
   │           │                 │                │
```
# 4️⃣ Detailed Message Processing
```
┌──────────────────────────────────────────────────┐
│           Bot::run() - Main Loop                 │
└──────────────────┬───────────────────────────────┘
                   │
                   ▼
        ┌──────────────────────┐
        │  recv() from socket  │
        └──────────┬───────────┘
                   │
                   ▼
        ┌──────────────────────┐
        │  Add to buffer       │
        └──────────┬───────────┘
                   │
                   ▼
        ┌──────────────────────┐
        │  Find "\r\n"?        │
        └──────────┬───────────┘
                   │
           ┌───────┴────────┐
           │                │
          Yes              No
           │                │
           ▼                ▼
    ┌─────────────┐   ┌──────────┐
    │Extract line │   │Continue  │
    └──────┬──────┘   │reading   │
           │          └──────────┘
           ▼
    ┌─────────────┐
    │parseLine()  │
    └──────┬──────┘
           │
           ▼
    ┌─────────────┐
    │Is PRIVMSG?  │
    └──────┬──────┘
           │
       ┌───┴───┐
      Yes     No
       │       │
       ▼       ▼
  ┌────────┐  ┌────────┐
  │Extract │  │Handle  │
  │message │  │PING/   │
  └────┬───┘  │other   │
       │      └────────┘
       ▼
  ┌────────┐
  │Starts  │
  │with !? │
  └────┬───┘
       │
   ┌───┴───┐
  Yes     No
   │       │
   ▼       ▼
┌──────┐  ┌──────┐
│Handle│  │Ignore│
│cmd   │  └──────┘
└──┬───┘
   │
   ▼
┌──────────────┐
│ !quote?      │
│ !help?       │
│ !game?       │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│Execute cmd   │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│sendPrivMsg() │
└──────────────┘
```
## **5️⃣ Complete Flow Example
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
