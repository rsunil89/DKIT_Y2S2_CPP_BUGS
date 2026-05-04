#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

// ============================================================
// Bug Types
// ============================================================
enum BugType { CRAWLER, HOPPER, FLYER };

string bugTypeToString(BugType t) {
    switch (t) {
        case CRAWLER: return "Crawler";
        case HOPPER:  return "Hopper";
        case FLYER:   return "Flyer";
        default:      return "Unknown";
    }
}

BugType stringToBugType(const string& s) {
    if (s == "Crawler" || s == "crawler") return CRAWLER;
    if (s == "Hopper" || s == "hopper")   return HOPPER;
    if (s == "Flyer" || s == "flyer")     return FLYER;
    return CRAWLER;
}

// ============================================================
// Direction
// ============================================================
enum Direction { NORTH, EAST, SOUTH, WEST, NONE };

string directionToString(Direction d) {
    switch (d) {
        case NORTH: return "North";
        case EAST:  return "East";
        case SOUTH: return "South";
        case WEST:  return "West";
        default:    return "None";
    }
}

Direction randomDirection() {
    return static_cast<Direction>(rand() % 4);
}

// ============================================================
// Position
// ============================================================
struct Position {
    int x, y;
    Position() : x(0), y(0) {}
    Position(int _x, int _y) : x(_x), y(_y) {}

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    string toString() const {
        return "(" + to_string(x) + "," + to_string(y) + ")";
    }
};

// ============================================================
// Bug Base Class
// ============================================================
class Bug {
protected:
    int id;
    BugType type;
    Position position;
    Direction direction;
    int size;
    bool alive;
    vector<Position> path;

public:
    Bug(int _id, BugType _type, Position _pos, int _size)
        : id(_id), type(_type), position(_pos), size(_size), alive(true), direction(NONE) {
        path.push_back(position);
    }

    virtual ~Bug() {}

    int getId() const { return id; }
    BugType getType() const { return type; }
    Position getPosition() const { return position; }
    int getSize() const { return size; }
    bool isAlive() const { return alive; }
    const vector<Position>& getPath() const { return path; }

    void kill() { alive = false; }

    void setPosition(const Position& p) {
        position = p;
        path.push_back(p);
    }

    // Virtual move method - each bug type implements its own behaviour
    virtual void move() = 0;

    string toString() const {
        return "ID: " + to_string(id)
            + " | Type: " + bugTypeToString(type)
            + " | Position: " + position.toString()
            + " | Size: " + to_string(size)
            + " | Status: " + (alive ? "Alive" : "Dead");
    }

    string lifeHistoryToString() const {
        string history = to_string(id) + " " + bugTypeToString(type) + " Path: ";
        for (size_t i = 0; i < path.size(); ++i) {
            history += path[i].toString();
            if (i < path.size() - 1) history += " -> ";
        }
        history += " | Size: " + to_string(size) + " | " + (alive ? "Alive" : "Dead");
        return history;
    }
};

// ============================================================
// Crawler - moves one cell in its current direction
// ============================================================
class Crawler : public Bug {
public:
    Crawler(int _id, Position _pos, int _size)
        : Bug(_id, CRAWLER, _pos, _size) {
        direction = randomDirection();
    }

    void move() override {
        if (!alive) return;

        // Pick a new random direction
        direction = randomDirection();

        Position newPos = position;
        switch (direction) {
            case NORTH: newPos.y = (newPos.y - 1 + 10) % 10; break;
            case EAST:  newPos.x = (newPos.x + 1) % 10;      break;
            case SOUTH: newPos.y = (newPos.y + 1) % 10;      break;
            case WEST:  newPos.x = (newPos.x - 1 + 10) % 10; break;
            default: break;
        }

        setPosition(newPos);
    }
};

// ============================================================
// Hopper - moves 2 cells in its current direction
// ============================================================
class Hopper : public Bug {
public:
    Hopper(int _id, Position _pos, int _size)
        : Bug(_id, HOPPER, _pos, _size) {
        direction = randomDirection();
    }

    void move() override {
        if (!alive) return;

        direction = randomDirection();

        Position newPos = position;
        switch (direction) {
            case NORTH: newPos.y = (newPos.y - 2 + 10) % 10; break;
            case EAST:  newPos.x = (newPos.x + 2) % 10;      break;
            case SOUTH: newPos.y = (newPos.y + 2) % 10;      break;
            case WEST:  newPos.x = (newPos.x - 2 + 10) % 10; break;
            default: break;
        }

        setPosition(newPos);
    }
};

