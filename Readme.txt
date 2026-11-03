# Elevator Simulation in C++

## Project Overview
This project is an interactive, simulation of a multi-floor elevator system. The simulation modeles elevator movement, handles simultaneous passenger requests, 
manages doors, and utilizes custom dynamic memory management to make sure it has a clean system architecture.



## Author & Team Information
* **Team Members:** 1 (Solo Project) becasue I could not find soemon to work with
* **Student Name:** Eric Orea
* **Course:** C++ Programming
* **Role & Contributions:** Designed, implemented, and tested all classes (`Elevator`, `Building`), algorithms, dynamic memory structures, console UI, and project documentation.



## Implemented Features
1. **Configurable Building & Elevators:**
   * Dynamic initialization of floor count (1–10) and elevator count.
2. **Elevator Dynamics & State Machine:**
   * Traversal states: `UP`, `DOWN`, and `IDLE`.
   * Door mechanics: Doors open (`[OPEN]`) at target pickup and drop-off floors, and close (`[CLOSED]`) before moving.
3. **LOOK Scheduling & Dispatching:**
   * Moves continuously in the active direction to service all pending calls before switching directions.
   * Proximity-based dispatching algorithm selects the closest idle elevator for new requests.
4. **Interactive Console UI:**
   * Step-by-step state display reporting real-time floor position, active direction, and door status.
   * Runtime input menu allowing users to add requests or advance time.
5. **Memory Management **
   * Uses dynamically allocated arrays (`new[]` / `delete[]`) rather than STL containers.
   * Follows the **Rule of Three** in the `Elevator` class with custom copy constructors, assignment operators, and destructors to prevent memory leaks and dangling pointers.



