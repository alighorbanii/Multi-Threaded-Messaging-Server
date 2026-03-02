# Multi-Threaded Unix Instant Messaging Server  
C / POSIX Threads / FIFO-Based IPC

## Overview

This project implements a concurrent instant messaging system in C using Unix named pipes (FIFOs) and POSIX threads. The server coordinates communication between multiple clients while maintaining thread-safe shared session state and isolating blocking I/O operations per client.

The implementation focuses on low-level systems programming concepts including explicit resource management, concurrency control, process handling, and inter-process communication without relying on high-level networking frameworks.

---

## System Architecture

### Client–Server Model

- Clients register with the server through a well-known FIFO.
- Each client creates its own dedicated FIFO for receiving messages.
- The server maintains a registry of active users and associated FIFOs.
- A dedicated worker thread is spawned for each connected client.
- Messages are routed by the server to the appropriate client FIFO.

### Concurrency Model

The server follows a thread-per-client architecture:

- Each client connection is handled by a separate POSIX thread.
- Blocking I/O operations are isolated to individual threads.
- Shared session data structures are accessed in a controlled manner.
- The design prevents a single blocked client from degrading overall system responsiveness.

This model demonstrates foundational concurrent service design using explicit thread management.

---

## Key Technical Concepts Demonstrated

- POSIX thread lifecycle management (`pthread_create`)
- Inter-process communication using Unix named pipes (FIFOs)
- Process forking for concurrent input and output handling
- File descriptor management and cleanup
- Defensive error handling for system calls
- Thread-safe access to shared resources
- Explicit resource lifecycle control (`fclose`, `unlink`, termination paths)

---

## Message Flow

1. A client registers with the server via the registration FIFO.
2. The server initializes a session entry for the client.
3. The server spawns a dedicated worker thread.
4. The client sends commands such as `list` or `send <user> <message>`.
5. The server routes outgoing messages to the target client's FIFO.
6. Clients asynchronously receive and display messages.

---

## Build Instructions

Compile both the server and client:

```bash
make
```

Remove compiled binaries:

```bash
make clean
```

---

## Running the Application

### Start the server

```bash
./imserver /tmp/serverfifo
```

### Start clients (in separate terminals)

```bash
./imclient /tmp/serverfifo alice
./imclient /tmp/serverfifo bob
```

---

## Client Commands

- `list` — Display currently active users
- `send <username> <message>` — Send a message to a specific user

Example:

```
send bob Hello from alice
```

---

## Engineering Focus

This project prioritizes:

- Correctness under concurrent access
- Deterministic resource management
- Isolation of blocking operations
- Clear separation of client and server responsibilities
- Explicit control over Unix system primitives

The implementation intentionally avoids high-level abstractions in order to demonstrate foundational systems programming concepts in C.

---

## Limitations

- Designed for Unix-based environments
- No authentication or encryption
- Thread-per-client model not optimized for high-scale production workloads
- Intended for systems programming demonstration and educational purposes

---

## Author

Ali Ghorbani  
