/*
 * This File declares our class and structure
 * for the breakpoint detection algorithm
 *
 * Author: Yash Gupta
 * Copyright: Yash Gupta, SSRC - UC Santa Cruz
 */

#include "IntervalTree.h"

class Breakpoint {
    long tau;               // EDM Variable
    long kappa;             // EDM Variable
    double bestStat;        // EDM Variable
    double *timeSeries;     // Time series containing the observations
    long timeSeriesCount;   // Observations in time series
    long sigma;             // Sigma variable which breaks up total observations in series
    long treeDepth;         // Depth of tree to be made
    long bestLocation;      // Breakpoint Location

    IntervalTree *wiDistLeft;       // Left half of within distance tree (T-a)
    IntervalTree *wiDistRight;      // Right half of within distance tree (T-b)
    IntervalTree *bwDistTree;       // The between distance tree (T-ab)

public:
    Breakpoint(double*, long, long, long);
    ~Breakpoint();

    long getBreakpointLocation();

    void forwardUpdate();
    void backwardUpate();
};
