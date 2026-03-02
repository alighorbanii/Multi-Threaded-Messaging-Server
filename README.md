# Multi-Threaded Unix Instant Messaging Server  
C / POSIX Threads / FIFO-Based IPC

## Overview

This project implements a concurrent instant messaging system in C using Unix named pipes (FIFOs) and POSIX threads. The server manages communication between multiple clients while maintaining thread-safe shared session state and isolating blocking I/O operations per client.

The implementation focuses on low-level systems programming concepts, including explicit resource management, process control, and concurrency handling without relying on high-level networking frameworks.

---

## Architecture

### Client-Server Model

- Clients connect to the server via a well-known FIFO.
- Each client creates a dedicated FIFO for receiving messages.
- The server maintains a registry of active users.
- A dedicated worker thread is created for each connected client.
- Messages are routed through client-specific FIFOs.

### Concurrency Model

The server follows a thread-per-client architecture:

- Each client connection is handled by a separate POSIX thread.
- Blocking operations are isolated to individual threads.
- Shared session data structures are managed safely under concurrent access.
- The design prevents a single blocked client from impacting overall server responsiveness.

---

## Technical Concepts Demonstrated

- POSIX thread creation and lifecycle management (`pthread_create`)
- Unix inter-process communication using named pipes (FIFOs)
- Process forking for concurrent input/output handling
- File descriptor management and cleanup
- Defensive error handling for system calls
- Thread-safe access to shared resources

---

## Message Flow

1. Client registers with the server via the registration FIFO.
2. Server initializes client session state.
3. Server spawns a worker thread for the client.
4. Client sends commands (`list`, `send <user> <message>`).
5. Server routes messages to the appropriate client FIFO.
6. Clients asynchronously receive and display messages.

---

## Build

```bash
make
