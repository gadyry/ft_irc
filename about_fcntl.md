To see how `O_NONBLOCK` changes a program's behavior, we can use a simple C program that reads from **Standard Input (stdin)**.

By default, `read()` is **blocking**. If you run the program and don't type anything, the program just sits there and waits. With `O_NONBLOCK` set, the program will try to read, fail immediately, and move on.

### The Code Example

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    char buf[10];
    int fd = 0; // Standard Input (stdin)

    // 1. First attempt: Normal (Blocking) mode
    printf("1. Normal Mode: Type something and press Enter (I will wait...)\n");
    int n = read(fd, buf, 10); 
    printf("Read %d bytes: %.*s\n", n, n, buf);

    // 2. Set the O_NONBLOCK flag
    int flags = fcntl(fd, F_GETFL, 0);       // Get current "Settings Menu"
    fcntl(fd, F_SETFL, flags | O_NONBLOCK); // Toggle "Non-Blocking" to ON

    // 3. Second attempt: Non-blocking mode
    printf("\n2. Non-Blocking Mode: Checking for data (I won't wait!)\n");
    n = read(fd, buf, 10);

    if (n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Result: No data available right now. I'm moving on!\n");
        }
    } else {
        printf("Read %d bytes: %.*s\n", n, n, buf);
    }

    return 0;
}

```

---

### What is happening in the Kernel?

When you run this code, the interaction with the **Open File Table** changes:

1. **Before `fcntl**`: The kernel looks at the `f_flags` for `fd 0`. The `O_NONBLOCK` bit is `0`. The kernel puts your process to "sleep" until someone types something in the terminal.
2. **The `fcntl` call**: The kernel goes into the `struct file` for your terminal connection and updates the `f_flags` field, flipping the bit for `O_NONBLOCK` to `1`.
3. **After `fcntl**`: When you call `read()`, the kernel checks that bit. Because it is now `1`, the kernel says: *"The 'Don't Wait' switch is on. Since there's no data in the buffer, I'll return -1 and set errno to `EAGAIN` (Try Again) immediately."*

### Common Use Case

This is how **Web Servers** (like Nginx) handle thousands of connections at once. Instead of having one thread waiting (blocking) on every single user, one thread loops through all the File Descriptors. It asks each one: *"Got data?"* If the answer is "No," it moves to the next one instantly thanks to `O_NONBLOCK`.

