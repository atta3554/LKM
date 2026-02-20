# Simple Netlink-Based Linux Kernel Module (LKM)

A minimal **Linux Loadable Kernel Module (LKM)** that communicates with **user space** via **Netlink sockets**.

The module receives a message (command + parameters) from a user-space program, performs an action based on the received parameters, and sends a response back to user space.

---

## Features

- Kernel ↔ User-space communication using **Netlink**
- Simple command/parameter parsing
- Sends structured responses back to the user-space client
- Minimal and easy-to-read codebase (educational purpose)

---

## Requirements

- Linux kernel headers for your running kernel
- Build tools: `make`, `gcc`
- Root privileges to load/unload kernel modules

On Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
