# PrimMST-RadioRange-Optimization

University algorithm exam project: Implementation of Prim's algorithm for Minimum Spanning Tree (MST) computation to optimize communication between outposts by minimizing radio range.

## Overview

This project addresses the problem of establishing communication between a network of outposts using both satellite and radio channels.  The goal is to minimize the required radio communication range (distance 'D') by strategically assigning a limited number of satellite channels to outposts.

The solution utilizes Prim's Minimum Spanning Tree (MST) algorithm to find the most cost-effective network connections, where cost is represented by the Euclidean distance between outposts. After computing the MST, satellite channels are assigned to the endpoints of the MST edges with the highest weights. This strategic allocation reduces the maximum distance that radio communication needs to cover, thus minimizing the required radio range 'D'.

## Algorithms and Data Structures Used

*   **Prim's Minimum Spanning Tree (MST) Algorithm:**  Used to compute the minimum total distance to connect all outposts. The eager version of Prim's algorithm is implemented for efficiency, especially in dense graphs.
*   **Priority Queue:** A custom `PriorityQueue` class (implemented as a heap) is used within Prim's algorithm to efficiently select the next vertex to add to the MST and to update edge priorities.
*   **Adjacency List:** The graph representing the outposts and their potential connections is stored using an adjacency list representation.

## Project Structure

The repository is organized as follows:
Minimum-Radio-Range-Outposts-MST/
├── src/
│ ├── main.cpp
│ ├── Graph.h
│ ├── PriorityQueue.h
│ └── Coords.h
├── doc/
│ └── Report.pdf
├── input.txt
└── README.md

*   **`src/`**: Contains all the C++ source code files (`.cpp` and `.h`).
    *   `main.cpp`:  The main program file, responsible for input processing, graph construction, algorithm execution, and output.
    *   `Graph.h`:  Header file for the `Graph` class, which implements the graph data structure and Prim's MST algorithm.
    *   `PriorityQueue.h`: Header file for the custom `PriorityQueue` class, used for efficient implementation of Prim's algorithm.
    *   `Coords.h`: Header file for the `Coords` class, used to represent outpost coordinates.
*   **`doc/`**: Contains project documentation.
    *   `Report.pdf`: The project report in PDF format, detailing the problem, solution, algorithm explanation, complexity analysis, and test results.
*   **`input.txt`**: The input file containing test cases for the program.
*   **`README.md`**: This README file, providing an overview of the project and instructions.

## How to Compile and Run

Assuming you have a C++ compiler (like g++) installed:
1.  **Compile the code:**
    ```bash
    g++ ./src/main.cpp -o outpost_communication
    ```
2.  **Run the executable from the repository root (where `input.txt` is located):**
    ```bash
    ./outpost_communication
    ```
    The program will read input from `input.txt` and output the minimum radio range 'D' for each test case to the console.

## Input File (`input.txt`) Format

The `input.txt` file should be formatted as follows:

N <-- Number of Test Cases
S_1 V_1 <-- Test Case 1: Number of Satellites, Number of Outposts
x_1 y_1 <-- Outpost 1 Coordinates
x_2 y_2 <-- Outpost 2 Coordinates
...
x_V1 y_V1 <-- Outpost V1 Coordinates
S_2 V_2 <-- Test Case 2: Number of Satellites, Number of Outposts
x_1 y_1
...
x_V2 y_V2
... (continues for N test cases)

Where:

*   `N`:  The total number of test cases.
*   `Si`:  The number of satellite channels available for the i-th test case.
*   `Vi`:  The number of outposts in the i-th test case.
*   `xi_j yi_j`: The x and y coordinates of the j-th outpost in the i-th test case.

## Report

For a detailed explanation of the problem, the algorithms used, complexity analysis, and test results, please refer to the `Report.pdf` file located in the `doc/` directory.

## Authors

*   Francesco La Rosa
*   Dario Zappata
---
