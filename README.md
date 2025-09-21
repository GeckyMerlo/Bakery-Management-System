# 🧁 Bakery Management System

**Language:** C  
**Tools:** GCC, Make, GDB, AddressSanitizer (ASAN), Valgrind (Memcheck, Callgrind, Massif), KCachegrind, Massif-Visualizer, Git, VS Code  
**Project Type:** Individual  
**Date:** September 2025  

A high-performance administrative software designed to manage orders, inventory, and recipes for a bakery.  
The program demonstrates the practical application of **algorithms and data structures** to ensure correctness, scalability, and time-space efficiency.

---

## 📋 Project Overview

This project was developed as part of the **Algorithms and Data Structures (2023/2024)** course during the **Computer Engineering BSc at Politecnico di Milano**.  
The official requirements and specifications are provided in Italian (see `2023_2024.pdf`).

The system focuses on:

- Implementing efficient **data structures and algorithms**  
- Guaranteeing **correctness** under all constraints  
- Ensuring **performance optimization** (time and space)  
- Dynamically processing **stdin commands** and producing the corresponding **stdout results**

The software autonomously:

1. Reads an input sequence containing specific commands  
2. Updates the internal state of the bakery accordingly  
3. Produces the expected outputs in real time  

---

## ✨ Features

- 📦 **Order Management** → Create and manage pastry orders  
- 🗃️ **Inventory Control** → Manage ingredient batches and expiration dates  
- 📜 **Recipe Handling** → Add, remove, and validate recipes  
- ⚡ **Performance-Oriented Design** → Optimized for speed and memory  
- 🖥️ **Autonomous Processing** → Reads commands from stdin, writes results to stdout  

---

## 🛠️ Prerequisites

Before running the program, ensure your system has:

- Linux or macOS  
- GCC (or Clang)  
- Make (for automatic builds)  
- Git (to clone the repository)  
- Debugging/Profiling Tools:
  - **GDB** → runtime debugging  
  - **AddressSanitizer (ASAN)** → detect memory errors at runtime  
  - **Valgrind** (Memcheck, Callgrind, Massif) → memory debugging and performance profiling  
  - **KCachegrind** → visualize Callgrind outputs  
  - **Massif-Visualizer** → visualize memory usage from Massif  

---

## 🚀 Instructions

1. **Open a terminal**  

2. **Clone the repository and navigate to the source folder**  
   ```bash
   git clone https://github.com/<your-username>/Bakery-Simulation.git
   cd Bakery-Simulation
   ```

3. **Build the program**  
   ```bash
   gcc -Wall -Werror -std=gnu11 -O2 -o bakery bakery.c -lm
   ```
   or simply:
   ```bash
   make
   ```

4. **Run the program with the provided test cases**  
   ```bash
   ./bakery < test_cases_pubblici/open1.txt > program.output.txt
   ```

5. **Verify the correctness by comparing outputs**  
   ```bash
   diff test_cases_pubblici/open1.output.txt program.output.txt
   ```

   ✅ If there are no differences, the test is passed.  

---

## 🛠️ Generating Additional Tests

1. Generate a new input file with the official generator:  
   ```bash
   ./generatore_casi_test/test_gen_2024_macos > new_test.txt
   ```

2. Generate the expected output using the official solution:  
   ```bash
   ./generatore_di_soluzioni/sol_2024_macos < new_test.txt > new_test.output.txt
   ```

3. Run your implementation and compare results:  
   ```bash
   ./bakery < new_test.txt > my_output.txt
   diff new_test.output.txt my_output.txt
   ```

---

## 📖 Problem Statement

The project simulates the **order management system of an industrial bakery** in discrete time:  
- A catalog of recipes with ingredients and quantities  
- A warehouse with refills and expiration dates  
- Customer orders, queued if ingredients are missing  
- A delivery van with **limited capacity** and **periodic departures**  
- Output includes confirmations of commands and the van’s load at each departure  

For full details, see the [problem statement](./2023_2024.pdf).

---

## ✅ Goals

- Correctly implement the simulation in `bakery.c`  
- Pass all provided public tests  
- Validate performance under additional generated tests  
- Measure and improve performance using **Valgrind, Callgrind, and Massif**  

---

## 📝 Notes

The program was adapted and tuned to perform efficiently on the **online verifier** used in the course.  
Because of this design choice, it may not pass **test case 8**, due to a too high number of collisions in the hash table.  
This behavior is expected and results from prioritizing performance on the verifier environment.