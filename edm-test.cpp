/*
 * edm-test.cpp
 *
 * Copyright (c) 2016, University of California, Santa Cruz, CA, USA.
 * Created by Yash Gupta <ygupta@ucsc.edu>, Yan Li <yanli@ascar.io>,
 * Department of Computer Science, Baskin School of Engineering.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Storage Systems Research Center, the
 *       University of California, nor the names of its contributors
 *       may be used to endorse or promote products derived from this
 *       software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * REGENTS OF THE UNIVERSITY OF CALIFORNIA BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include "IntervalTree.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define check(x) { if (!(x)) { cerr << "Failed at line " << __LINE__ << ": " #x << endl; abort(); } }

/**
 * Return the median of v. v must be sorted.
 * @param v
 * @return
 */
double vector_median(const vector<double> &v) {
    size_t n = v.size();
    if (0 == n) return 0;
    if (n % 2 == 0) {
        // 0 1 2 3 4 5, n = 6
        return (v[n/2-1] + v[n/2]) / 2;
    } else {
        // 0 1 2 3 4 5 6, n = 7
        return v[n/2];
    }
}

/**
 * Read one line from an istream and use the data to test an interval tree
 * @param in an istream to read data from
 * @return true on success, false on failure
 */
bool test_interval_tree_using_samples(istream &in) {
    size_t sample_size;
    double exp_median;

    in >> sample_size;
    check (!in.fail() && in.peek() == ',')
    in.seekg(1, in.cur);    // skip ','
    in >> exp_median;
    check (!in.fail() && in.peek() == ',')
    in.seekg(1, in.cur);    // skip ','

    const int tree_depth = (int)std::ceil(std::log(sample_size));
    IntervalTree test(true, tree_depth);
    vector<double> samples(sample_size);
    for (size_t i = 0; i < sample_size; ++i) {
        double d;
        in >> d;
        check(!in.fail());
        if (i < sample_size - 1) {
            check(in.peek() == ',');
        } else {
            check(in.peek() == '\n' || in.peek() == EOF);
        }
        test.add(d);
        samples[i] = d;
        in.seekg(1, in.cur);    // skip ',' or '\n'
    }
    if (test.getApproxMedian() != exp_median) {
        sort(samples.begin(), samples.end());
        double real_median = vector_median(samples);
        double edm_error = abs(real_median - exp_median);
        double our_error = abs(real_median - test.getApproxMedian());
        cerr << "Failed test. Sample size: " << sample_size
             << ", expected value " << exp_median
             << ", actual value " << test.getApproxMedian()
             << ", real median " << real_median
             << ", EDM error " << edm_error
             << ", our error " << our_error << ". ";
        if (our_error < edm_error)
            cerr << "We won.";
        else
            cerr << "We lost.";
        cerr << endl;
        // test.displayTree();
        return false;
    }
    return true;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "Usage: $0 input_sample_csv" << endl;
        return 2;
    }

    ifstream fin(argv[1]);
    string s; getline(fin, s); // skip the header line

    int failed_cases = 0;
    do {
        if (!test_interval_tree_using_samples(fin))
            ++failed_cases;
    } while (!fin.eof());

    return failed_cases;
}
