#include <iostream>

#include "test.h"


// Helpers
// ===========================================================
void highest_prio()
{
    std::cout << "highest_prio running...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "highest_prio done!\n";
}

void high_prio()
{
    std::cout << "high_prio running...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "high_prio done!\n";
}

void normal_prio()
{
    std::cout << "normal_prio running...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "normal_prio done!\n";
}

void low_prio()
{
    std::cout << "low_prio running...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "low_prio done!\n";
}

void lowest_prio()
{
    std::cout << "lowest_prio running...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "lowest_prio done!\n";
}

void foo_3_except_nok()
{
    std::cout << "foo_3_except_nok running...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    throw -3;
}

void foo_3_except_ok()
{
    std::cout << "foo_3_except_ok running...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    throw std::out_of_range("Out of range exception");
}

// Tests
// ===========================================================
void test_1()
{
    thread_pool pool(1);

    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::highest, highest_prio);
}

void test_2()
{
    thread_pool pool;

    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::highest, highest_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::low, low_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::lowest, lowest_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::lowest, lowest_prio);
    pool.do_job(priority::high, high_prio);
}

void test_3()
{
    thread_pool pool(2);
    std::cout << "Pool has " << pool.thread_count() << " threads\n";

    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::highest, highest_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::low, low_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::lowest, lowest_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::lowest, lowest_prio);
    pool.do_job(priority::high, high_prio);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool.restart(5);
    std::cout << "Pool has " << pool.thread_count() << " threads\n";
}

void test_4()
{
    thread_pool pool(2);

    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::highest, highest_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::low, low_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::normal, normal_prio);
    
    pool.pause();
    std::cout << "Pool paused\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::lowest, lowest_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::lowest, lowest_prio);
    pool.do_job(priority::high, high_prio);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Pool resumed\n";
    pool.resume();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Pool destroyed\n";
}

void test_5()
{
    thread_pool pool(2);

    pool.do_job(priority::normal, foo_3_except_nok);
    pool.do_job(priority::high, foo_3_except_ok);
    pool.do_job(priority::highest, foo_3_except_nok);
    pool.do_job(priority::normal, foo_3_except_ok);
    pool.do_job(priority::low, foo_3_except_nok);
    pool.do_job(priority::normal, foo_3_except_ok);
    pool.do_job(priority::normal, foo_3_except_nok);
    pool.do_job(priority::high, foo_3_except_nok);
    pool.do_job(priority::lowest, foo_3_except_ok);
    pool.do_job(priority::normal, foo_3_except_nok);
    pool.do_job(priority::lowest, foo_3_except_nok);
    pool.do_job(priority::high, foo_3_except_ok);

    std::this_thread::sleep_for(std::chrono::seconds(4));
    pool.force_shutdown();

    std::cout << "Pool shutdown with finished jobs " << pool.jobs_done() << '\n';

    pool.restart(5);
    std::cout << "Pool restarted\n";

    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::highest, highest_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::low, low_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::high, high_prio);
    pool.do_job(priority::lowest, lowest_prio);
    pool.do_job(priority::normal, normal_prio);
    pool.do_job(priority::lowest, lowest_prio);
    pool.do_job(priority::high, high_prio);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool.force_shutdown();

    std::cout << "Pool shutdown with finished jobs " << pool.jobs_done() << '\n';

    std::cout << "Pool destroyed\n";
}
