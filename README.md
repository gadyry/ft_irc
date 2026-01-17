## FT_IRC

```cpp
/*
    ✔ How a socket is created inside the kernel
    ✔ What memory structures are allocated
    ✔ How ports are chosen
    ✔ How TCP performs the 3-Way Handshake
    ✔ How accept() creates a new socket
    ✔ How the kernel handles buffers, queues, backlog
    ✔ How data moves between kernel ↔ user space
*/
```
# Unix Sockets – Deep Dive for Low-Level Developers

This document breaks down the concepts from a classic Unix sockets explanation, but with deep technical detail suitable for anyone who wants to truly understand what happens under the hood.

---

## 🔥 1. "Everything in Unix is a file!"

In Unix, **all I/O operations use file descriptors**. A file descriptor (FD) is simply an integer that indexes into a table maintained by the kernel for each process.

This means:

* files → FD
* terminals → FD
* pipes → FD
* FIFOs → FD
* sockets → FD
* even `/dev/random`, `/dev/null` → FD

Unix abstracts I/O so that **one single interface (`read`, `write`) works for nearly everything**.

When you want to communicate with another process, even over the network, you still use a file descriptor.

---

## 🔥 2. Obtaining a File Descriptor for Network Communication

To create a network communication endpoint, you call:

```c
int fd = socket(AF_INET, SOCK_STREAM, 0);
```
Excellent question.
This parameter is often misunderstood, so let’s explain it **precisely, step by step**, with **no hand-waving**.

---

## The function again

```c
int socket(int domain, int type, int protocol);
```

You asked specifically about:

```c
int protocol
```

---

## 1️⃣ What the `protocol` parameter REALLY means

> The `protocol` parameter specifies **which concrete protocol implementation** to use **inside the selected domain and type**.

In other words:

* `domain` → *protocol family* (IPv4, IPv6, Unix, …)
* `type` → *communication semantics* (stream, datagram, raw)
* `protocol` → *exact protocol number* inside that family

---

## 2️⃣ Why this parameter exists at all (historical reason)

A single `(domain, type)` pair **can theoretically support multiple protocols**.

Example (conceptual):

```
AF_INET + SOCK_STREAM
    ├── TCP
    ├── SCTP
    └── some future protocol
```

So the OS designers added `protocol` to let you **explicitly choose**.

---

## 3️⃣ Why we almost always pass `0`

### Rule defined by POSIX

> If `protocol` is `0`, the system selects the **default protocol** for the given `(domain, type)`.

Examples:

| domain   | type        | protocol=0 resolves to |
| -------- | ----------- | ---------------------- |
| AF_INET  | SOCK_STREAM | TCP                    |
| AF_INET  | SOCK_DGRAM  | UDP                    |
| AF_INET6 | SOCK_STREAM | TCP                    |
| AF_UNIX  | SOCK_STREAM | Unix stream            |

So this:

```c
socket(AF_INET, SOCK_STREAM, 0);
```

means:

> “Give me the default stream protocol for IPv4 → TCP”

---

## 4️⃣ Why passing `0` is the CORRECT choice in ft_irc

For **ft_irc**:

* You want **IPv4**
* You want **reliable stream**
* There is **exactly one correct default** → TCP

So:

```cpp
socket(AF_INET, SOCK_STREAM, 0);
```

is:

* portable
* clean
* POSIX-compliant
* expected by evaluators

Passing anything else gives **no benefit**.

---

## 5️⃣ When would `protocol` NOT be zero? (advanced cases)

These are **NOT for ft_irc**, but good to understand.

### Example 1: Explicit TCP (rare)

```c
socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
```

Equivalent to protocol `0`, but:

* less portable
* more verbose
* unnecessary

---

### Example 2: Raw sockets (NOT allowed in 42)

```c
socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
```

Used for:

* `ping`
* network diagnostics
* packet crafting

❌ Requires root privileges
❌ Not relevant for IRC

---

### Example 3: Multiple protocols for same type (rare)

Some systems support:

* SCTP (`IPPROTO_SCTP`)
* DCCP

You would specify protocol explicitly.

---

## 6️⃣ What happens if you pass a WRONG protocol?

Example:

```c
socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
```

Result:

