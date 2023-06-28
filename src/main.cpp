#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

std::mutex mtx;  // Mutex to protect shared resources
std::condition_variable cv;  // Condition variable for synchronization
bool finished = false;  // Flag to indicate if all producers have finished
int count = 0;  // Count of active producer threads

void producer()
{
    // Generate a unique value based on the thread ID
    double value = static_cast<double>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    bool lastMessage = false;

    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << value << " :sent ." << std::this_thread::get_id() << std::endl;
        count++;

        if (count == 2)
            lastMessage = true;  // Set lastMessage flag when the second producer thread starts
    }

    // Continue producing until lastMessage is true or finished flag is set
    while (!lastMessage && !finished)
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            value = value / 10.0;
            std::cout << std::fixed << std::setprecision(9) << value << " :sent ." << std::this_thread::get_id() << std::endl;
        }

        if (value < 0.000000001)  // Check if value is close to zero
            lastMessage = true;  // Set lastMessage flag when value is close to zero

        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Sleep for 500 milliseconds
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << std::this_thread::get_id() << " finished." << std::endl;
        count--;

        if (count == 0)
            finished = true;  // Set finished flag when all producer threads have finished
    }

    cv.notify_one();  // Notify the consumer thread that producers have finished
}

void consumer()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return finished; });  // Wait for the finished flag to be set

    std::cout << "Consumer finished." << std::endl;
}

int main()
{
    std::thread consumerThread(consumer);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Ensure consumer starts first

    std::vector<std::thread> producerThreads;
    producerThreads.emplace_back(producer);
    producerThreads.emplace_back(producer);

    for (auto& thread : producerThreads) {
        try {
            thread.join();  // Wait for each producer thread to finish
        }
        catch (const std::exception& e) {
            std::cout << "Exception caught in producer thread: " << e.what() << std::endl;
        }
    }

    consumerThread.join();  // Wait for the consumer thread to finish

    return 0;
}
