//
//  node.cpp
//  search
//
//  Created by Young Kim on 9/9/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "node.h"

using namespace std;

/* Public Func Definition */

/* Constructor */
Node::Node(int nid, int depth, int cost, Node *parent, int space, vector<int> *state) {
    this->nodeID = nid;
    this->depth = depth;
    
    this->parent = parent;
    this->children = new vector<Node*>();
    
    this->n = space;
    this->state = new vector<int>(*state);
    this->frontier = new vector<int>();
    this->invalid = new set<int>();
    
    for(int i = 0; i < this->state->size(); i++) {
        if((*this->state)[i] == -1) {
            this->invalid->insert(i);
        }
    }
    
    this->cost = cost;
}

/* Destructor */
Node::~Node() {
    delete this->state;
    delete this->frontier;
    delete this->invalid;
    
    for(int i = 0; i < this->children->size(); i++) {
        delete (*this->children)[i];
    }
    
    delete this->children;
}


int Node::insertLizard() {
    
    int nid = this->nodeID; // insert loc in state space
    int n = this->n;
    vector<int> *currState = this->state;
    
    // insert lizard, update state space
    if((*currState)[nid] != 0) {
        cout << "DEBUG: Fatal error can't insert at " << nid << endl;
        return -1;
    }
    (*currState)[nid] = 1;
    
    // invalidate possible locations for future lizard
    set<int> *invalid = this->invalid;
    
    // node id range is from 0 to (n x n) - 1
    if(nid != -1 && nid < (n * n)) {
        lookEast(nid, n, currState, invalid);
        lookWest(nid, n, currState, invalid);
        lookNorth(nid, n, currState, invalid);
        lookSouth(nid, n, currState, invalid);
        lookNorthEast(nid, n, currState, invalid);
        lookSouthEast(nid, n, currState, invalid);
        lookNorthWest(nid, n, currState, invalid);
        lookSouthWest(nid, n, currState, invalid);
        
        // update state space for invalid loc
        set<int>::iterator it;
        for(it = invalid->begin(); it != invalid->end(); ++it) {
            (*currState)[*it] = -1;
        }
    }
    return nid;
}

void Node::updateCost() {
    this->cost = (int) this->invalid->size();
}

int Node::expandNode() {
    
    int nid = this->nodeID;
    int n = this->n;
    
    if(nid >= (n * n)) {
        cout << "DEBUG: Fatal error node id out of bounds!" << endl;
    }
    
    vector<int> *currState = this->state;
    
    // update frontier vector for possible states
    for(int i = 0; i < currState->size(); i++) {
        if((*currState)[i] == 0) {
            this->frontier->push_back(i);
            
            // update children list with new nodes
            int newDepth = this->depth + 1;
            int newCost = this->cost; // parent cost
            
            Node *c = new Node(i, newDepth, newCost, this, this->n, this->state);
            c->insertLizard();
            c->updateCost(); // update cost to num of conflicts or invalid nodes
            this->children->push_back(c);
            
        }
    }
    return (int) this->frontier->size();
}

int Node::goalTest() {
    
    int c = 0;
    for(int i = 0; i < state->size(); i++) {
        if((*state)[i] == 1) {
            c++;
        }
    }
    return c;
}

bool Node::isEqualState(Node *rhs) {
    
    bool check1 = (depth == rhs->depth);
    
    bool check5 = false;
    
    if(check1 == true) {
        if(nodeID == rhs->nodeID) {
            check5 = true;
        } else if(idxRotate90(nodeID) == rhs->nodeID) {
            check5 = true;
        } else if(idxRotate180(nodeID) == rhs->nodeID) {
            check5 = true;
        } else if(idxRotate270(nodeID) == rhs->nodeID) {
            check5 = true;
        } else if(idxReflectVertical(nodeID) == rhs->nodeID) {
            check5 = true;
        } else if(idxReflectVertical(idxRotate90(nodeID)) == rhs->nodeID) {
            check5 = true;
        } else if(idxReflectVertical(idxRotate180(nodeID)) == rhs->nodeID) {
            check5 = true;
        } else if(idxReflectVertical(idxRotate270(nodeID)) == rhs->nodeID) {
            check5 = true;
        } else if(idxReflectHorizontal(nodeID) == rhs->nodeID) {
            check5 = true;
        } else if(idxReflectHorizontal(idxRotate90(nodeID)) == rhs->nodeID) {
            check5 = true;
        } else if(idxReflectHorizontal(idxRotate180(nodeID)) == rhs->nodeID) {
            check5 = true;
        } else if(idxReflectHorizontal(idxRotate270(nodeID)) == rhs->nodeID) {
            check5 = true;
        } else {
            return false;
        }
    } else {
        return check1;
    }
    
    return check1;
}

