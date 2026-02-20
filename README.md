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
```

---

##Build
```bash
make
```
This should produce a .ko file (e.g., netlink_lkm.ko).

---

##Load / Unload
###Load
```bash
sudo insmod netlink_lkm.ko
dmesg | tail -n 50
```
###Unload
```bash
sudo rmmod netlink_lkm
dmesg | tail -n 50
```

---

##User-space Client
A user-space program sends a Netlink message to the module and prints the received reply.
Example (replace with your actual binary name/path):
```bash
make user
./user_client "command" "param1=value1" "param2=value2"
```

---

Notes & Safety

Kernel modules run in kernel space. A bug can crash the system.
Test in a VM first.
Keep code minimal and validate user input.
Avoid unsafe memory operations.