// ============================================================
// Flyer - moves diagonally
// ============================================================
class Flyer : public Bug {
public:
    Flyer(int _id, Position _pos, int _size)
        : Bug(_id, FLYER, _pos, _size) {}

    void move() override {
        if (!alive) return;

        // Flyers move diagonally: random combination of N/S and E/W
        Direction vertDir = randomDirection(); // N or S
        Direction horizDir = randomDirection(); // E or W

        // Constrain vertical to N/S only
        if (vertDir == EAST || vertDir == WEST) vertDir = (rand() % 2 == 0) ? NORTH : SOUTH;
        // Constrain horizontal to E/W only
        if (horizDir == NORTH || horizDir == SOUTH) horizDir = (rand() % 2 == 0) ? EAST : WEST;

        Position newPos = position;

        switch (vertDir) {
            case NORTH: newPos.y = (newPos.y - 1 + 10) % 10; break;
            case SOUTH: newPos.y = (newPos.y + 1) % 10;      break;
            default: break;
        }
        switch (horizDir) {
            case EAST: newPos.x = (newPos.x + 1) % 10;       break;
            case WEST: newPos.x = (newPos.x - 1 + 10) % 10;  break;
            default: break;
        }

        setPosition(newPos);
    }
};

// ============================================================
// Bug Board - manages all bugs and the 10x10 grid
// ============================================================
class BugBoard {
private:
    vector<Bug*> bugs;
    int nextId;

    // Helper: get bugs at a specific position
    vector<Bug*> getBugsAtPosition(const Position& pos) {
        vector<Bug*> result;
        for (Bug* b : bugs) {
            if (b->isAlive() && b->getPosition() == pos) {
                result.push_back(b);
            }
        }
        return result;
    }

public:
    BugBoard() : nextId(1) {
        srand(static_cast<unsigned>(time(nullptr)));
    }

    ~BugBoard() {
        for (Bug* b : bugs) {
            delete b;
        }
        bugs.clear();
    }

    // ============================================================
    // Menu 1: Initialize Bug Board from file
    // ============================================================
    bool initializeFromFile(const string& filename) {
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
            string typeStr;
            int x, y, size;

            ss >> typeStr >> x >> y >> size;

            if (ss.fail()) {
                cout << "Warning: Invalid format on line " << lineNum << ": " << line << "\n";
                continue;
            }

            if (x < 0 || x > 9 || y < 0 || y > 9) {
                cout << "Warning: Position out of bounds on line " << lineNum << "\n";
                continue;
            }

            if (size <= 0) {
                cout << "Warning: Invalid size on line " << lineNum << "\n";
                continue;
            }

            BugType type = stringToBugType(typeStr);
            Position pos(x, y);
            Bug* bug = nullptr;

            switch (type) {
                case CRAWLER: bug = new Crawler(nextId, pos, size); break;
                case HOPPER:  bug = new Hopper(nextId, pos, size);  break;
                case FLYER:   bug = new Flyer(nextId, pos, size);   break;
            }

            if (bug) {
                bugs.push_back(bug);
                cout << "Loaded: " << bug->toString() << "\n";
                nextId++;
            }
        }

