#ifndef BUGBOARD_H
#define BUGBOARD_H

#include <vector>
#include <string>
#include "Bug.h"

class BugBoard {
private:
    vector<Bug*> bugs;
    int nextId;

    // Helper: get alive bugs at a specific position
    vector<Bug*> getBugsAtPosition(pair<int, int> pos);

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
