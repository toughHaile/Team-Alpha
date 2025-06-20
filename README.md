# MiniGit - A Minimalist Version Control System

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Status](https://img.shields.io/badge/Status-Production%20Ready-brightgreen.svg)

## Project Overview
A lightweight Git-like version control system implementing core DVCS features in C++, designed as a teaching tool for understanding version control systems.

## Key Features
| Feature       | Status      | File               | Dependencies         |
|--------------|-------------|--------------------|----------------------|
| `init`       | ✅ Stable   | RepositoryManager  | filesystem           |
| `add`        | ✅ Stable   | RepositoryManager  | Blob, Logger         |
| `commit`     | ✅ Stable   | Commit             | Blob, StagingArea    |
| `log`        | ✅ Stable   | RepositoryManager  | Commit               |
| `branch`     | ✅ Stable   | BranchMap          | Commit               |
| `checkout`   | ✅ Stable   | BranchMap          | filesystem           |
| `merge`      | ✅ Stable   | BranchMap          | Commit, Diff         |
| `diff`       | ⚠️ Beta    | Diff               | Blob                 |

## Installation
### Requirements
- C++17 compiler (g++/clang++)
- OpenSSL 1.1.1+ (for SHA-1 hashing)
- CMake 3.12+ (optional)

### Build Instructions
```bash
# Clone repository
git clone https://github.com/your-username/Team-Alpha.git
cd Team-Alpha
# Build with CMake (recommended)
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4

# Or compile directly
g++ -std=c++17 -O2 -Wall src/*.cpp -Iinclude -lssl -lcrypto -o minigit


## Usage Examples
# Initialize repository
./minigit init

# Create and track files
echo "Hello World" > hello.txt
./minigit add hello.txt
./minigit commit -m "Initial commit"

# View history
./minigit log

# Create feature branch
./minigit branch feature
./minigit checkout feature

# Make changes and merge
echo "New feature" >> feature.txt
./minigit add feature.txt
./minigit commit -m "Add feature"
./minigit checkout main
./minigit merge feature

## Data Structures
```mermaid
classDiagram
    class RepositoryManager {
        +init()
        +add()
        +log()
    }
    class Commit {
        +hash
        +parent
        +getBlobs()
    }
    RepositoryManager --> Commit
    Commit --> Blob
## Team Members
## Team Members
| Name               | Role                | Key Contributions          |
|--------------------|---------------------|----------------------------|
| Hailemariam Dagnew | Project Lead        | Core Architecture          |
| Hearmon Tesfaye    | Blob System         | Hashing & Storage          |
| Oumer Jemal        | Commit System       | DAG Implementation         |
| Hiruy Dereje       | Staging Area        | Change Tracking            |
| Hiruy Legesse      | Branching System    | Merge/Checkout Logic       |
| Amanuel Dejene     | Logging System      | Activity Tracking          |
