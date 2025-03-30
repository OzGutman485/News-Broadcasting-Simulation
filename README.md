# News Broadcasting System

## Project Overview
This project simulates a news broadcasting system using concurrent programming techniques and multithreading in C++. It demonstrates key operating system concepts including producer-consumer patterns, thread synchronization, mutual exclusion, and bounded/unbounded buffers.

## Course Information
- **Course**: Operating Systems
- **Institution**: Bar-Ilan University
- **Department**: Computer Science

## Project Architecture

The system simulates a news broadcasting infrastructure with the following components:

1. **Producers**: Generate news items of different categories (SPORTS, NEWS, WEATHER)
2. **Dispatcher**: Routes incoming news items to appropriate category queues
3. **Co-Editors**: Process news items from specific categories
4. **Screen Manager**: Displays processed news items to the console

### Component Communication
Components communicate through thread-safe buffer implementations:
- **BoundedBuffer**: Fixed-capacity queue with blocking behavior when full
- **UnboundedBuffer**: Unlimited-capacity queue for final news processing

## Key Classes

- **Producer**: Generates news items and pushes them to their dedicated queue
- **BoundedBuffer**: Synchronized fixed-size buffer with insert/remove operations
- **UnboundedBuffer**: Synchronized unlimited-size buffer for final stage processing
- **Dispatcher**: Routes news items from producer queues to category-specific queues

## Implementation Details

### Synchronization Mechanisms
- **Mutexes**: Protect critical sections in buffer implementations
- **Condition Variables**: Enable blocking behavior for buffer operations
- **Thread Coordination**: Uses "DONE" sentinel values to signal completion

### Thread Safety
All queue implementations (BoundedBuffer and UnboundedBuffer) are thread-safe through the use of:
- Mutex locks for exclusive access
- Condition variables for thread waiting/signaling
- Atomic counters for shared state

## Configuration

The system is configurable through a configuration file, which specifies:
- Number of producers
- Number of products per producer
- Queue sizes for producers and co-editors

Example configuration file format:
```
PRODUCER 1
10
queue size = 5
PRODUCER 2
15
queue size = 7
Co-Editor queue size = 10
```

## Building and Running

### Prerequisites
- C++11 or later
- pthread library
- Standard C++ libraries

### Compilation
cd src dir in the project
Use the provided Makefile to compile the project:
```
make
```

This will compile the project using g++ with C++11 standard and create an executable named `ex3.out`.

### Execution
Run the compiled binary with a configuration file:
```
./ex3.out config.txt
```

Where `config.txt` is your configuration file containing producer and co-editor settings.

### Cleaning Up
To clean up object files and the executable:
```
make clean
```

## Project Files

- **main.cpp**: Entry point and main coordination logic
- **BoundedBuffer.h/cpp**: Implementation of bounded buffer with synchronization
- **Unbounded_Buffer.cpp**: Implementation of unbounded buffer with synchronization
- **Producer.cpp**: News item generation logic
- **Dispatcher.cpp**: News routing implementation
- **makefile**: Build configuration for compiling the project

## Learning Outcomes

This project demonstrates fundamental operating system concepts:
- Concurrent programming and multithreading
- Thread synchronization with mutexes and condition variables
- Producer-consumer pattern implementation
- Resource allocation and deadlock prevention
- Bounded and unbounded buffer implementations