void Node::printState() {
    cout << "DEBUG: Print State Space of node[" << nodeID << "]" << endl;
    
    vector<int> *v = this->state;
    int n = this->n;
    
    for(int i = 0; i < v->size(); i++) {
        if((*v)[i] == -1) {
            cout << 3 << " ";
        } else {
            cout << (*v)[i] << " ";
        }
        
        if(idxToCol(i, n) == n - 1) {
            cout << endl;
        }
    }
}

void Node::printFrontier() {
    cout << "DEBUG: Print Frontier Vector" << endl;
    
    vector<int> *v = this->frontier;
    
    if(v->empty()) {
        cout << "vector is empty!" << endl;
    } else {
        cout << "[ ";
        for(int i = 0; i < v->size(); i++) {
            cout << (*v)[i] << " ";
        }
        cout << "]" << endl;
    }
}

void Node::printChildren() {
    cout << "DEBUG: Print Children Nodes" << endl;
    
    vector<Node*> *v = this->children;
    if(v->empty()) {
        cout << "children vector empty!" << endl;
    } else {
        cout << "[ ";
        for(int i = 0; i < v->size(); i++) {
            cout << (*v)[i]->nodeID << " ";
        }
        cout << "]" << endl;
    }
    
}

void Node::printNode() {
    cout << "DEBUG: Print Node Info" << endl;
    
    cout << "\t Node ID: " << this->nodeID << endl;
    cout << "\t depth: " << this->depth << endl;
    cout << "\t cost: " << this->cost << endl;
    
    if(this->parent == NULL) {
        cout << "\t parent node id: none" << endl;
    } else {
        cout << "\t parent node id: " << this->parent->nodeID << endl;
    }
    
    
    this->printState();
    this->printFrontier();
    this->printChildren();
    this->printInvalid();
    
}

/* Private Func Definition */


/* Helper Func Definitions */

int Node::idxToCol(int i, int n) {
    return i % n;
}

int Node::idxToRow(int i, int n) {
    return i / n;
}

int Node::coordToIdx(int row, int col) {
    return ((row * n) + col);
}

int Node::idxRotate90(int i) {
    int w = this->n;
    
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int row90 = (w - 1) - col;
    int col90 = row;
    
    return coordToIdx(row90, col90);
}

int Node::idxRotate180(int i) {
    int w = this->n;
    
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int row180 = (w - 1) - row;
    int col180 = (w - 1) - col;
    
    return coordToIdx(row180, col180);
}

int Node::idxRotate270(int i) {
    int w = this->n;
    
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int row270 = col;
    int col270 = (w - 1) - row;
    
    return coordToIdx(row270, col270);
}

int Node::idxReflectVertical(int i) {
    int w = this->n;
    
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int rowRV = row;
    int colRV = (w - 1) - col;
    
    return coordToIdx(rowRV, colRV);
}

int Node::idxReflectHorizontal(int i) {
    int w = this->n;
    
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int rowRH = (w - 1) - row;
    int colRH = col;
    
    return coordToIdx(rowRH, colRH);
}

void Node::printInvalid() {
    cout << "DEBUG: Print Invalid Nodes" << endl;
    set<int> *v = this->invalid;
    set<int>::iterator it;
    
    cout << "size of invalid set: " << v->size() << endl;
    /* cout << "invalid node id: {";
    for(it = v->begin(); it != v->end(); ++it) {
        cout << *it << " ";
    }
    cout << "}" << endl; */
}

/* Operators Func Definitions */

