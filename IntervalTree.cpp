/*
 * This file defines the class functions
 * declared in "IntervalTree.h" header
 *
 * Author: Yash Gupta
 * Copyright: Yash Gupta, SSRC - UC Santa Cruz
 */

#include "IntervalTree.h"

// TODO: Switch printf to cout for C++ consistency

/* Default Constructor */
IntervalTree::IntervalTree(bool initialize, int passedDepthLevel)
{
    root = NULL;
    _depthLevel = passedDepthLevel;
    nodesAdded = 0;
    isInitialized = false;

    if (initialize == true) {
        isInitialized = true;
        _constructTree(&root, ROOT_BEG, ROOT_END);
    }
}

/* Destructor */
IntervalTree::~IntervalTree()
{
    _garbageCollect(root);
}

/**
 * Free's all the heap memory
 * acquired by the Tree
 *
 * Arguments
 *      node: Tree/SubTree to begin collection at
 */
void
IntervalTree::_garbageCollect(IntervalNode *node)
{
    if (node->left) {
        _garbageCollect(node->left);
    }

    IntervalNode *temp = node->right;
    delete node;

    if (temp) {
        _garbageCollect(temp);
    }
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
        std::cout << "[INIT] Tree has already been Initialized" << std::endl;
        return;
    } else {
        isInitialized = true;
        _constructTree(&root, ROOT_BEG, ROOT_END);
    }
}

/**
 * Construct an interval tree recursively
 * using the intervals provided
 *
 * Arguments
 *      node: Address of node to start construction at
 *      low: Low Interval value
 *      high: High Interval value
 */
void
IntervalTree::_constructTree(IntervalNode** node, double low, double high)
{
    static int depthLevel = 0;

    *node = makeNode(low, high);
    //std::cout << "Made Node [" << low << ", " << high << "]" << std::endl;

    if (depthLevel < _depthLevel) {
        depthLevel += 1;
        _constructTree(&((*node)->left), low, (low + high) / 2.0);
        _constructTree(&((*node)->right), (low + high) / 2.0, high);
        depthLevel -= 1;
    }
}

/**
 * Make a tree node based on the
 * interval passed
 *
 * Arguments
 *      low: Low value of Interval
 *      high: Hgh value of Interval
 */
IntervalNode*
IntervalTree::makeNode(double low, double high)
{
    IntervalNode *newNode = new IntervalNode;
    newNode->intervalSpan.low = low;
    newNode->intervalSpan.high = high;
    newNode->observationsInInterval = 0;
    newNode->left = newNode->right = NULL;

    return newNode;
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
        _add(root, observation);
    } else {
        std::cout << "[ADD] Observation not within limit" << std::endl;
    }
}

/**
 * Traverses the tree and adds the
 * observation to it
 *
 * Arguments
 *      node: Node at which to be added
 *      observation: value of observation
 */
void
IntervalTree::_add(IntervalNode *node, double observation)
{
    node -> observationsInInterval += 1;

    // TODO: Clean up checks

    // Check for left child interval
    if ((observation >= node->intervalSpan.low) && (observation < ((node->intervalSpan.low + node->intervalSpan.high) / 2.0))) {
        if (node->left) {
            _add(node->left, observation);
        } else {
            return;
        }
    }

    // Check for right child interval
    // TODO: The interval should be closed for the right most leaf node
    if ((observation >= ((node->intervalSpan.low + node->intervalSpan.high) / 2.0)) && (observation < node->intervalSpan.high)) {
        if (node->right) {
            _add(node->right, observation);
        } else {
            return;
        }
    }
}

/**
 * Call median calculator
 */
double
IntervalTree::getApproxMedian()
{
    if (root == NULL) {
        std::cout << "[MEDIAN] Root is NULL" << std::endl;
        return -1;
    } else {
        return _getApproxMedian(root, ceil(nodesAdded / 2.0));
    }
}

/**
 * Calculate median based on EDM
 * implementation.
 *
 * Arguments
 *      node: Calculate median of Tree/SubTree at node
 *      K: EDM implementation specific variable
 */
double
IntervalTree::_getApproxMedian(IntervalNode *node, long K)
{
    if (isLeafNode(node)) {
        return (node->intervalSpan.low + node->intervalSpan.high) / 2.0;
    }

    if (node->observationsInInterval == K) {
        long leftObservation = node->left->observationsInInterval;
        long rightObservation = node->right->observationsInInterval;

        double leftMidPoint = (node->left->intervalSpan.low + node->left->intervalSpan.high) / 2.0;
        double rightMidPoint = (node->right->intervalSpan.low + node->right->intervalSpan.high) / 2.0;

        return (double)(((leftObservation * leftMidPoint) + (rightObservation * rightMidPoint)) / (leftObservation + rightObservation));
    }

    if (node->left->observationsInInterval >= K) {
        return _getApproxMedian(node->left, K);
    }

    if (node->left->observationsInInterval < K) {
        K = K - (node->left->observationsInInterval);
        return _getApproxMedian(node->right, K);
    }

    // Control flow should never reach here. Error!
    return -1;
}

/**
 * Check root for valid value and
 * call displayer if valid
 */
void
IntervalTree::displayTree()
{
    if (root == NULL) {
        printf("[DISPLAY] Tree is Empty\n");
        return;
    }
    IntervalNode *tempNode = root;
    _displayTree(tempNode);
}

/**
 * In order traversal of Tree
 * to display all the intervals
 *
 * Arguments
 *      node: Node to begin traversing at
 */
void
IntervalTree::_displayTree(IntervalNode *node)
{
    if (node->left) {
        _displayTree(node->left);
    }

    printf("[%0.3f, %0.3f] Observations: %ld\n", node->intervalSpan.low, node->intervalSpan.high, node->observationsInInterval);

    if (node->right) {
        _displayTree(node->right);
    }
}
