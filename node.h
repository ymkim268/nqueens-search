//
//  node.h
//  search
//
//  Created by Young Kim on 9/9/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#ifndef node_h
#define node_h

#include <vector>
#include <set>

#include "input.h"

class Node {
    
public:
    
    int nodeID;
    int depth;
    int cost;
    
    Node *parent;
    std::vector<Node*> *children; // same as frontier but with Node object
    
    std::vector<int> *state; // state space
    std::vector<int> *frontier; // unexplored nodes ID here
    
    /* Constructor */
    Node(int nid, int depth, int cost, Node *parent, int space, std::vector<int> *state);
    /* Destructor */
    ~Node();
    
    void printState();
    void printFrontier();
    void printChildren();
    void printNode();
    
    int expandNode(); // only when node expand -> update children, frontier
    int insertLizard(); // operator Insert Lizard
    int goalTest();
    
    bool isEqualState(Node *rhs);
    
private:
    
    int n; // n x n (width x height)
    std::set<int> *invalid; // set of invalid nodes
    
    void updateCost(); // update cost with size of invalid nodes
    
    /* Helper functions */
    int idxToCol(int i, int n);
    int idxToRow(int i, int n);
    int coordToIdx(int row, int col);
    
    // used to identify mirror states
    int idxRotate90(int i);
    int idxRotate180(int i);
    int idxRotate270(int i);
    int idxReflectVertical(int i);
    int idxReflectHorizontal(int i);
    
    void printInvalid();
    
    /* Operator Helper functions */
    void lookEast(int p, int w, std::vector<int> *state, std::set<int> *invalid);
    void lookWest(int p, int w, std::vector<int> *state, std::set<int> *invalid);
    void lookSouth(int p, int w, std::vector<int> *state, std::set<int> *invalid);
    void lookNorth(int p, int w, std::vector<int> *state, std::set<int> *invalid);
    void lookSouthEast(int p, int w, std::vector<int> *state, std::set<int> *invalid);
    void lookNorthEast(int p, int w, std::vector<int> *state, std::set<int> *invalid);
    void lookSouthWest(int p, int w, std::vector<int> *state, std::set<int> *invalid);
    void lookNorthWest(int p, int w, std::vector<int> *state, std::set<int> *invalid);
    
};



#endif /* node_h */
