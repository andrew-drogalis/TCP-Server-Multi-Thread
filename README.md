# Multithread TCP / UDP Server

## Table of Contents

- [About the Project](#About-the-Project)
- [Running the Client and Server](#Running-the-Client-and-Server)
- [Benchmarks](#Benchmarks)
- [Background Information](#Background-Information)
- [Dependencies](#Dependencies)
- [Building Executable](#Building-Executable)
- [License](#License)

## About the Project

This project uses a multithread server with an epoll instance to handle multiple connections, each on an individual thread. The goal was to apply modern Linux socket programming techniques, similar to those of real-world large scale applications.

Older methods, such as poll and select, are not scale-able as they perform operations with a linear time complexity. Epoll utilized an RB-Tree (Red Black Tree) under the hood to keep track of the file descriptors.

## Running the Client and Server

The Client takes (5) arguments which override the default values.

```bash
./Client -i [IP Address] -p [Port Number] -b [Buffer Size] -t [TCP/UDP] -m [Benchmark]
```

```text
IP Address: Standard IPv4 Address
Port Number: Valid Port Number 0 - 65535
Buffer Size: Int > 0;
TCP/UDP: Char T or U;
Benchmark: bool;
```

The Multithread-Server takes (5) arguments and can be passed to the executable with the following flags.

```bash
./Multithread-Server -p [Port Number] -c [Max Clients] -e [Max Events] -b [Buffer Size] -t [TCP/UDP]
```

```text
Port Number: Valid Port Number 0 - 65535
Max Clients, Max Events, Buffer Size: Int > 0;
TCP/UDP: Char T or U;
```

The server should be initialized prior to the client, otherwise the client will fail to connect since there is no server online. The resulting client / server program is shown below.

<img src="https://raw.githubusercontent.com/andrew-drogalis/Multithread-TCP-UDP-Server/main/assets/TCP-Server_Client.png" alt="TCP Client Server Terminal" style="width: 850px; padding-top: 10px;">

## Benchmarks

## Background Information

The following is a diagram of the OSI (Open System Interconnection) model. The TCP and UDP is located on the 4th layer (Transport).

<img src="https://raw.githubusercontent.com/andrew-drogalis/Multithread-TCP-UDP-Server/main/assets/Network-Stack-Models1.png" alt="Database-Online-Screenshot" style="width: 500px; padding-top: 10px;">

Source: https://vichargrave.github.io/programming/tcp-ip-network-programming-design-patterns-in-cpp/

The diagram below demonstrates the order of operations to initialize a standard server / client TCP application.

<img src="https://raw.githubusercontent.com/andrew-drogalis/Multithread-TCP-UDP-Server/main/assets/Socket-Workflow.png" alt="Socket Workflow" style="width: 500px; padding-top: 10px;">

Source: https://vichargrave.github.io/programming/tcp-ip-network-programming-design-patterns-in-cpp/

The epoll instance is apart of the Linux kernel. Once there are file descriptors in the ready list, epoll_wait returns the number of FD's that are ready.

<img src="https://raw.githubusercontent.com/andrew-drogalis/Multithread-TCP-UDP-Server/main/assets/epoll_ready.png" alt="Database-Online-Screenshot" style="width: 500px; padding-top: 10px;">

Source: https://medium.com/@avocadi/what-is-epoll-9bbc74272f7c

## Dependencies

This repository contains a .devcontainer directory. The .devcontainer has all the required dependencies and can be run inside Docker with the Dev Containers VSCode extension.

#### User Install Required

- [Google Tests](https://github.com/google/googletest) | Testing Only

## Building Executable

```
$ cd server && mkdir build
$ cmake -S . -B build
$ cmake --build build --target Multithread-Server

$ cd client && mkdir build
$ cmake -S . -B build
$ cmake --build build --target Client
```

## License

This software is distributed under the GNU license. Please read [LICENSE](https://github.com/andrew-drogalis/Multithread-TCP-Server/blob/main/LICENSE) for information on the software availability and distribution.