* ❌ `socket()` fails
* ❌ returns `-1`
* ❌ `errno = EPROTONOSUPPORT`

Because:

* UDP does NOT support stream semantics

---

## 7️⃣ Mental model (very important)

Memorize this mapping:

```
domain   = where (IPv4, IPv6, local)
type     = how (stream, datagram)
protocol = which exact implementation
```

And remember:

> **protocol = 0 means “choose the obvious one”**

---

## 8️⃣ One-sentence exam answer (memorize)

> The `protocol` parameter selects the specific protocol within a given domain and socket type, and passing `0` lets the system choose the default protocol, such as TCP for `AF_INET` and `SOCK_STREAM`.

---

## 9️⃣ Final rule for ft_irc (DO NOT DEVIATE)

```cpp
socket(AF_INET, SOCK_STREAM, 0);
```

Anything else:

* adds complexity
* gives no advantage
* may be considered wrong

---

Behind the scenes, the kernel:

1. Allocates socket-related internal structures (`struct socket`, `struct sock`).
2. Allocates a generic Unix `struct file` object.
3. Creates an FD entry in your process FD table.
4. Returns the integer `fd` to user space.

This FD is now your handle into the kernel's TCP/IP stack.

---

## 🔥 3. "Why not use read() and write()?"

You **can** use `read()` and `write()` on a socket. TCP sockets fully support these calls because they behave like any other file descriptor.

However, `send()` and `recv()` provide extra capabilities via flags, such as:

* `MSG_DONTWAIT` – non-blocking for this call
* `MSG_NOSIGNAL` – prevent `SIGPIPE`
* `MSG_PEEK` – examine data without removing it
* `MSG_MORE` – hint to TCP that more data is coming
* `MSG_WAITALL` – wait for the full buffer
* `MSG_OOB` – out‑of‑band data

So:

* **`read`/`write`** → simple, POSIX, generic
* **`send`/`recv`** → socket‑aware, more control

---

## 🔥 4. Types of Sockets

A socket is defined by three parameters:

```
(domain, type, protocol)
```

### 🌀 Internet Sockets (AF_INET / AF_INET6)

Used for network communication over IP.

Used in:

* IRC servers
* HTTP servers
* SSH
* general client-server applications

### 🧱 Unix Domain Sockets (AF_UNIX)

Use filesystem paths like:

```
/tmp/mysock
/var/run/docker.sock
```

Used for fast, local inter-process communication:

* Nginx ↔ PHP-FPM
* Docker engine
* System services

### 📦 Other Rare/Legacy Socket Types

E.g., X.25, Netlink, packet sockets.

For most developers—especially for an IRC server—these can be safely ignored.

---

## 🔥 5. The Core Message of This Article

The original article is essentially saying:

> "A socket is just a file descriptor backed by a kernel-network object. You obtain it using `socket()`. You can use `read` and `write`, but `send` and `recv` are better suited for network behavior. Unix abstracts all I/O via file descriptors."

This is the essence of the Unix philosophy:

* Everything is a file.
* Everything uses a file descriptor.
* Specialized subsystems add specialized calls when needed.

---

## 🔥 6. Want to Go Deeper?

If needed, you can explore topics such as:

* Kernel internals of `socket()`
* How file descriptor tables work
* TCP state machines and how they attach to sockets
* How `read()` maps to kernel TCP receive paths
* How user-space messages travel to the NIC
* Event loops with `select`, `poll`, `epoll`

Just ask if you want this extended in the next section!

## How `socket()` Works Internally (Deep Dive)

When you call:

```c
int fd = socket(AF_INET, SOCK_STREAM, 0);
```

you’re triggering a *system call* that makes the kernel create a new socket. Here’s the deep breakdown:

### 1. User Space → Kernel Space Transition

Calling `socket()` switches the CPU from **user mode (Ring 3)** to **kernel mode (Ring 0)**. Execution goes to the kernel function chain:

```
sys_socket → __sys_socket → __sock_create
```

### 2. Kernel Allocates `struct socket`

The kernel first creates a high-level socket structure:

```c
struct socket {
    socket_state state;
    struct proto_ops *ops;
    struct sock *sk;
};
```

It stores:

* the socket type (STREAM or DGRAM)
* the domain (AF_INET, AF_UNIX…)
* a table of operations (`send`, `recv`, `bind`, `connect`…)
* a pointer to the low-level TCP/UDP implementation (`struct sock`)

