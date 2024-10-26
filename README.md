
# C Virtual Memory Allocator

### Author: Nichita-Adrian Bunu, 323CA Facultatea de Automatica si Calculatoare UNSTPB 
**Contact:** [nichita_adrian.bunu@stud.acs.upb.ro](mailto:nichita_adrian.bunu@stud.acs.upb.ro)

---

## Overview

Simulation of a kernel buffer, using an approach that leverages linked lists.
A virtual memory allocator (VMA) allows for dynamic memory management, where blocks of memory are allocated, deallocated, read, written, and protected within an "arena." The allocator supports memory protection levels and visual representation of allocated and free memory blocks.

Functions: ALLOC_ARENA, DEALLOC_ARENA, ALLOC_BLOCK, FREE_BLOCK, READ, WRITE, PMAP, MPROTECT

---

## Features and Functionality

- **Memory Arena Management**:
  - Allocate and deallocate an arena for managing memory.
- **Block Allocation and Freeing**:
  - Allocate blocks within the arena and manage them using a linked list structure.
  - Merge blocks when possible to optimize memory usage.
- **Memory Read and Write**:
  - Support for reading and writing data to specific addresses within allocated blocks.
  - Memory permissions for read, write, and execute operations.
- **Memory Protection**:
  - Allows setting protection permissions on allocated memory regions.
  - Verifies permissions before allowing read/write access.
- **Memory Map Display**:
  - Shows the memory layout of the arena, including allocated blocks and their permissions.

---

## Code Structure

### `main.c`

Implements the primary functionality and interface for interacting with the VMA:
- **Command Handling**: Processes commands like `ALLOC_ARENA`, `ALLOC_BLOCK`, `FREE_BLOCK`, `READ`, `WRITE`, `PMAP`, and `MPROTECT`.
- **Interactive Mode**: Accepts commands from the user, performs actions on the arena, and provides feedback.

### `vma.c`

Contains the core memory management functions:
- **Memory Allocation**: Manages the arena allocation (`alloc_arena`), block allocation (`alloc_block`), and deallocation (`dealloc_arena`).
- **Memory Access**: Implements functions for reading, writing, and protecting memory blocks.
- **Utility Functions**: Functions like `pmap` to print the memory layout, and permission parsing functions for setting access rights.

---

## How to Run

1. **Build the project**:
   ```bash
   make build
   ```

2. **Run the executable**:
   ```bash
   make run_vma
   ```

3. **Clean up the compiled files**:
   ```bash
   make clean
   ```

---
