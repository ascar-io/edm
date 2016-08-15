/*
 * This file defines the class functions
 * declared in "EDM.h" header
 *
 * Author: Yash Gupta
 * Copyright: Yash Gupta, SSRC - UC Santa Cruz
 */

#include "EDM.h"

/* Default Constructor */
Breakpoint::Breakpoint(double *passedTimeSeries, long passedCount, long passedSigma, long passedDepth)
{
    timeSeries = passedTimeSeries;
    timeSeriesCount = passedCount;
    sigma = passedSigma;
    treeDepth = passedDepth;

    wiDistLeft = new IntervalTree(true, treeDepth);
    wiDistRight = new IntervalTree(true, treeDepth);
    bwDistTree = new IntervalTree(true, treeDepth);

    tau = INIT_TAU;
    kappa = INIT_KAPPA;
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
    int i, j;
    double median1, median2, median3;
    short forwardMove = 0;

    // Initialize within distance trees
    for (i = 0; i < sigma; ++i) {
        for (j = i + 1; j < sigma; ++j) {
            wiDistLeft->add(abs(timeSeries[i] - timeSeries[j]));
            wiDistRight->add(abs(timeSeries[i + (sigma - 1)] - timeSeries[j + (sigma - 1)]));
        }
    }

    // Initialize between distance tree
    for (i = 0; i < sigma; ++i) {
        for (j = 0; j < sigma; ++j) {
            bwDistTree->add(abs(timeSeries[i] - timeSeries[j + (sigma - 1)]));
        }
    }

    median1 = bwDistTree->getApproxMedian();
    median2 = wiDistLeft->getApproxMedian();
    median3 = wiDistRight->getApproxMedian();

    bestStat = (tau * (kappa - tau)) / kappa;
    bestStat = bestStat * (2 * median1 - median2 - median3);
    bestLocation = (sigma - 1);
    tau = (sigma - 1);

    while (tau < (timeSeriesCount - sigma)) {
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
    for (tempKappa = tau + (sigma - 1); tempKappa < timeSeriesCount; ++tempKappa) {
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
    while (tempKappa >= (tau + (sigma - 1))) {
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
