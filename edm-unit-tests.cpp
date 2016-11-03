/*
 * A simple Test driver C++ program to test
 * detection
 *
 * Author: Yash Gupta
 * Copyright: Yash Gupta, SSRC - UC Santa Cruz
 */

#include <cmath>
#include "EDM.h"
#include <iostream>

using namespace std;

bool test_breakpoint() {
	const size_t sigma = 24;
	const size_t n = 100;
    const int tree_depth = (int)std::ceil(std::log(n));
	double d[n];

	for (size_t i = 0; i < n/2; ++i)
		d[i] = 30;
	for (size_t i = n/2; i < n; ++i)
		d[i] = 10;

	Breakpoint bp(d, n, sigma, tree_depth);
    cout << "Breakpoint: " << bp.getBreakpointLocation() << endl;
    return true;
}

/* Test Driver */
int main()
{
    IntervalTree test(true, 2);
    test.add(0.09);
    test.add(0.42);
    test.add(0.99);
    test.add(0.3);
	test.add(1.0);
    test.displayTree();
    std::cout << std::endl;

    std::cout << "Approximate Median: " << test.getApproxMedian() << std::endl;

    test_breakpoint();

    return 0;
}
