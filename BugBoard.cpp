#include "BugBoard.h"
#include "Crawler.h"
#include "Hopper.h"
#include "Flyer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

BugBoard::BugBoard() : nextId(1) {
    srand(static_cast<unsigned>(time(nullptr)));
}

BugBoard::~BugBoard() {
    for (Bug* b : bugs) {
        delete b;
    }
    bugs.clear();
}

vector<Bug*> BugBoard::getBugsAtPosition(pair<int, int> pos) {
    vector<Bug*> result;
    for (Bug* b : bugs) {
        if (b->isAlive() && b->getPosition() == pos) {
            result.push_back(b);
        }
    }
    return result;
}

// ============================================================
// Menu 1: Initialize Bug Board from file
// ============================================================
bool BugBoard::initializeFromFile(const string& filename) {
    // Clear existing bugs
    for (Bug* b : bugs) delete b;
    bugs.clear();
    nextId = 1;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'\n";
        return false;
    }

    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        // Skip empty lines
        if (line.empty()) continue;

        stringstream ss(line);
        string token;

        // Format: Type;ID;X;Y;Direction;Health[;HopLength]

        // Read Type
        getline(ss, token, ';');
        string typeStr = token;

        // Read ID
        getline(ss, token, ';');
        int id = stoi(token);

        // Read X
        getline(ss, token, ';');
        int x = stoi(token);

        // Read Y
        getline(ss, token, ';');
        int y = stoi(token);

        // Read Direction
        getline(ss, token, ';');
        int dir = stoi(token);

        // Read Health
        getline(ss, token, ';');
        int health = stoi(token);

        if (x < 0 || x > 9 || y < 0 || y > 9) {
            cout << "Warning: Position out of bounds on line " << lineNum << "\n";
            continue;
        }

        if (health < 1 || health > 20) {
            cout << "Warning: Invalid health value on line " << lineNum << "\n";
            continue;
        }

        Direction direction = static_cast<Direction>(dir);
        pair<int, int> pos(x, y);
        Bug* bug = nullptr;

        if (typeStr == "C" || typeStr == "c") {
            bug = new Crawler(id, pos, direction, health);
        } else if (typeStr == "H" || typeStr == "h") {
            // Read HopLength
            getline(ss, token, ';');
            int hopLength = stoi(token);
            bug = new Hopper(id, pos, direction, health, hopLength);
        } else if (typeStr == "F" || typeStr == "f") {
            bug = new Flyer(id, pos, direction, health);
        } else {
            cout << "Warning: Unknown bug type '" << typeStr << "' on line " << lineNum << "\n";
            continue;
        }

        if (bug) {
            bugs.push_back(bug);
            cout << "Loaded: " << bug->toString() << "\n";
            // Track the highest ID to set nextId properly
            if (id >= nextId) nextId = id + 1;
        }
    }

    file.close();
    cout << "\nLoaded " << bugs.size() << " bugs from file.\n";
    return true;
}

// ============================================================
// Menu 2: Display all Bugs
// ============================================================
void BugBoard::displayAllBugs() const {
    if (bugs.empty()) {
        cout << "No bugs on the board.\n";
        return;
    }

    cout << "\n===== All Bugs =====\n";
    for (const Bug* b : bugs) {
        string dirStr;
        switch (b->getDirection()) {
            case Direction::NORTH: dirStr = "North"; break;
            case Direction::EAST:  dirStr = "East";  break;
            case Direction::SOUTH: dirStr = "South"; break;
            case Direction::WEST:  dirStr = "West";  break;
            default:               dirStr = "None";  break;
        }

        cout << b->getId() << " "
             << b->getType() << " "
             << "(" << b->getPosition().first << "," << b->getPosition().second << ") "
             << b->getHealth() << " "
             << dirStr << " "
             << (b->isAlive() ? "Alive" : "Dead");

        // Show hopLength for Hoppers
        if (b->getHopLength() > 0) {
            cout << " " << b->getHopLength();
        }

        cout << "\n";
    }
    cout << "Total: " << bugs.size() << " bugs (" << countAlive() << " alive)\n";
}

// ============================================================
// Menu 3: Find a Bug by ID
// ============================================================
void BugBoard::findBug(int id) const {
    for (const Bug* b : bugs) {
        if (b->getId() == id) {
            cout << "\nBug Found:\n";
            cout << b->toString() << "\n";
            cout << "Life History: " << b->lifeHistoryToString() << "\n";
            return;
        }
    }
    cout << "Bug with ID " << id << " not found.\n";
}

