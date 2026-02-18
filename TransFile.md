
Read [](ft_irc/commands/PRIVMSG.cpp)

Here's the full picture of what happens under the hood when LimeChat does a "file transfer":

---

## Step-by-Step: DCC File Transfer Through Your Server

### Phase 1: The DCC Request (goes through YOUR server)

When **Alice** wants to send a file to **Bob** via LimeChat:

```
Alice's LimeChat constructs this PRIVMSG:
┌─────────────────────────────────────────────────────────────────┐
│ PRIVMSG Bob :\x01DCC SEND photo.jpg 3232235777 5000 48231\x01   │
│                     │         │          │      │      │        │
│                     │         │          │      │      └ file size (bytes)
│                     │         │          │      └ port Alice is listening on
│                     │         │          └ Alice's IP (as integer)
│                     │         └ filename                        |
│                     └ DCC subcommand                            |
└─────────────────────────────────────────────────────────────────┘
```

### Phase 2: Your Server's Role (what your code actually does)

In PRIVMSG.cpp, your server treats it as **just another PRIVMSG**:

```
1. tokens[0] = "PRIVMSG"
2. tokens[1] = "Bob"           ← target (not a channel, no '#')
3. tokens[2..] = ":\x01DCC SEND photo.jpg 3232235777 5000 48231\x01"
                   ↑
                   Your server doesn't parse this at all.
                   It's just a string. The \x01 bytes are just characters.
```

Your code reaches this part:

```cpp
// Line 65-67 of PRIVMSG.cpp
// receiver = Bob's Client*
if(send(receiver->getFdClient(), fullmsg.c_str(), fullmsg.length(), 0) == -1)
    throw std::runtime_error("send() failed");
```

**That's it.** Your server just forwards the raw bytes to Bob. It has zero knowledge of DCC, files, IPs, or ports.

### Phase 3: The Actual File Transfer (BYPASSES your server entirely)

```
       Phase 1 & 2: DCC Request          Phase 3: Actual Transfer     
       (through your server)              (direct, peer-to-peer)       
                                                                       
  ┌─────────┐    PRIVMSG DCC SEND    ┌──────────┐                     
  │  Alice   │ ──────────────────────►│  YOUR    │                     
  │ LimeChat │                        │  SERVER  │                     
  └─────────┘                         └──────────┘                     
       │                                   │                           
       │                                   │  forward PRIVMSG to Bob   
       │                                   │                           
       │                              ┌──────────┐                     
       │                              │   Bob    │                     
       │                              │ LimeChat │                     
       │                              └──────────┘                     
       │                                   │                           
       │  Bob's LimeChat parses the DCC    │                           
       │  message, extracts Alice's IP     │                           
       │  and port, then connects DIRECTLY │                           
       │                                   │                           
       │◄══════════════════════════════════╡                           
       │   TCP connection on port 5000     │                           
       │   (your server is NOT involved)   │                           
       │                                   │                           
       │══════ photo.jpg bytes ═══════════►│                           
       │   Direct transfer, peer-to-peer   │
       ─────────────────────────────────────               
```

### Summary

| Phase | What happens | Your server's role |
|-------|-------------|-------------------|
| **1. Request** | Alice sends `PRIVMSG Bob :DCC SEND ...` | Receives raw bytes |
| **2. Relay** | Your PRIVMSG handler forwards to Bob | Just calls `send()` — treats DCC as plain text |
| **3. Transfer** | Bob connects directly to Alice's IP:port | **None** — completely bypassed |

Your server is just a **dumb relay** for the DCC negotiation message. The `\x01` (CTCP delimiter) and everything inside it is opaque data that only the IRC clients understand. The actual file bytes never touch your server.

This is why LimeChat shows "file transfer" features — the **client** implements DCC, not the server. Your server just needs to deliver PRIVMSGs faithfully, which it does.
