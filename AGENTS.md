# AGENTS.md

## Project Overview

This repository implements a **Neural Network based Shogi AI**.

The project is divided into independent modules so that coding agents can implement and test each component incrementally.

Core pipeline:

```
Board Representation
      ↓
Move Generation
      ↓
Legal Move Validation
      ↓
Game Manager
      ↓
Self Play
      ↓
Training (Neural Network)
      ↓
Search (MCTS / AlphaBeta)
```

Primary language: **C++**

Neural network framework: **LibTorch (PyTorch C++ API)**

---

# Repository Structure

```
ShogiAI/
│
├ src/
│   ├ board/
│   │   board.hpp
│   │   bitboard.hpp
│   │
│   ├ move/
│   │   move.hpp
│   │   move_generator.hpp
│   │
│   ├ rules/
│   │   legal_move.hpp
│   │   win_condition.hpp
│   │
│   ├ search/
│   │   mcts.hpp
│   │   mate_solver.hpp
│   │
│   ├ nn/
│   │   model.hpp
│   │   inference.hpp
│   │
│   └ engine/
│       usi_engine.hpp
│
├ training/
│   dataset_builder.cpp
│   trainer.cpp
│
├ selfplay/
│   selfplay.cpp
│
├ tests/
│
├ configs/
│
└ CMakeLists.txt
```

Agents should place code in the appropriate module.

---

# Core Data Structures

## Board

Board uses **bitboards**.

```
struct Board {
    Bitboard piece_bb[32];
    int hand[2][8];
    int side_to_move;
};
```

Square index:

```
square = (file - 1) * 9 + (rank - 1)
```

Range:

```
0..80
```

---

# Move Representation

Move format:

```
struct Move {
    int from;
    int to;
    int piece;
    bool promote;
    bool drop;
};
```

Moves should be compatible with **USI notation**.

Example:

```
7g7f
P*5e
```

---

# Coding Rules

Agents must follow these rules.

### Language

C++20

### Naming

```
ClassName
snake_case_function
snake_case_variable
```

### Header

All headers must use:

```
#pragma once
```

### STL

Allowed:

```
vector
array
optional
span
```

Avoid heavy abstractions.

---

# Build Instructions

Build using CMake.

```
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

Binary:

```
./shogi_ai
```

---

# Testing

Unit tests should be added for:

* board representation
* move generation
* legal move validation

Example command:

```
ctest
```

Agents must update tests if behavior changes.

---

# Implementation Order

Agents should implement modules in this order.

1️⃣ Board representation
2️⃣ Move generator
3️⃣ Legal move validation
4️⃣ Game manager
5️⃣ Self-play generator
6️⃣ Neural network evaluation
7️⃣ MCTS search
8️⃣ Mate solver

---

# Performance Constraints

Move generation must be optimized.

Preferred techniques:

* bitboards
* bit operations
* lookup tables
* magic bitboards

Avoid dynamic allocations in hot paths.

---

# Neural Network

Model architecture:

```
Input: board tensor
Conv layers
Residual blocks
Policy head
Value head
```

Output:

```
policy[move]
value[-1..1]
```

Inference must support:

```
CPU
CUDA
```

---

# Training

Training pipeline:

```
selfplay → dataset → training
```

Training command example:

```
./trainer --dataset data.bin --epochs 100
```

---

# Self Play

Selfplay generates training data.

```
./selfplay --games 10000
```

Output:

```
selfplay_data.bin
```

---

# Cloud Training

Training can run on GPU instances.

Recommended:

```
RTX3090+
VRAM >= 24GB
CUDA 12
```

---

# Agent Behavior Rules

When modifying the repository, agents must:

1. Maintain buildability
2. Update tests
3. Avoid breaking interfaces
4. Keep modules independent
5. Prefer simple implementations first

Agents should **implement minimal working versions before optimizing**.

---

# Allowed Tasks for Agents

Agents are allowed to:

* implement missing modules
* refactor code
* add tests
* improve performance

Agents must **not change the project architecture** without explicit instruction.