At this stage, the socket exists but has no IP, no port, no connection.

### 3. Kernel Creates the TCP/UDP Object: `struct sock`

This structure contains the full TCP state machine:

```c
struct sock {
    struct proto *protocol;
    struct sk_buff_head receive_queue;
    struct sk_buff_head write_queue;
    __u32 state;
};
```

It stores:

* connection state (CLOSED, LISTEN, ESTABLISHED…)
* send & receive buffers
* timers (retransmission, keepalive)
* congestion control
* sequence numbers
* retransmission logic
This is the real network endpoint.
This is the **true network object**.

### 4. Kernel Assigns a File Descriptor (FD)

The kernel adds an entry to your process’s FD table:

```
fd → struct file → struct socket → struct sock
```

This is why:

* sockets appear in `/proc/<pid>/fd/`
* you can use `read()` and `write()` on them
* epoll/select works on sockets

### 5. No Port or IP Yet

After `socket()` returns:

* no port is assigned
* no IP is associated
* no connection exists

The socket is created but passive.
The socket is created but unnamed.

Names (IP + port) are attached later using bind() — and this is where sockaddr_in enters the picture.
```md
# How IPv4 Addresses Are Passed to the Kernel: `sockaddr_in`

Before the kernel can bind a socket to an address, user space must describe that address in a standardized format.

For IPv4, that format is:

```

struct sockaddr_in

````

---

## What Is `sockaddr_in`?

`sockaddr_in` is the IPv4-specific structure used to describe:

- which protocol family (IPv4)
- which local IP address
- which TCP/UDP port

In other words:

> `sockaddr_in` is the **name you give to a socket**.

---

## Structure Definition (Simplified)

