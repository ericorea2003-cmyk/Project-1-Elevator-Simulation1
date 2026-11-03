/**
 * Elevator Simulation
 * Description:  implements a simulation of an elevator system using C++.
 *
 *@Author: Eric Orea
 * @Date: 10/20/2026
 */

#include <iostream>
#include <string>
#include <cmath>

// Status representations using basic enums
enum Direction { UP, DOWN, IDLE };
enum DoorState { CLOSED, OPEN };

// this Class represents an Elevator with dynamic stop management
class Elevator {
private:
    int id;
    int currentFloor;
    int minFloor;
    int maxFloor;
    Direction direction;
    DoorState door;

    // Dynamically allocated boolean array tracking stops for each floor
    // Index i corresponds to floor (i + 1)
    bool* floorStops;
    int totalFloors;

public:
    // Constructor allocats the dynamic memory
    Elevator(int id = 1, int minF = 1, int maxF = 10) {
        this->id = id;
        this->minFloor = minF;
        this->maxFloor = maxF;
        this->currentFloor = minF;
        this->direction = IDLE;
        this->door = CLOSED;
        this->totalFloors = maxF - minF + 1;

        this->floorStops = new bool[this->totalFloors];
        for (int i = 0; i < this->totalFloors; ++i) {
            this->floorStops[i] = false;
        }
    }

    //  manually manages the memory   
    ~Elevator() {
        delete[] floorStops;
        floorStops = nullptr;
    }

    // This copies the Constructor following the Rule of Three
    Elevator(const Elevator& other) {
        this->id = other.id;
        this->currentFloor = other.currentFloor;
        this->minFloor = other.minFloor;
        this->maxFloor = other.maxFloor;
        this->direction = other.direction;
        this->door = other.door;
        this->totalFloors = other.totalFloors;

        this->floorStops = new bool[this->totalFloors];
        for (int i = 0; i < this->totalFloors; ++i) {
            this->floorStops[i] = other.floorStops[i];
        }
    }

    // This copies the Assignments Operator following the Rule of Three
    Elevator& operator=(const Elevator& other) {
        if (this != &other) {
            delete[] this->floorStops;

            this->id = other.id;
            this->currentFloor = other.currentFloor;
            this->minFloor = other.minFloor;
            this->maxFloor = other.maxFloor;
            this->direction = other.direction;
            this->door = other.door;
            this->totalFloors = other.totalFloors;

            this->floorStops = new bool[this->totalFloors];
            for (int i = 0; i < this->totalFloors; ++i) {
                this->floorStops[i] = other.floorStops[i];
            }
        }
        return *this;
    }

    int GetId() const { return id; }
    int GetCurrentFloor() const { return currentFloor; }
    Direction GetDirection() const { return direction; }

    void AddStop(int floor) {
        if (floor < minFloor || floor > maxFloor) return;

        floorStops[floor - minFloor] = true;

        if (direction == IDLE) {
            if (floor > currentFloor) {
                direction = UP;
            }
            else if (floor < currentFloor) {
                direction = DOWN;
            }
            else {
                door = OPEN;
                floorStops[floor - minFloor] = false;
            }
        }
    }

    bool HasStopsAbove() const {
        for (int f = currentFloor + 1; f <= maxFloor; ++f) {
            if (floorStops[f - minFloor]) return true;
        }
        return false;
    }

    bool HasStopsBelow() const {
        for (int f = minFloor; f < currentFloor; ++f) {
            if (floorStops[f - minFloor]) return true;
        }
        return false;
    }

    // This method moves the elevator by one step
    void Step() {
        // If the door was open at a stop, it closes it before moving
        if (door == OPEN) {
            door = CLOSED;
            return;
        }

        if (direction == UP) {
            if (HasStopsAbove()) {
                currentFloor++;
                if (floorStops[currentFloor - minFloor]) {
                    door = OPEN;
                    floorStops[currentFloor - minFloor] = false;
                }
            }
            else if (HasStopsBelow()) {
                direction = DOWN;
            }
            else {
                direction = IDLE;
            }
        }
        else if (direction == DOWN) {
            if (HasStopsBelow()) {
                currentFloor--;
                if (floorStops[currentFloor - minFloor]) {
                    door = OPEN;
                    floorStops[currentFloor - minFloor] = false;
                }
            }
            else if (HasStopsAbove()) {
                direction = UP;
            }
            else {
                direction = IDLE;
            }
        }
        else {
            // Idle state: this determines if any stops are waiting
            if (HasStopsAbove()) direction = UP;
            else if (HasStopsBelow()) direction = DOWN;
        }
    }

