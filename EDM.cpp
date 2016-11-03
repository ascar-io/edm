/*
 * This file defines the class functions
 * declared in "EDM.h" header
 *
 * Author: Yash Gupta
 * Copyright: Yash Gupta, SSRC - UC Santa Cruz
 */

#include <cmath>
#include "EDM.h"

using namespace std;

/**
 * Scales the values in the time series to
 * the interval (0, 1]. Current algorithm
 * simply finds the largest observation and
 * divides the entire series by it.
 *
 * Arguments
 *      timeSeries: The Series to scale
 *      count: The number of observations in series
 */
static void
_scaleTimeSeries(double *timeSeries, long count) {
    double max = timeSeries[0];

    for (long i = 1; i < count; i++) {
        if (timeSeries[i] > max) {
            max = timeSeries[i];
        }
    }

    for (long i = 0; i < count; i++) {
        timeSeries[i] /= max;
    }
}

/* Default Constructor */
Breakpoint::Breakpoint(double *passedTimeSeries, long passedCount, long passedDelta, long passedDepth)
{
    timeSeries = passedTimeSeries;
    timeSeriesCount = passedCount;
    delta = passedDelta;
    treeDepth = passedDepth;

    wiDistLeft = new IntervalTree(true, treeDepth);
    wiDistRight = new IntervalTree(true, treeDepth);
    bwDistTree = new IntervalTree(true, treeDepth);

    tau = delta;
    kappa = tau * 2;

    _scaleTimeSeries(timeSeries, timeSeriesCount);
}

/* Destructor */
Breakpoint::~Breakpoint() {
    delete wiDistLeft;
    delete wiDistRight;
    delete bwDistTree;
}

/**
 * Main algorithm which implements breakpoint detection
 * and utilizes the *update functions
 */
long
Breakpoint::getBreakpointLocation()
{
    double median1, median2, median3;
    short forwardMove = 0;

    // Initialize within distance trees
    for (long i = 0; i < delta; ++i) {
        for (long j = i + 1; j < delta; ++j) {
            wiDistLeft->add(abs(timeSeries[i] - timeSeries[j]));
            wiDistRight->add(abs(timeSeries[i + (delta - 1)] - timeSeries[j + (delta - 1)]));
        }
    }

    // Initialize between distance tree
    for (long i = 0; i < delta; ++i) {
        for (long j = 0; j < delta; ++j) {
            bwDistTree->add(abs(timeSeries[i] - timeSeries[j + (delta - 1)]));
        }
    }

    median1 = bwDistTree->getApproxMedian();
    median2 = wiDistLeft->getApproxMedian();
    median3 = wiDistRight->getApproxMedian();

    bestStat = (tau * (kappa - tau)) / kappa;
    bestStat = bestStat * (2 * median1 - median2 - median3);
    bestLocation = (delta - 1);
    tau = (delta - 1);

    while (tau < (timeSeriesCount - delta)) {
        if (forwardMove) {
            forwardUpdate();
        } else {
            backwardUpate();
        }
        forwardMove = 1 - forwardMove;
    }

    return bestLocation;
}

/**
 * Forward Update operation of the algorithm
 */
void
Breakpoint::forwardUpdate()
{
    double stat;
    double median1, median2, median3;
    long tempKappa;

    ++tau;
    for (tempKappa = tau + (delta - 1); tempKappa < timeSeriesCount; ++tempKappa) {
        wiDistRight->add(abs(/*-*/timeSeries[tempKappa] - timeSeries[tempKappa - 1]));
        median1 = bwDistTree->getApproxMedian();
        median2 = wiDistLeft->getApproxMedian();
        median3 = wiDistRight->getApproxMedian();

        stat = (tau * (tempKappa - tau)) / tempKappa;
        stat = stat * (2 * median1 - median2 - median3);
        if (stat > bestStat) {
            bestStat = stat;
            bestLocation = tau;
        }
    }
}

/**
 * Backward Update operation of the algorithm
 */
void
Breakpoint::backwardUpate()
{
    double stat;
    double median1, median2, median3;
    long tempKappa;

    ++tau;
    tempKappa = (timeSeriesCount - 1);
    while (tempKappa >= (tau + (delta - 1))) {
        wiDistRight->add(abs(timeSeries[tempKappa] - timeSeries[tempKappa - 1]));
        median1 = bwDistTree->getApproxMedian();
        median2 = wiDistLeft->getApproxMedian();
        median3 = wiDistRight->getApproxMedian();

        stat = (tau * (tempKappa - tau)) / tempKappa;
        stat = stat * (2 * median1 - median2 - median3);
        if (stat > bestStat) {
            bestStat = stat;
            bestLocation = tau;
        }
        tempKappa = tempKappa - 1;
    }
}
