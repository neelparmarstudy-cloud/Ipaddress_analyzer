# IP Address Analyzer

A low-level networking utility program built in C designed to validate and break down both IPv4 and IPv6 network addresses. The application manually parses address structures using string tokens and format blocks, executing deep subnet boundary maps and scoping evaluations without relying on external network dependencies.

## 🚀 Key Features

### 🔹 IPv4 Deep Analysis
* **Strict Structural Validation:** Verifies length, checks for non-numeric input parameters, ensures octet integer bounds match values between `0` and `255`, and flags invalid leading zeros.
* **Classful Routing Detection:** Identifies the network category structure (Class A, B, C, D, or E) dynamically using the first octet range.
* **Subnet & Metric Computation:** Calculates corresponding Default Subnet Masks, isolated Network Addresses, Host Addresses, Subnet IDs, and explicit Classless Inter-Domain Routing (CIDR) block notations.
* **Network Scoping:** Identifies local loopback paths (`127.0.0.0/8`) and categorizes addresses as Public or Private spaces (matching `10.x`, `172.16-31.x`, or `192.168.x` parameters).

### 🔹 IPv6 Structural Mapping
* **Colon & Hexadecimal Boundary Parsing:** Validates hex character sets (`0-9`, `a-f`), manages string length requirements, and tracks zero-compression double colons (`::`) safely to reject multiple compressions.
* **Address Classification:** Flags Unspecified addresses (`::`), local Loopback diagnostic structures (`::1`), Link-Local unicast blocks (`fe80::`), and Global Unicast targets.

---

## 📁 Repository Structure
📁 IP-Address-Analyzer/
│
├── 📄 Ipadress_analyser.c    # Core C source script containing all parsing, validation, and analytics engines
├── 📄 .gitignore             # Excludes volatile platform binaries and terminal store configurations
├── 📄 LICENSE                # MIT License open-source terms
└── 📄 README.md              # Comprehensive project overview and deployment guide
🛠️ Tech Stack
Language: C (C99 standard or higher recommended for <stdbool.h>)

Standard System Libraries: <stdio.h>, <string.h>, <stdlib.h>, <stdbool.h>, <ctype.h>

⚙️ Compilation & Run Guide
To compile and execute this console application across your development terminal:

1. Clone the Repository
Bash
git clone [https://github.com/YOUR_USERNAME/Ipaddress_analyzer.git](https://github.com/neelparmarstudy-cloud/Ipaddress_analyzer.git)
cd IP-Address-Analyzer

3. Compile the Source Code
Compile the code using a C compiler (such as gcc):

Bash
gcc -std=c99 Ipadress_analyser.c -o IPAnalyzer

3. Execute the Binary
On Windows Workstations:

DOS
Ipadress_analyzer.exe
