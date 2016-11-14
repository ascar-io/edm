/*
 * This File declares our class and structure
 * for an Interval Tree used by the Breakpoint
 * detection algorithm
 *
 * Author: Yash Gupta
 * Copyright: Yash Gupta, SSRC - UC Santa Cruz
 */

#include <stdio.h>
#include <iostream>
#include <math.h>

/* Change these macros on basis of requirement */
#define ROOT_BEG        (0.0)       // Beginning of the Root Node Interval
#define ROOT_END        (1.0)       // Ending of the Root Node Interval

struct Interval {
    double low;         // Low end of Interval
    double high;        // High end of interval
};

struct IntervalNode {
    long observationsInInterval;    // Observations observed in this Interval
    Interval intervalSpan;          // Interval Span for this Node
};

class IntervalTree {
    IntervalNode *tree;         // The Tree array
    long _treeSize;             // Sizenifies size of the array
    bool isInitialized;         // Determines whether tree has been initialized or not
    long nodesAdded;            // Number of Nodes added to tree
    unsigned long _depthLevel;  // Depth level of the tree

    void _garbageCollect();
    void _constructTree(double, double);
    void _add(long, double);
    void _displayTree(long);
    double _getApproxMedian(long, long);

    /**
     * Get boolean value to know
     * whether specified node is a leaf
     * node of the tree or not
     */
    bool isLeafNode(long index) {
        long leftChild = (index << 1) + 1;
        long rightChild = (index << 1) + 2;

        if (leftChild >= _treeSize || rightChild >= _treeSize) {
            return true;
        } else {
            return false;
        }
    }

public:
    IntervalTree(bool, unsigned long);
    ~IntervalTree();

    /**
     * Wrapper for adding an element to the tree
     */
    void add(double);

    /**
     * Wrapper for contructing the tree
     */
    void constructTree();

    /**
     * Wrapper for displaying the tree
     */
    void displayTree();


    /**
     * Wrapper for using the EDM algorithm
     * to get an approximate median
     */
    double getApproxMedian();

    /**
     * Get the number of observations added
     * to the tree
     */
    long getSize() {
        return nodesAdded;
    }
};
