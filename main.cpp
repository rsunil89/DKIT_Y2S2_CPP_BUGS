#include <iostream>
#include "BugBoard.h"

using namespace std;

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
