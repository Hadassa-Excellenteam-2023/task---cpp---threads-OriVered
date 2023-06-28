# task---cpp---threads-OriVered

This is a simple multi-threading exercise that demonstrates the use of `std::thread` in C++. The application generates two producer threads and a consumer thread. The producer threads generate a sequence of numbers, based on their unique thread id, and then send these numbers to the consumer thread, which prints them. 

The program stops when all producer threads have finished producing numbers.

## How to run

1. run this with visual studio with c++ extenention


## Code Overview

* `main.cpp` - Contains the main function which starts the consumer thread first and then the producer threads.

* Producers:
 * Generate a unique value based on their thread ID.
 * Produce numbers until a flag, `lastMessage`, is true or `finished` flag is set. 
 * Each iteration, divide the current value by 10.0.
 * Set the `lastMessage` flag when the value is close to zero. 
 * Print their thread ID and the value sent.
 * Once done, decrement the `count` and if it reaches zero, set the `finished` flag.

* Consumer:
 * Wait until the `finished` flag is set.
 * Once the flag is set, print a message and then finishes.

The `std::mutex` and `std::condition_variable` are used to synchronize the producer and consumer threads.
