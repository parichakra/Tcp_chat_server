# TCP Chat Server

This project implements a TCP-based chat server in C, allowing communication between two clients through a server. The server uses multi-threading to manage connections and relay messages between clients in real-time.

## Features
- TCP communication between two clients via a server.
- Multi-threaded server for handling multiple clients simultaneously.
- Real-time message relay between connected clients.

## Getting Started

### Prerequisites
- A C compiler (e.g., GCC).
- Linux environment (or any system supporting socket programming).

### Building the Project

1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd <repository_directory>
   ```

2. Compile the server and client programs:
   ```bash
   gcc -pthread -o server daytimetcpserver.c
   gcc -o client client.c
   ```

### Running the Programs

#### Step 1: Start the Server
Run the server program and specify the port number:
```bash
./server <port>
```
Example:
```bash
./server 8080
```

#### Step 2: Start the Clients
Run the client program on two separate terminals and specify the server IP address and port number:
```bash
./client <server_ip> <port>
```
Example:
```bash
./client 127.0.0.1 8080
```

#### Step 3: Communication
Once both clients are connected to the server:
- Type a message in one client terminal and press Enter.
- The server will relay the message to the other client terminal in real-time.

## File Structure
- `daytimetcpserver.c`: The server program.
- `client.c`: The client program.
- `README.md`: Documentation for the project.

## How It Works
1. **Server**:
   - Listens for incoming client connections.
   - Manages each client connection using a separate thread.
   - Relays messages between connected clients.

2. **Client**:
   - Connects to the server.
   - Allows users to send messages to the server.
   - Receives messages from the server sent by another client.

## Example

**Server Output:**
```
Server is waiting for connections at port 8080
Connection from 127.0.0.1, port 34567
Connection from 127.0.0.1, port 34568
Two clients connected. Communication enabled.
```

**Client 1 Terminal:**
```
Connected to the server. Type your messages below:
Hello from Client 1!
Message from other client: Hello from Client 2!
```

**Client 2 Terminal:**
```
Connected to the server. Type your messages below:
Hello from Client 2!
Message from other client: Hello from Client 1!
```

## Known Issues
- The server currently supports only two clients at a time. Future enhancements can include scaling to support multiple clients.

## License
This project is open-source and available under the [MIT License](LICENSE).

## Contributing
Contributions are welcome! Feel free to submit issues or pull requests to improve the project.

## Authors
- **Your Name** (Add your name and contact information here)

## Acknowledgments
- Inspired by basic TCP socket programming examples.
