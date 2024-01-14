#define BS_THREAD_POOL_ENABLE_PAUSE
#include "./include/BS_thread_pool.hpp"       // BS::thread_pool
#include "./include/BS_thread_pool_utils.hpp" // BS::synced_stream
#include <chrono>                   // std::chrono
#include <thread>                   // std::this_thread

BS::synced_stream sync_out;
BS::thread_pool pool(4);

void sleep_half_second(const int i)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    sync_out.println("Task ", i, " done.");
}

void check_if_paused()
{
    if (pool.is_paused())
        sync_out.println("Pool paused.");
    else
        sync_out.println("Pool unpaused.");
}

int main()
{
    pool.detach_sequence(0, 8, sleep_half_second);
    sync_out.println("Submitted 8 tasks. Waiting for them to complete.");
    pool.wait();
    pool.detach_sequence(8, 20, sleep_half_second);
    sync_out.println("Submitted 12 more tasks.");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    pool.pause();
    check_if_paused();
    sync_out.println("Waiting for the ", pool.get_tasks_running(), " running tasks to complete.");
    pool.wait();
    sync_out.println("All running tasks completed. ", pool.get_tasks_queued(), " tasks still queued.");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    sync_out.println("Still paused...");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    sync_out.println("Still paused...");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    pool.unpause();
    check_if_paused();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    sync_out.println("Waiting for the remaining ", pool.get_tasks_total(), " tasks (", pool.get_tasks_running(), " running and ", pool.get_tasks_queued(), " queued) to complete.");
    pool.wait();
    sync_out.println("All tasks completed.");
}