    void Display() const {
        std::string dirText = (direction == UP) ? "UP" : (direction == DOWN ? "DOWN" : "IDLE");
        std::string doorText = (door == OPEN) ? "[OPEN]" : "[CLOSED]";

        std::cout << "Elevator " << id << " | Floor: " << currentFloor
            << " | Direction: " << dirText << " | Doors: " << doorText << "\n";
    }
};

// This Building class manages an arrays and the dynamics allocated to the elevators
class Building {
private:
    int totalFloors;
    int numElevators;
    Elevator* elevatorList; // Pointer to a dynamically allocated array of Elevators

public:
    Building(int floors, int elevatorsCount) {
        totalFloors = floors;
        numElevators = elevatorsCount;
        elevatorList = new Elevator[numElevators];

        // Initializes each elevator with a unique ID
        for (int i = 0; i < numElevators; ++i) {
            elevatorList[i] = Elevator(i + 1, 1, totalFloors);
        }
    }

    ~Building() {
        delete[] elevatorList;
        elevatorList = nullptr;
    }

    void RequestElevator(int startFloor, int destFloor) {
        int bestIndex = 0;
        int minDistance = totalFloors + 1;

        // Dispatch algorithm: chooses the closest elevator using pointers
        for (int i = 0; i < numElevators; ++i) {
            int dist = std::abs(elevatorList[i].GetCurrentFloor() - startFloor);
            if (elevatorList[i].GetDirection() == IDLE && dist < minDistance) {
                minDistance = dist;
                bestIndex = i;
            }
        }

        elevatorList[bestIndex].AddStop(startFloor);
        elevatorList[bestIndex].AddStop(destFloor);

        std::cout << ">> Request registered: assigned to Elevator "
            << elevatorList[bestIndex].GetId() << "\n";
    }

    void AdvanceSimulation() {
        for (int i = 0; i < numElevators; ++i) {
            elevatorList[i].Step();
        }
    }

    void DisplayStatus() const {
        std::cout << "\n---------------- Building Status ----------------\n";
        for (int i = 0; i < numElevators; ++i) {
            elevatorList[i].Display();
        }
        std::cout << "-------------------------------------------------\n";
    }

    int GetTotalFloors() const { return totalFloors; }
};

int main() {
    int totalFloors = 10;
    int numElevators = 2;

    // Allocates the Buildings object dynamically or on a stack
    Building campusBuilding(totalFloors, numElevators);

    int userChoice = 0;

    while (userChoice != 3) {
        campusBuilding.DisplayStatus();
        std::cout << "\nMenu Options:\n";
        std::cout << "1. Advance 1 step\n";
        std::cout << "2. Create floor request\n";
        std::cout << "3. Quit\n";
        std::cout << "Select an option: ";

        if (!(std::cin >> userChoice)) {
            std::cin.clear();
            while (std::cin.get() != '\n'); // Clears stream
            continue;
        }

        if (userChoice == 1) {
            campusBuilding.AdvanceSimulation();
        }
        else if (userChoice == 2) {
            int fromFloor = 0;
            int toFloor = 0;

            std::cout << "Enter current floor (1-" << totalFloors << "): ";
            std::cin >> fromFloor;
            std::cout << "Enter target floor (1-" << totalFloors << "): ";
            std::cin >> toFloor;

            if (fromFloor >= 1 && fromFloor <= totalFloors &&
                toFloor >= 1 && toFloor <= totalFloors && fromFloor != toFloor) {
                campusBuilding.RequestElevator(fromFloor, toFloor);
            }
            else {
                std::cout << "Invalid floor numbers. Try again.\n";
            }
        }
    }

    std::cout << "Simulation finished.\n";
    return 0;
}