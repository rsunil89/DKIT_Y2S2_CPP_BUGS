# A Bug's Life - Bug Board Simulation

A C++ console application that simulates the movement and interaction of various bugs on a 10x10 grid board.

## Features

- **Initialize Bug Board** - Load bugs from a text file (`bugs.txt`)
- **Display All Bugs** - View all bugs with their details (ID, type, position, health, direction, status)
- **Find a Bug** - Search for a specific bug by ID
- **Tap the Board** - Triggers movement for all bugs, then fights/eating on occupied cells
- **Life History** - View the path each bug has travelled
- **Cell Display** - See which bugs occupy each cell on the board
- **Run Simulation** - Auto-tap every second until one bug remains
- **Exit & Save** - Writes life history to `bugs_life_history_<timestamp>.out`

## Bug Types

| Type | Movement Behaviour |
|------|-------------------|
| **Crawler (C)** | Moves 1 unit in the direction it's facing |
| **Hopper (H)** | Hops 2-4 units in the direction it's facing |
| **Flyer (F)** | Moves diagonally 1 unit in both X and Y |

## Building

### Using CMake:
```bash
mkdir build && cd build
cmake ..
make
./BugLife
```

### Using g++ directly:
```bash
g++ -std=c++11 -o BugLife main.cpp Bug.cpp Crawler.cpp Hopper.cpp Flyer.cpp BugBoard.cpp
./BugLife
```

### Pre-built binary:
```bash
./main
```

## Input File Format (`bugs.txt`)

Lines are semi-colon delimited: `Type;ID;X;Y;Direction;Health;[HopLength]`

```
C;101;0;0;4;10          # Crawler
H;102;9;0;1;8;2         # Hopper (hopLength=2)
F;111;3;3;2;6           # Flyer
```

## Menu Options

1. Initialize Bug Board
2. Display all Bugs
3. Find a Bug
4. Tap the Bug Board
5. Display Life History
6. Display all Cells
7. Run Simulation
8. Exit

## Assignment

Developed for DKIT Year 2, Semester 2 - C++ Continuous Assessment.