```cpp
struct sockaddr_in {
    sa_family_t    sin_family;   // Address family (AF_INET)
    in_port_t      sin_port;     // Port (network byte order)
    struct in_addr sin_addr;     // IPv4 address
    unsigned char  sin_zero[8];  // Padding (unused)
};
````

Each field exists for a reason.

---

## Field-by-Field Meaning

### `sin_family`

```c
sin_family = AF_INET;
```

Tells the kernel:

> “Interpret this address as IPv4.”

Without this, the kernel does not know how to parse the structure.

---

### `sin_port`

```c
sin_port = htons(6667);
```

* Stores the TCP/UDP port number
* Must be in **network byte order** (big-endian)

**Why?**
Because the network stack is architecture-independent.

---

### `sin_addr`

```c
sin_addr.s_addr = INADDR_ANY;
```

* `INADDR_ANY` means: bind on **all local interfaces**
* This is what servers use to accept external connections

**Alternative (testing only):**

```c
sin_addr.s_addr = inet_addr("127.0.0.1");
```

---

### `sin_zero`

Unused padding to make the structure compatible with `struct sockaddr`.

It must be zeroed but is never read.

---

## Why Is `sockaddr_in` Cast to `sockaddr`?

`bind()` is defined as:

```c
int bind(int fd, const struct sockaddr *addr, socklen_t len);
```

But IPv4 needs extra fields.

So user space does:

```c
bind(fd, (struct sockaddr *)&addr, sizeof(addr));
```

The kernel:

* Reads `sa_family`
* Sees `AF_INET`
* Interprets memory as `sockaddr_in`

This design allows **multiple address families** through one API.

---

## The `bind()`, `listen()`, `accept()` Sequence (Deep Internal Breakdown)

Now that you understand how sockets are created and how IPv4 addresses are represented, here is what happens next.

---

## 3. What `bind()` Really Does Internally

When you call:

```c
bind(fd, (struct sockaddr*)&addr, sizeof(addr));
```

you ask the kernel to attach a **local name (IP + port)** to your socket.

---

### 🔥 Inside the Kernel

* The kernel validates the `sockaddr_in`
* Checks port availability
* Verifies permissions (privileged ports)
* Applies rules like `SO_REUSEADDR`
* Stores the address internally:

  * **Local IP** → `sk->sk_rcv_saddr`
  * **Local port** → `sk->sk_num`

At this point, the socket finally has an identity.

---

### 🧠 Important Clarifications

* `bind()` does **not** make the socket listen
* `bind()` does **not** create a connection
* `bind()` only reserves an address + port

If `bind()` is skipped:

* the kernel auto-assigns an ephemeral port
* this happens during `connect()`, not before

```
```

Ports and addresses come from:

* `bind()` → assigns a local address/port
* `connect()` → initiates 3-way handshake
* `listen()` → marks socket as listening
* `accept()` → creates a *new* socket for each client

## The `bind()`, `listen()`, `accept()` Sequence (Deep Internal Breakdown)

Now that you know what happens inside `socket()`, here is what happens next in the lifecycle of a server socket.

---

# 3. What `bind()` Really Does Internally

When you call:

```c
bind(fd, (struct sockaddr*)&addr, sizeof(addr));
```

you ask the kernel to attach your socket to a specific local IP and port.

### 🔥 Inside the Kernel:

* The kernel first checks if the port is available.
* It checks socket rules: reserved ports (<1024), `SO_REUSEADDR`, conflicts, etc.
* Then it assigns:

  * **local IP** → stored in `sk->sk_rcv_saddr`
  * **local port** → stored in `sk->sk_num`

### 🧠 Important:

`bind()` does *not* make the socket listen.
`bind()` does *not* create a connection.
`bind()` only reserves an address + port pair.

If you skip `bind()`, the kernel will auto-assign:

* an ephemeral port
* local IP based on routing

This happens during `connect()`, not before.

---

# 4. What `listen()` Does in the Kernel

When you call:

```c
int listen(fd, backlog);
```
### * DESCRIPTION
    Creation of socket-based connections requires several operations.  First, a socket is created with socket(2).  Next, a willingness
    to accept incoming connections and a queue limit for incoming connections are specified with listen().  Finally, the connections
    are accepted with accept(2).  The listen() call applies only to sockets of type SOCK_STREAM.

    The backlog parameter defines the maximum length for the queue of pending connections.  If a connection request arrives with the
    queue full, the client may receive an error with an indication of ECONNREFUSED.  Alternatively, if the underlying protocol sup-
    ports retransmission, the request may be ignored so that retries may succeed.
### * RETURN VALUES
    The listen() function returns the value 0 if successful; otherwise the value -1 is returned and the global variable errno is set
    to indicate the error.

the kernel marks your socket as a **listening socket**.

### It changes the internal TCP state:

```
CLOSED → LISTEN
```

### And allocates two queues:

#### 1. **SYN Queue (Pending Queue)**

Stores half-open connections:

* after kernel receives SYN
* before the final ACK is sent

#### 2. **Accept Queue (Completed Queue)**

Stores fully established connections *ready* for `accept()`.

### `backlog` controls the size of the accept queue.

If both queues fill up:

* new clients get dropped or refused
* you see `ECONNREFUSED` or `ETIMEDOUT`

This is how Linux protects your server from overload.

---

# 5. The TCP 3-Way Handshake (Real Internal Flow)

This happens **only when a client calls `connect()`**.

### Step-by-step:

#### 🟦 1. Client → Server: **SYN**

Client wants to start a TCP session.

* kernel allocates a **request_sock**
* entry is put into the **SYN queue**

#### 🟩 2. Server → Client: **SYN + ACK**

Server accepts the request.

* sends SYN/ACK
* allocates full `struct sock`

#### 🟨 3. Client → Server: **ACK**

Client confirms.

### 🔥 Now kernel moves connection:

```
SYN queue → Accept queue
```

The connection is now **fully established**.
Waiting for your program to call:

```c
accept(fd, ...)
```

---

# 6. What `accept()` Does (Very Important!)

When you call:

```c
int client_fd = accept(server_fd, ...);
```

you are **not** reading anything.
You are **not** creating a new connection.

### ✔ `accept()` removes 1 connection from the *accept queue*.

### ✔ `accept()` creates a **new file descriptor**.

### ✔ This FD represents a unique TCP session.

The server now has two FDs:

* `server_fd` → the listening socket (never used to send/recv)
* `client_fd` → real connection with the client

### Internally:

* the kernel duplicates pointers to `struct sock`
* adds a new FD entry in your process table
* marks this new socket as `ESTABLISHED`

This is the socket you use for:

* send()
* recv()
* read()
* write()

---

# Summary Diagram

```
           socket()
              ↓
      +--------------------+
      |   Unbound Socket   |
      +--------------------+
              ↓ bind()
      +--------------------+
      | IP:PORT Assigned   |
      +--------------------+
              ↓ listen()
      +-------------------------------+
      |  LISTEN Socket + 2 Queues     |
      |  SYN Queue + Accept Queue     |
      +-------------------------------+
                     ↓ 3-Way Handshake
             (Kernel moves connection)
                     ↓ accept()
      +-------------------------------+
      |  New FD = Established Socket  |
      +-------------------------------+