// look to EAST of curr loc (p) for possible loc to add to frontier
void Node::lookEast(int p, int w, vector<int> *state, set<int> *frontier) {
    int r = idxToRow(p, w);
    for(int i = p + 1; i < ((r + 1) * w); i++) {
        // hit a tree; don't add to frontier, break
        if((*state)[i] == 2) {
            break;
        } else {
            // loc with -1 (invalid) will be already in set
            frontier->insert(i);
        }
    }
}

// look to WEST of curr loc (p) for possible loc to add to frontier
void Node::lookWest(int p, int w, vector<int> *state, set<int> *frontier) {
    int r = idxToRow(p, w);
    for(int i = p - 1; i >= (r * w); i--) {
        // hit a tree or invalid; don't add to frontier, break
        if((*state)[i] == 2) {
            break;
        } else {
            frontier->insert(i);
        }
    }
}

// look to SOUTH of curr loc (p) for possible loc to add to frontier
void Node::lookSouth(int p, int w, vector<int> *state, set<int> *frontier) {
    int c = idxToCol(p, w);
    for(int i = p + w; i <= (c + w * (w - 1)); i += w) {
        // hit a tree or invalid; don't add to frontier, break
        if((*state)[i] == 2) {
            break;
        } else {
            frontier->insert(i);
        }
    }
}

// look to NORTH of curr loc (p) for possible loc to add to frontier
void Node::lookNorth(int p, int w, vector<int> *state, set<int> *frontier) {
    int c = idxToCol(p, w);
    for(int i = p - w; i >= c; i -= w) {
        // hit a tree or invalid; don't add to frontier, break
        if((*state)[i] == 2) {
            break;
        } else {
            frontier->insert(i);
        }
    }
}

// look to SOUTH-EAST of curr loc (p) for possible loc to add to frontier
void Node::lookSouthEast(int p, int w, vector<int> *state, set<int> *frontier) {
    // case: not at edge col, else can't look beyond
    if(idxToCol(p, w) != (w - 1)) {
        for(int i = p + w + 1; i < (w * w); i += (w + 1)) {
            
            // hit a tree or invalid; don't add to frontier, break
            if((*state)[i] == 2) {
                break;
            } else {
                frontier->insert(i);
                
                // case: edge col, break
                if(idxToCol(i, w) == (w - 1)) {
                    break;
                }
            }
        }
    }
    
}

// look to NORTH-EAST of curr loc (p) for possible loc to add to frontier
void Node::lookNorthEast(int p, int w, vector<int> *state, set<int> *frontier) {
    // case: not at edge col, else can't look beyond
    if(idxToCol(p, w) != (w - 1)) {
        for(int i = p - w + 1; i > 0; i -= (w - 1)) {
            
            // hit a tree or invalid; don't add to frontier, break
            if((*state)[i] == 2) {
                break;
            } else {
                frontier->insert(i);
                
                // case: edge col, break
                if(idxToCol(i, w) == (w - 1)) {
                    break;
                }
            }
        }
    }
}

// look to SOUTH-WEST of curr loc (p) for possible loc to add to frontier
void Node::lookSouthWest(int p, int w, vector<int> *state, set<int> *frontier) {
    // case: not at edge col, else can't look beyond
    if(idxToCol(p, w) != 0) {
        for(int i = p + w - 1; i < (w * w); i += (w - 1)) {
            
            // hit a tree or invalid; don't add to frontier, break
            if((*state)[i] == 2) {
                break;
            } else {
                frontier->insert(i);
                
                // case: edge col, break
                if(idxToCol(i, w) == 0) {
                    break;
                }
            }
        }
    }
}

// look to NORTH-WEST of curr loc (p) for possible loc to add to frontier
void Node::lookNorthWest(int p, int w, vector<int> *state, set<int> *frontier) {
    // case: not at edge col, else can't look beyond
    if(idxToCol(p, w) != 0) {
        for(int i = p - w - 1; i >= 0; i -= (w + 1)) {
            
            // hit a tree or invalid; don't add to frontier, break
            if((*state)[i] == 2) {
                break;
            } else {
                frontier->insert(i);
                
                // case: edge col, break
                if(idxToCol(i, w) == 0) {
                    break;
                }
            }
        }
    }
}