// ============================================================
// Menu 4: Tap the Bug Board
// ============================================================
void BugBoard::tap() {
    if (bugs.empty()) {
        cout << "No bugs on the board to move.\n";
        return;
    }

    int aliveCount = countAlive();
    if (aliveCount <= 1) {
        cout << "Only " << aliveCount << " bug(s) remaining. No movement needed.\n";
        return;
    }

    cout << "\n===== Tapping the Bug Board! =====\n";

    // Step 1: Pick one random alive bug to freeze (it does not move this tap)
    vector<Bug*> aliveBugs;
    for (Bug* b : bugs) {
        if (b->isAlive()) {
            aliveBugs.push_back(b);
        }
    }

    Bug* frozenBug = nullptr;
    if (!aliveBugs.empty()) {
        int frozenIndex = rand() % aliveBugs.size();
        frozenBug = aliveBugs[frozenIndex];
        cout << "Bug " << frozenBug->getId() << " is frozen and will not move this tap.\n";
    }

    // Step 2: All alive bugs move (except the frozen one)
    for (Bug* b : bugs) {
        if (b->isAlive() && b != frozenBug) {
            b->move();
        }
    }

    // Step 3: Bugs on same cell fight - 3-round paired fighting system
    vector<pair<int, int>> processedPositions;

    for (Bug* b : bugs) {
        if (!b->isAlive()) continue;

        pair<int, int> pos = b->getPosition();

        // Check if we already processed this position
        bool alreadyProcessed = false;
        for (const auto& p : processedPositions) {
            if (p == pos) {
                alreadyProcessed = true;
                break;
            }
        }
        if (alreadyProcessed) continue;

        processedPositions.push_back(pos);

        // Get all alive bugs at this position
        vector<Bug*> bugsAtPos = getBugsAtPosition(pos);

        if (bugsAtPos.size() <= 1) continue;

        cout << "Fight at (" << pos.first << "," << pos.second << ")! ";
        cout << bugsAtPos.size() << " bugs present.\n";

        // Pair off bugs for 3-round fights
        // Strategy: pair bugs by index (0 vs 1, 2 vs 3, etc.)
        // If odd number, the last bug doesn't fight and remains unscathed
        for (size_t i = 0; i + 1 < bugsAtPos.size(); i += 2) {
            Bug* bugA = bugsAtPos[i];
            Bug* bugB = bugsAtPos[i + 1];

            cout << "  Bug " << bugA->getId() << " vs Bug " << bugB->getId() << "!\n";

            // 3 rounds of fighting (stop early if one bug dies)
            for (int round = 1; round <= 3; ++round) {
                // Check if either bug is already dead
                if (!bugA->isAlive() || !bugB->isAlive()) break;

                // Both bugs incur random damage between 0-5
                int damageA = rand() % 6;  // 0-5
                int damageB = rand() % 6;  // 0-5

                int healthBeforeA = bugA->getHealth();
                int healthBeforeB = bugB->getHealth();

                bugA->takeDamage(damageA);
                bugB->takeDamage(damageB);

                cout << "    Round " << round << ": Bug " << bugA->getId()
                     << " takes " << damageA << " damage (health " << healthBeforeA << "->" << bugA->getHealth() << ")"
                     << ", Bug " << bugB->getId()
                     << " takes " << damageB << " damage (health " << healthBeforeB << "->" << bugB->getHealth() << ")\n";
            }

            // Determine winner/loser
            if (!bugA->isAlive() && bugB->isAlive()) {
                cout << "    Result: Bug " << bugB->getId() << " wins! Bug " << bugA->getId() << " is dead.\n";
                bugA->setEatenBy(bugB->getId());
            } else if (!bugB->isAlive() && bugA->isAlive()) {
                cout << "    Result: Bug " << bugA->getId() << " wins! Bug " << bugB->getId() << " is dead.\n";
                bugB->setEatenBy(bugA->getId());
            } else if (!bugA->isAlive() && !bugB->isAlive()) {
                cout << "    Result: Both bugs died!\n";
            } else {
                cout << "    Result: Both bugs survived with health " << bugA->getHealth() << " and " << bugB->getHealth() << "\n";
            }
        }

        if (bugsAtPos.size() % 2 != 0) {
            cout << "  Bug " << bugsAtPos.back()->getId() << " has no opponent and remains unscathed.\n";
        }
        cout << "\n";
    }

    int remaining = countAlive();
    cout << "Bugs remaining: " << remaining << "\n";

    if (remaining == 1) {
        cout << "\n*** END POINT REACHED: Only one bug remains! ***\n";
        for (Bug* b : bugs) {
            if (b->isAlive()) {
                cout << "Winner: " << b->toString() << "\n";
            }
        }
    }
}

