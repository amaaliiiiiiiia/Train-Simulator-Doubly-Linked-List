Train Simulator (Doubly Linked List & Task Queue)
This project implements a train management system using dynamic data structures and a command queue for task execution.

## Data Structures (tren.h file)
I have defined two main structures to represent the train:

Carriage: Contains 3 arguments: the carriage name (char), a pointer to the previous carriage, and a pointer to the next one.
Train: Contains 2 arguments: a pointer to the Locomotive (sentinel node) and a pointer to the Mechanic (current position indicator).

## Movement and Modification Functions
### Movement
MOVE_LEFT: If the mechanic is in the first carriage (locomotive is to the left), they move to the last carriage. Otherwise, they move one position to the left.
MOVE_RIGHT: If the mechanic is in the last carriage, I allocate memory for a new carriage named "#", connect it to the train, and move the mechanic there. Otherwise, the mechanic moves to the right.

### Editing
WRITE: Changes the name of the current carriage where the mechanic is located to the new character received as a parameter.
CLEAR_CELL: Removes the current carriage. If the train has only one carriage, it is reset to "#". If there are multiple carriages, the connections are re-established as if the current carriage didn't exist, and the mechanic moves one position to the left.
CLEAR_ALL: To use the CLEAR_CELL logic, I start from the last carriage. As long as there are at least 2 carriages, I remove them one by one. Finally, the remaining carriage is renamed to "#" using the WRITE function.
INSERT_LEFT / INSERT_RIGHT: Allocates memory for a new carriage with a specific character. It handles the re-linking with adjacent carriages and moves the mechanic into the newly created carriage (if INSERT_LEFT is called at the first carriage, it returns an "ERROR").

## Search Logic
SEARCH: Starts from the mechanic and traverses the train circularly. If string S is found, the mechanic is moved to the starting position of the match.
SEARCH_RIGHT: Searches for string S to the right, up to the last carriage. If found, the mechanic moves to the last character of the match.
SEARCH_LEFT: Searches for string S to the left, up to the first carriage. If found, the mechanic moves to the last character of the match.
In all cases, if the string is not found, an "ERROR" message is printed to the output file.

## Command Management (tema1.c file)
### Queue System
I defined a QueueNode (command name and next pointer) and a Queue (front, rear, and length). I implemented several helper functions:
enqueue / dequeue: Standard operations for the back/front of the queue.
enqueue2 / dequeue2: Specialized functions used for the SWITCH command to insert/remove from the opposite ends.

### Main Execution Flow
Initialization: Memory is allocated for the train, locomotive, and the initial "#" carriage. Files tema1.in and tema1.out are opened.
Reading: The program reads the number of commands and processes the file line by line.
SWITCH Logic: A variable s (0 or 1) tracks the switch state. If s=1, new commands are added to the front of the queue (enqueue2).
EXECUTE: This command picks the next task from the queue (based on the s state), runs the corresponding function from tren.h, and frees the command memory.

Instant Commands: SHOW and SHOW_CURRENT are not added to the queue; they are executed immediately when encountered.

Cleanup: At the end, the program deallocates all memory (carriages, locomotive, queue) and closes the files.