        file.close();
        cout << "\nLoaded " << bugs.size() << " bugs from file.\n";
        return true;
    }

    // ============================================================
    // Menu 2: Display all Bugs
    // ============================================================
    void displayAllBugs() const {
        if (bugs.empty()) {
            cout << "No bugs on the board.\n";
            return;
        }

        cout << "\n===== All Bugs =====\n";
        for (const Bug* b : bugs) {
            cout << b->toString() << "\n";
        }
        cout << "Total: " << bugs.size() << " bugs\n";
    }

    // ============================================================
    // Menu 3: Find a Bug by ID
    // ============================================================
    void findBug(int id) const {
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
    void tap() {
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

        // Step 1: All alive bugs move
        for (Bug* b : bugs) {
            if (b->isAlive()) {
                b->move();
            }
        }

        // Step 2: Bugs on same cell fight - biggest eats the rest
        // Track which positions have been processed
        vector<Position> processedPositions;

        for (Bug* b : bugs) {
            if (!b->isAlive()) continue;

            Position pos = b->getPosition();

            // Check if we already processed this position
            bool alreadyProcessed = false;
            for (const Position& p : processedPositions) {
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

            // Find the biggest bug
            Bug* biggest = bugsAtPos[0];
            for (Bug* candidate : bugsAtPos) {
                if (candidate->getSize() > biggest->getSize()) {
                    biggest = candidate;
                }
            }

            // Biggest bug eats all others
            cout << "Fight at " << pos.toString() << "! ";
            cout << "Bug " << biggest->getId() << " (size " << biggest->getSize() << ") eats: ";

            for (Bug* victim : bugsAtPos) {
                if (victim != biggest) {
                    cout << victim->getId() << " ";
                    biggest->getSize(); // Note: we could add sizes, but spec says "biggest eats all others"
                    victim->kill();
                }
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
    void displayLifeHistory() const {
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
    void displayAllCells() const {
        cout << "\n===== Bug Board (10x10) =====\n";
        cout << "     ";
        for (int x = 0; x < 10; ++x) {
            cout << "  Col" << x << "   ";
        }
        cout << "\n";

        for (int y = 0; y < 10; ++y) {
            cout << "Row " << setw(2) << y << " ";
            for (int x = 0; x < 10; ++x) {
                Position pos(x, y);
                vector<Bug*> bugsHere;
                for (const Bug* b : bugs) {
                    if (b->isAlive() && b->getPosition() == pos) {
                        bugsHere.push_back(const_cast<Bug*>(b));
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

        // Also list all cells with bugs in detail
        cout << "\n===== Cells with Bugs (Detailed) =====\n";
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                Position pos(x, y);
                vector<Bug*> bugsHere;
                for (const Bug* b : bugs) {
                    if (b->isAlive() && b->getPosition() == pos) {
                        bugsHere.push_back(const_cast<Bug*>(b));
                    }
                }

                if (!bugsHere.empty()) {
                    cout << "Cell " << pos.toString() << ": ";
                    for (size_t i = 0; i < bugsHere.size(); ++i) {
                        cout << bugsHere[i]->toString();
                        if (i < bugsHere.size() - 1) cout << " | ";
                    }
                    cout << "\n";
                }
            }
        }
    }

    // ============================================================
    // Menu 7: Run simulation (auto-tap every second)
    // ============================================================
    void runSimulation() {
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
    void writeLifeHistoryToFile(const string& filename) {
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
    int countAlive() const {
        int count = 0;
        for (const Bug* b : bugs) {
            if (b->isAlive()) count++;
        }
        return count;
    }

    // ============================================================
    // Check if board is initialized
    // ============================================================
    bool isInitialized() const {
        return !bugs.empty();
    }
};

// ============================================================
// Main Program with Menu
// ============================================================
int main() {
    BugBoard board;
    int choice = 0;
    bool initialized = false;

    cout << "========================================\n";
    cout << "     BUG BOARD SIMULATION SYSTEM\n";
    cout << "========================================\n\n";

    do {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. Initialize Bug Board (load data from file)\n";
        cout << "2. Display all Bugs\n";
        cout << "3. Find a Bug (given an id)\n";
        cout << "4. Tap the Bug Board (causes move all, then fight/eat)\n";
        cout << "5. Display Life History of all Bugs (path taken)\n";
        cout << "6. Display all Cells listing their Bugs\n";
        cout << "7. Run simulation (generates a Tap every second)\n";
        cout << "8. Exit (write Life History of all Bugs to file)\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number (1-8).\n";
            continue;
        }

        switch (choice) {
            case 1: {
                string filename;
                cout << "Enter filename to load bugs from: ";
                cin >> filename;
                board.initializeFromFile(filename);
                initialized = board.isInitialized();
                break;
            }
            case 2: {
                if (!initialized) { cout << "Please initialize the board first (Option 1).\n"; break; }
                board.displayAllBugs();
                break;
            }
            case 3: {
                if (!initialized) { cout << "Please initialize the board first (Option 1).\n"; break; }
                int id;
                cout << "Enter Bug ID to find: ";
                cin >> id;
                board.findBug(id);
                break;
            }
            case 4: {
                if (!initialized) { cout << "Please initialize the board first (Option 1).\n"; break; }
                board.tap();
                break;
            }
            case 5: {
                if (!initialized) { cout << "Please initialize the board first (Option 1).\n"; break; }
                board.displayLifeHistory();
                break;
            }
            case 6: {
                if (!initialized) { cout << "Please initialize the board first (Option 1).\n"; break; }
                board.displayAllCells();
                break;
            }
            case 7: {
                if (!initialized) { cout << "Please initialize the board first (Option 1).\n"; break; }
                board.runSimulation();
                break;
            }
            case 8: {
                if (!initialized) { cout << "No data to save. Goodbye!\n"; break; }
                string filename;
                cout << "Enter filename to save life history: ";
                cin >> filename;
                board.writeLifeHistoryToFile(filename);
                cout << "Exiting program. Goodbye!\n";
                break;
            }
            default: {
                cout << "Invalid choice. Please enter a number between 1 and 8.\n";
                break;
            }
        }
    } while (choice != 8);

    return 0;
}