```

If you want, next we can add:

* **How send/recv move data between user space and kernel buffers**
* **How epoll/select handle readiness and avoid blocking**
* **How Linux handles disconnects, FIN/ACK, TIME_WAIT**

## Deep Dive: Memory Structures Allocated by the Kernel for a Socket

When a socket is created, Linux allocates several core data structures. These structures live **in kernel space**, not in your process memory. Understanding them is essential for low-level networking.

---

# 1. `struct file` — The User-Space Handle (FD Table Entry)

Every socket you create becomes a file descriptor. The kernel allocates a `struct file`:

```c
struct file {
    struct file_operations *f_op;
    void *private_data;   // points to struct socket
    ...
};
```

### Purpose:

* Represents the socket in your process FD table
* Allows `read()`, `write()`, `poll()`, etc.

---

# 2. `struct socket` — The High-Level Socket Object

Allocated inside the kernel during `socket()` creation:

```c
struct socket {
    socket_state state;
    struct proto_ops *ops;
    struct sock *sk;       // pointer to the low-level TCP/UDP stack
};
```

### Purpose:

* Links your file descriptor to the networking stack
* Stores high-level operations (send, recv, accept)
* Not protocol-specific

---

# 3. `struct sock` — The Core TCP/UDP Endpoint

This is the **heart** of a network connection. Allocated when the kernel builds a full network endpoint.

```c
struct sock {
    int sk_state;              // TCP state machine
    struct sk_buff_head sk_receive_queue;
    struct sk_buff_head sk_write_queue;
    struct proto *sk_prot;     // TCP or UDP implementation
    __be32 sk_rcv_saddr;       // local IP
    __be16 sk_num;             // local port
    ...
};
```

### Purpose:

* Manages TCP state transitions (SYN_SENT, ESTABLISHED, etc.)
* Manages send/receive queues
* Stores IP/port
* Holds congestion control, timers, retransmission logic

This structure is large and complex; it represents **one endpoint** of a TCP connection.

---

# 4. `struct request_sock` — For Half-Open Connections (SYN Queue)

During the TCP 3-way handshake, before `accept()` happens, the kernel creates a lightweight structure:

```c
struct request_sock {
    struct sock_common *skc;   // partial connection info
    u32 rcv_isn;               // initial sequence number
    u32 snd_isn;
    ...
};
```

### Purpose:

* Exists only during handshake
* Lives in the **SYN queue**
* Promoted to a full `struct sock` when connection is established

---

# 5. `struct sk_buff` — The Packet Buffer (Critical!)

Every network packet is stored in a structure called the **socket buffer**:

```c
struct sk_buff {
    struct sk_buff *next;
    struct sk_buff *prev;
    unsigned char *head;
    unsigned char *data;
    unsigned char *tail;
    unsigned char *end;
    struct net_device *dev;
    struct sock *sk;
    ...
};
```

### Purpose:

* Stores actual packet data
* Used in both RX (receive) and TX (send) path
* Chained into queues:

  * `sk_receive_queue`
  * `sk_write_queue`

This is **where your data lives** before `recv()` or after `send()`.

---

# 6. Queues Allocated for TCP

A TCP socket allocates multiple internal queues:

### ✔ Receive Queue (`sk_receive_queue`)

* Filled by NIC/driver → network stack
* Drained by `recv()`

### ✔ Write Queue (`sk_write_queue`)

* Filled by `send()`
* Drained by TCP retransmission engine

### ✔ Out-of-order queue

* Holds packets that arrived early (reordering)

### ✔ SYN queue

* Holds half-open connections

### ✔ Accept queue

* Holds fully established connections waiting for `accept()`

---

# 7. Timer Structures

TCP allocates timers for:

* retransmission
* delayed ACK
* keepalive
* time-wait expiry

Each timer allocates a `timer_list` structure.

---

# 8. Memory for Routing and Neighbor Discovery

When a socket connects, kernel may allocate:

* route cache entries
* ARP/ND entries
* per-connection path MTU info

---

# Summary of All Kernel Allocations

```
socket() → allocates:
    struct file
    struct socket
    struct sock

