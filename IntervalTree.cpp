/*
 * This file defines the class functions
 * declared in "IntervalTree.h" header
 *
 * Author: Yash Gupta
 * Copyright: Yash Gupta, SSRC - UC Santa Cruz
 */

#include "IntervalTree.h"

/* Default Constructor */
IntervalTree::IntervalTree(bool initialize, unsigned long passedDepthLevel)
{
    tree = NULL;
    _treeSize = 0;
    _depthLevel = passedDepthLevel;
    nodesAdded = 0;
    isInitialized = false;

    if (_depthLevel == 0) {
        _treeSize = 1;
    } else {
        _treeSize = (1 << _depthLevel) - 1;
    }


    if (initialize == true) {
        isInitialized = true;
        tree = new IntervalNode[_treeSize];
        _constructTree(ROOT_BEG, ROOT_END);
    }
}

/* Destructor */
IntervalTree::~IntervalTree()
{
    _garbageCollect();
}

/**
 * Free's all the heap memory
 * acquired by the Tree
 */
void
IntervalTree::_garbageCollect()
{
    delete [] tree;
}

/**
 * User facing contruct call which checks
 * for a valid boolean and then initializes
 * tree if valid
 */
void
IntervalTree::constructTree()
{
    if (isInitialized) {
        std::cout << "[CONSTRUCT] Tree has already been Initialized" << std::endl;
        return;
    } else {
        isInitialized = true;
        tree = new IntervalNode[_treeSize];
        _constructTree(ROOT_BEG, ROOT_END);
    }
}

/**
 * Construct an interval tree iteratively
 * using the intervals provided
 *
 * Arguments
 *      low: Low Interval value
 *      high: High Interval value
 */
void
IntervalTree::_constructTree(double low, double high)
{
    unsigned long traverseLength;

    if (_depthLevel == 0) {
        traverseLength = 0;
    } else {
        traverseLength = (1 << (_depthLevel - 1)) - 1;
    }

    tree[0].observationsInInterval = 0;
    tree[0].intervalSpan.low = low;
    tree[0].intervalSpan.high = high;

    for (unsigned long i = 0; i < traverseLength; i++) {
        long leftChild = (i << 1) + 1;
        long rightChild = (i << 1) + 2;
        Interval temp = tree[i].intervalSpan;

        tree[leftChild].observationsInInterval = 0;
        tree[leftChild].intervalSpan.low = temp.low;
        tree[leftChild].intervalSpan.high = (temp.low + temp.high) / 2.0;

        tree[rightChild].observationsInInterval = 0;
        tree[rightChild].intervalSpan.low = (temp.low + temp.high) / 2.0;
        tree[rightChild].intervalSpan.high = temp.high;
    }
}

/**
 * Creates an interval structure based on
 * the observation and Creates a node
 * based on it
 *
 * Arguments
 *      observation: Observation to be added
 */
void
IntervalTree::add(double observation)
{
    if (observation >= ROOT_BEG && observation <= ROOT_END) {
        nodesAdded += 1;
        _add(0, observation);
    } else {
        std::cout << "[ADD] Observation not within limit" << std::endl;
    }
}

/**
 * Traverses the tree and adds the
 * observation to it
 *
 * Arguments
 *      index: Index of tree to add observation
 *      observation: value of observation
 */
void
IntervalTree::_add(long index, double observation)
{
    long leftChild;
    long rightChild;
    Interval temp;

    tree[index].observationsInInterval += 1;
    leftChild = (index << 1) + 1;
    rightChild = (index << 1) + 2;
    temp = tree[index].intervalSpan;

    // Check for left child interval
    if (observation >= temp.low && observation < ((temp.low + temp.high) / 2.0)) {
        if (leftChild < _treeSize) {
            _add(leftChild, observation);
        }
    }

    // Check for right child interval. Have a closed right interval if the high value is 1
    if (temp.high == 1) {
        if (observation >= ((temp.low + temp.high) / 2.0) && observation <= temp.high) {
            if (rightChild < _treeSize) {
                _add(rightChild, observation);
            }
        }
    }
    else if (observation >= ((temp.low + temp.high) / 2.0) && observation < temp.high) {
        if (rightChild < _treeSize) {
            _add(rightChild, observation);
        }
    }
}

