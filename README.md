## FT_IRC
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