// ============================================================
// Menu 5: Display Life History of all Bugs
// ============================================================
void BugBoard::displayLifeHistory() const {
    if (bugs.empty()) {
        cout << "No bugs on the board.\n";
        return;
    }

    cout << "\n===== Life History of All Bugs =====\n";
    for (const Bug* b : bugs) {
        cout << b->lifeHistoryToString() << "\n";
    }
}

// ============================================================
// Menu 6: Display all Cells listing their Bugs
// ============================================================
void BugBoard::displayAllCells() const {
    cout << "\n===== Bug Board (10x10) =====\n";
    cout << "     ";
    for (int x = 0; x < 10; ++x) {
        cout << "  Col" << x << "   ";
    }
    cout << "\n";

    for (int y = 0; y < 10; ++y) {
        cout << "Row " << setw(2) << y << " ";
        for (int x = 0; x < 10; ++x) {
            pair<int, int> pos(x, y);
            vector<Bug*> bugsHere;
            for (Bug* b : bugs) {
                if (b->isAlive() && b->getPosition() == pos) {
                    bugsHere.push_back(b);
                }
            }

            if (bugsHere.empty()) {
                cout << " [  --  ] ";
            } else {
                string cellContent = "[";
                for (size_t i = 0; i < bugsHere.size(); ++i) {
                    cellContent += to_string(bugsHere[i]->getId());
                    if (i < bugsHere.size() - 1) cellContent += ",";
                }
                cellContent += "]";
                cout << " " << setw(7) << left << cellContent << " ";
            }
        }
        cout << "\n";
    }

    // Also list all cells with bugs in detail (spec format)
    cout << "\n===== Cells with Bugs (Detailed) =====\n";
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            pair<int, int> pos(x, y);
            vector<Bug*> bugsHere;
            for (Bug* b : bugs) {
                if (b->isAlive() && b->getPosition() == pos) {
                    bugsHere.push_back(b);
                }
            }

            cout << "(" << x << "," << y << "): ";
            if (bugsHere.empty()) {
                cout << "empty";
            } else {
                for (size_t i = 0; i < bugsHere.size(); ++i) {
                    if (i > 0) cout << ", ";
                    cout << bugsHere[i]->getType() << " " << bugsHere[i]->getId();
                }
            }
            cout << "\n";
        }
    }
}

// ============================================================
// Menu 7: Run simulation (auto-tap every second)
// ============================================================
void BugBoard::runSimulation() {
    int aliveCount = countAlive();
    if (aliveCount <= 1) {
        cout << "Need at least 2 bugs to run simulation. Current alive: " << aliveCount << "\n";
        return;
    }

    cout << "\n===== Running Simulation (Tapping every second) =====\n";
    cout << "Press Ctrl+C to stop...\n\n";

    int tapCount = 0;
    while (countAlive() > 1) {
        tapCount++;
        cout << "--- Tap #" << tapCount << " ---\n";
        tap();
        cout << "\n";

        if (countAlive() <= 1) {
            cout << "Simulation complete after " << tapCount << " taps.\n";
            break;
        }

        this_thread::sleep_for(chrono::seconds(1));
    }
}

// ============================================================
// Menu 8: Write Life History to file
// ============================================================
void BugBoard::writeLifeHistoryToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "' for writing.\n";
        return;
    }

    for (const Bug* b : bugs) {
        file << b->lifeHistoryToString() << "\n";
    }

    file.close();
    cout << "Life history written to '" << filename << "'\n";
}

// ============================================================
// Helper: count alive bugs
// ============================================================
int BugBoard::countAlive() const {
    int count = 0;
    for (const Bug* b : bugs) {
        if (b->isAlive()) count++;
    }
    return count;
}

// ============================================================
// Check if board is initialized
// ============================================================
bool BugBoard::isInitialized() const {
    return !bugs.empty();
}
