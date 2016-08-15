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
    IntervalNode *left;             // Left Child
    IntervalNode *right;            // Right Child
};

class IntervalTree {
    IntervalNode *root;         // Root of tree
    bool isInitialized;         // Determines whether tree has been initialized or not
    int nodesAdded;             // Number of Nodes added to tree
    int _depthLevel;            // Depth level of the tree

    void _garbageCollect(IntervalNode *);
    void _constructTree(IntervalNode**, double, double);
    void _add(IntervalNode *, double);
    void _displayTree(IntervalNode *);
    double _getApproxMedian(IntervalNode *, long K);
    IntervalNode* makeNode(double, double);

    bool isLeafNode(IntervalNode *node) {
        if (node->left) {
            return false;
        } else if (node -> right) {
            return false;
        } else {
            return true;
        }
    }

public:
    IntervalTree(bool, int);
    ~IntervalTree();

    void add(double);
    void constructTree();
    void displayTree();

    double getApproxMedian();

    IntervalNode *getRoot() {
        return root;
    }

    int getSize() {
        return nodesAdded;
    }
};
