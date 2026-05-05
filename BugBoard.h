#ifndef BUGBOARD_H
#define BUGBOARD_H

#include <vector>
#include <string>
#include "Bug.h"

// ANSI colour codes for enhanced console output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

class BugBoard {
private:
    vector<Bug*> bugs;
    int nextId;

    // Helper: get alive bugs at a specific position
    vector<Bug*> getBugsAtPosition(pair<int, int> pos);

    // Display stats summary
    void displayStats() const;

public:
    BugBoard();
    ~BugBoard();

    // Menu 1: Initialize Bug Board from file
    bool initializeFromFile(const string& filename);

    // Menu 2: Display all Bugs
    void displayAllBugs() const;

    // Menu 3: Find a Bug by ID
    void findBug(int id) const;

    // Menu 4: Tap the Bug Board
    void tap();

    // Menu 5: Display Life History of all Bugs
    void displayLifeHistory() const;

    // Menu 6: Display all Cells listing their Bugs
    void displayAllCells() const;

    // Menu 7: Run simulation (auto-tap every second)
    void runSimulation();

    // Menu 8: Write Life History to file
    void writeLifeHistoryToFile(const string& filename);

    // Helper: count alive bugs
    int countAlive() const;

    // Check if board is initialized
    bool isInitialized() const;
};

#endif
