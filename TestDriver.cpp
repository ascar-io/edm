/*
 * A simple Test driver C++ program to test
 * detection
 *
 * Author: Yash Gupta
 * Copyright: Yash Gupta, SSRC - UC Santa Cruz
 */

#include "EDM.h"

/* Test Driver */
int
main(int argc, char *argv[])
{
    IntervalTree test(true, 2);
    test.add(0.09);
    test.add(0.42);
    test.add(0.99);
    test.add(0.3);
    test.displayTree();
    std::cout << std::endl;

    std::cout << "Approximate Median: " << test.getApproxMedian() << std::endl;

    double testVal[] = {1, 2, 3};
    Breakpoint testDriver(testVal, 3, 1, 2);

    return 0;
}