/**
 * Call median calculator
 */
double
IntervalTree::getApproxMedian()
{
    if (nodesAdded == 0) {
        std::cout << "[MEDIAN] Tree is Empty" << std::endl;
        return -1;
    } else {
        return _getApproxMedian(0, ceil(nodesAdded / 2.0));
    }
}

/**
 * Calculate median based on EDM
 * implementation.
 *
 * Arguments
 *      index: Calculate median of Tree/SubTree at index
 *      K: EDM implementation specific variable
 */
double
IntervalTree::_getApproxMedian(long index, long K)
{
    long leftChild = (index << 1) + 1;
    long rightChild = (index << 1) + 2;
    Interval temp = tree[index].intervalSpan;
    Interval tempLeft;
    Interval tempRight;

    if (isLeafNode(index)) {
        if (K != tree[index].observationsInInterval) {
            double low = temp.low;
            double high = temp.high;
            double weight = (double)K / (double)(tree[index].observationsInInterval);

            return low + ((high - low) * weight);
        } else if (K == tree[index].observationsInInterval) {
            if (index + 1 < _treeSize) {
                Interval tempNext = tree[index + 1].intervalSpan;
                long observationsInNextInterval = tree[index + 1].observationsInInterval;
                long observationsInCurrentInterval = tree[index].observationsInInterval;
                double currentLow = temp.low;
                double currentHigh = temp.high;
                double nextLow = tempNext.low;
                double nextHigh = tempNext.high;

                double currentWeight = (currentHigh - currentLow) / (double)observationsInCurrentInterval;
                double nextWeight = (nextHigh - nextLow) / (double)observationsInNextInterval;

                return (currentWeight + nextWeight) / 2.0;
            } else {
                double low = temp.low;
                double high = temp.high;
                double weight = (double)K / (double)(tree[index].observationsInInterval);

                return low + ((high - low) * weight);
            }
        }
    }

    tempLeft = tree[leftChild].intervalSpan;
    tempRight = tree[rightChild].intervalSpan;

    if (tree[index].observationsInInterval == K) {
        long leftObservation = tree[leftChild].observationsInInterval;
        long rightObservation = tree[rightChild].observationsInInterval;

        double leftMidPoint = (tempLeft.low + tempLeft.high) / 2.0;
        double rightMidPoint = (tempRight.low + tempRight.high) / 2.0;

        double leftWeight = (double)leftObservation * leftMidPoint;
        double rightWeight = (double)rightObservation * rightMidPoint;
        double overallWeight = leftWeight + rightWeight;

        return overallWeight / (double)(leftObservation + rightObservation);
    }

    if (tree[leftChild].observationsInInterval >= K) {
        return _getApproxMedian(leftChild, K);
    } else {
        K = K - tree[leftChild].observationsInInterval;
        return _getApproxMedian(rightChild, K);
    }

    // Control flow should never reach here. Error!
    return -1;
}

/**
 * Check tree for valid value and
 * call displayer if valid
 */
void
IntervalTree::displayTree()
{
    if (!isInitialized) {
        std::cout << "[DISPLAY] Tree is not Initialized" << std::endl;
        return;
    }
    _displayTree(0);
}

/**
 * In order traversal of Tree
 * to display all the intervals
 *
 * Arguments
 *      index: The index to begin traversing at
 */
void
IntervalTree::_displayTree(long index)
{
    long leftChild = (index << 1) + 1;
    long rightChild = (index << 1) + 2;
    Interval temp = tree[index].intervalSpan;

    if (leftChild < _treeSize) {
        _displayTree(leftChild);
    }

    printf("%ld [%0.3f, %0.3f] Observations: %ld\n",
           index,
           temp.low,
           temp.high,
           tree[index].observationsInInterval);

    if (rightChild < _treeSize) {
        _displayTree(rightChild);
    }
}
