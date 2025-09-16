#include <gtest/gtest.h>
#include "ticktoucan.h"

static void setTrue(void* arg) {
    bool* flag = static_cast<bool*>(arg);
    *flag = true;
}

static void incrementVal(void* arg) {
    int* val = static_cast<int*>(arg);
    *val = *val + 1;
}

TEST(SetupTest, CanScheduleSingleTask) {
    TickToucan& scheduler = TickToucan::instance();
    bool taskExecuted = false;

    scheduler.init(20); // Initialize with 1ms tick

    scheduler.scheduleAt(5, setTrue, &taskExecuted); // 100ms delay

    auto start = std::chrono::steady_clock::now();
    
    while(!taskExecuted)
    {
        scheduler.dispatch();        
    }

    auto end = std::chrono::steady_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "[CanScheduleSingleTask] Duration: " << duration_ms << " ms" << std::endl;

    EXPECT_TRUE(taskExecuted);
}

TEST(SetupTest, ExecutesMultipleTasksInOrder) {
    TickToucan& scheduler = TickToucan::instance();
    bool taskExecuted1 = false;
    bool taskExecuted2 = false;

    bool print = false;

    scheduler.init(20); // Initialize with 1ms tick

    scheduler.scheduleAt(5, setTrue, &taskExecuted1); // 100ms delay

    scheduler.scheduleAt(2, setTrue, &taskExecuted2); // 100ms delay


    auto start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end1, end2;
    int64_t dur1, dur2;

    while(!(taskExecuted1 && taskExecuted2))
    {
        scheduler.dispatch();
        if(taskExecuted2 && (!print))
        {
            print = true;
            end2 = std::chrono::steady_clock::now();
            dur2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start).count();
            std::cout << "[ExecutesMultipleTasksInOrder] Duration until task 2: " << dur2 << " ms" << std::endl;
        }       
    }

    end1 = std::chrono::steady_clock::now();
    dur1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start).count();
    std::cout << "[ExecutesMultipleTasksInOrder] Duration until task 1: " << dur1 << " ms" << std::endl;
    EXPECT_TRUE(taskExecuted1);
    EXPECT_TRUE(taskExecuted2);

    EXPECT_GE(dur1, 100);
    EXPECT_LE(dur1, 110);

    EXPECT_GE(dur2, 40);
    EXPECT_LE(dur2, 50);
}

TEST(SetupTest, PeridicTaskRuns) {
    TickToucan& scheduler = TickToucan::instance();

    int val = 0;
    int prev_val = 0;
    
    scheduler.init(20); // Initialize with 1ms tick

    scheduler.scheduleEvery(2, incrementVal, &val, 0); // 100ms delay

    auto start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end;
    int64_t dur1;

    while(val < 10)
    {
        prev_val = val;
        scheduler.dispatch();
        if(val != prev_val)
        {
            if(val <= 2)
            {
                start = std::chrono::steady_clock::now();
                continue; // Skip first two runs to avoid timing inaccuracies                
            }
            end = std::chrono::steady_clock::now();
            dur1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "[PeridicTaskRuns] Value: " << val << " at " << dur1 << " ms" << std::endl;
            start = end;
            EXPECT_GE(dur1, 38);
            EXPECT_LE(dur1, 42);

        } 
    }


}