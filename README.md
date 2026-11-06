# Chess

A chess engine in C++ where two AIs play against each other in the terminal.

> **Status: work in progress.** A graphical interface and more search algorithms are planned.

## Features

- **Full piece set**: pawn, rook, knight, bishop, queen and king, each with its own move rules
- **Search algorithms**:
  - Minimax
  - Alpha-beta pruning, with a transposition table to reuse already evaluated positions
- **Evaluation**: material value plus a positional bonus for each piece
- **Parallel search**: the root moves are explored in parallel with OpenMP

## Build and run

Requires `g++` with OpenMP support (Linux).

```bash
make          # build the app
make run      # play an AI vs AI game
make valgrind # play a game under Valgrind (memory leak check)
make test     # run the tests under Valgrind
make clean    # remove build files
```

The search depth and the algorithms used by each side are set in `src/App.cpp`.

## Project structure

```
include/chess/       # Headers
src/chess/
├── Chess.cpp        # Game loop and rules
├── board/           # Board representation and display
├── piece/           # Pieces and their moves
└── algorithms/      # Minimax, alpha-beta, heuristics
test/chess/          # Tests
```

## Ideas

- Graphical interface
- Negamax and other search algorithms
- Better parallelization
