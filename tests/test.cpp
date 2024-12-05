#include <gtest/gtest.h>

#include "src/TTaskScheduler.h"
#include <math.h>
#include <string>


TEST(TestSuite, ExampleTest) {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;},
                             a, c);
    auto id2 = scheduler.add([](float b, float v){return b * b + v;},
                             b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);},
                             b, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);},
                             b, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v){return v/(2*a);},
                             a, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v){return v/(2*a);},
                             a, scheduler.getFutureResult<float>(id4));
    scheduler.executeAll();

    ASSERT_EQ(2.0, scheduler.getResult<float>(id5));
    ASSERT_EQ(0.0, scheduler.getResult<float>(id6));
}


TEST (TestSuite, FactorialTest) {
    TTaskScheduler scheduler;

    int index = 1;
    auto func = [](int n, int last){return last * n;};
    auto id1 = scheduler.add(func,index++, 1);
    auto id2 = scheduler.add(func,index++, scheduler.getFutureResult<int>(id1));
    auto id3 = scheduler.add(func,index++, scheduler.getFutureResult<int>(id2));
    auto id4 = scheduler.add(func,index++, scheduler.getFutureResult<int>(id3));
    auto id5 = scheduler.add(func,index++, scheduler.getFutureResult<int>(id4));
    auto id6 = scheduler.add(func,index++, scheduler.getFutureResult<int>(id5));
    auto id7 = scheduler.add(func,index++, scheduler.getFutureResult<int>(id6));
    auto id8 = scheduler.add(func,index++, scheduler.getFutureResult<int>(id7));
    auto id9 = scheduler.add(func,index++, scheduler.getFutureResult<int>(id8));

    ASSERT_EQ(1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9, scheduler.getResult<int>(id9));
}


TEST (TestSuite, AllArgCntAndTYpesTest) {
    TTaskScheduler scheduler;

    auto f_sum = [](int a, int b) {return a + b;};

    int n = 22;
    auto id1 = scheduler.add([](int a) {return a * a;}, n);
    auto id2 = scheduler.add([](int a){return 2 * a;}, scheduler.getFutureResult<int>(id1));
    auto id3 = scheduler.add(f_sum, n, n);
    auto id4 = scheduler.add(f_sum, scheduler.getFutureResult<int>(id2), scheduler.getFutureResult<int>(id3));
    auto id5 = scheduler.add(f_sum, n, scheduler.getFutureResult<int>(id4));
    auto id6 = scheduler.add(f_sum, scheduler.getFutureResult<int>(id5), n);

    ASSERT_EQ(scheduler.getResult<int>(id6), 1056);
}