listen() → allocates:
    SYN queue
    accept queue

Handshake → allocates:
    struct request_sock
    then full struct sock for new client

Data transfer → allocates:
    struct sk_buff (packet buffers)
    queue entries (RX/TX queues)
```
# Deep Networking Internals — Part 2: How `send()` and `recv()` Actually Work

This article continues the low-level journey, following the natural sequence after understanding socket creation, bind/listen/accept, and kernel memory structures.

We now dive into **what happens when your program calls `send()` and `recv()`** — step by step, from user space all the way to the NIC and back.

---

# 1. The Moment You Call `send()`

When you call:

```c
send(fd, buffer, len, flags);
```

your data does NOT go directly to the network.

### What actually happens:

1. **Copy From User Space → Kernel Space**
   Linux copies your bytes from your buffer into a freshly allocated `struct sk_buff`.

2. **Append to `sk_write_queue`**
   The packet buffer (skb) is added to the socket's write queue.

3. **TCP Transmission Logic Starts**
   The TCP stack takes over:

   * segmentation (split into MSS size)
   * sequence number assignment
   * congestion control
   * retransmission timer setup

4. **NIC Driver Takes the Packet**
   The packet is passed to the NIC driver and then DMA-transferred to hardware.

5. **Packet Goes on the Wire**
   The NIC sends the Ethernet frame.

### Key Insight:

`send()` returns **before the packet is delivered**.
It only confirms the data is now inside the kernel.

---

# 2. The NIC Sends the Packet (Hardware Path)

Once the skb is handed to the NIC driver:

* data is copied into a hardware ring buffer
* NIC uses DMA to read from kernel memory
* NIC creates the Ethernet frame
* NIC transmits it physically using electrical/optical signals

If the NIC is busy, packets are queued.

---

# 3. The Remote Host Receives the Packet

Once the packet reaches the destination machine:

* the NIC receives it and stores it into its RX ring
* raises an interrupt (or uses NAPI polling)
* kernel network stack processes the packet
* IP layer checks routing
* TCP layer validates sequence numbers
* payload is stored into `sk_receive_queue` of the destination socket

No user code is run at this stage.

---

# 4. The Moment You Call `recv()`

When you call:

```c
recv(fd, buffer, len, flags);
```

the kernel behaves depending on queue state.

### Case 1: Data available

If `sk_receive_queue` has sk_buffs:

* kernel copies data → user buffer
* dequeues the skb or adjusts its offset
* returns number of bytes copied

### Case 2: No data available

If blocking mode (default):

* the calling thread sleeps
* kernel wakes it when new data arrives

If non-blocking mode:

* `recv()` immediately returns `-1` with `errno = EAGAIN`

If socket is closed:

* returns `0` (EOF)

---

# 5. What Happens Inside the Kernel During `recv()`

The kernel performs:

1. dequeue head of `sk_receive_queue`
2. copy skb->data to your user buffer
3. free skb if fully consumed
4. update socket state (ACK handling, window updates)

TCP may also send **ACK packets** as a result of you calling `recv()`.

---

# 6. Zero-Copy Optimization (Advanced Concept)

For large sends/receives, Linux can avoid copying data using:

* `sendfile()`
* `splice()`
* `MSG_ZEROCOPY`

These allow the kernel to use page references instead of copying bytes.

But normal `send()`/`recv()` always copy.

---

# 7. Interaction With `epoll` / `select`

* `epoll` checks if queues are empty or full
* writable if `sk_write_queue` can accept more data
* readable if `sk_receive_queue` has at least one skb

`epoll` never reads or writes packets.
It only checks readiness.

---

# 8. Summary Diagram

```
User Buffer → send() → skb → write queue → TCP engine → NIC → Wire → NIC
→ kernel → skb → receive queue → recv() → User Buffer
```

---

If you want, next articles can cover:
