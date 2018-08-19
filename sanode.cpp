//
//  sanode.cpp
//  search
//
//  Created by Young Kim on 9/16/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "sanode.h"


using namespace std;

/* Constructor */
SaNode::SaNode(int depth, int cost, SaNode *parent, int space, std::vector<int> *state) {
    
    // srand((unsigned int) time(NULL));
    
    this->depth = depth;
    this->cost = cost;
    
    this->parent = parent;
    
    this->state = new vector<int>(*state);
    this->lizardLoc = new vector<int>();
    this->conflictMap = new map<int, int>();

    for(int i = 0; i < this->state->size(); i++) {
        if((*this->state)[i] == 1) {
            this->lizardLoc->push_back(i);
        }
    }
    
    this->n = space;
    this->q = (int) this->lizardLoc->size();
    
}

/* Destructor */
SaNode::~SaNode() {
    delete this->state;
    delete this->lizardLoc;
    delete this->conflictMap;
}

int SaNode::getValue() {
    return this->cost;
}

int SaNode::randomLoc(int max) {
    return rand() % max;
}
void SaNode::updateCost() {
    
    int counter = 0;
    for(int i = 0; i < this->lizardLoc->size(); i++) {
        int idx = (*this->lizardLoc)[i];
        
        vector<int> c; // index loc of conflicts
        
        int ce = lookEast(idx, this->n, this->state);
        int cw = lookWest(idx, this->n, this->state);
        int cn = lookNorth(idx, this->n, this->state);
        int cs = lookSouth(idx, this->n, this->state);
        int cne = lookNorthEast(idx, this->n, this->state);
        int cnw = lookNorthWest(idx, this->n, this->state);
        int cse = lookSouthEast(idx, this->n, this->state);
        int csw = lookSouthWest(idx, this->n, this->state);
        
        c.push_back(ce);
        c.push_back(cw);
        c.push_back(cn);
        c.push_back(cs);
        c.push_back(cne);
        c.push_back(cnw);
        c.push_back(cse);
        c.push_back(csw);
        
        for(int i = 0; i < c.size(); i++) {
            if(c[i] != -1) {
                counter++;
                
                if(this->conflictMap->find(c[i]) == this->conflictMap->end()) {
                    (*this->conflictMap)[c[i]] = 1;
                } else {
                    (*this->conflictMap)[c[i]] += 1;
                }
            }
        }
        
        //cout << "lizard at " << idx << endl;
        //cout << ce << "," << cw << "," << cn << "," << cs << ",";
        //cout << cne << "," << cnw << "," << cse << "," << csw << endl;
    }
    
    cout << "printing conflict map!" << endl;
    for(int i = 0; i < this->lizardLoc->size(); i++) {
        int idx = (*this->lizardLoc)[i];
        cout << idx << " = " << (*this->conflictMap)[idx] << endl;
    }
    
    this->cost = counter;
}

int getMinConflict(map<int, int> *m) {
    map<int, int>::iterator it;
    
    it = m->begin();
    int min = (*it).second;
    for( ; it != m->end(); ++it) {
        if((*it).second < min) {
            min = (*it).second;
        }
    }
    return min;
}

int getMaxConflict(map<int, int> *m) {
    map<int, int>::iterator it;
    
    it = m->begin();
    int max = (*it).second;
    for( ; it != m->end(); ++it) {
        if((*it).second > max) {
            max = (*it).second;
        }
    }
    return max;
}

void printSet(set<int> *s) {
    set<int>::iterator it;
    cout << "[";
    for(it = s->begin(); it != s->end(); ++it) {
        cout << (*it) << " ";
    }
    cout << "]" << endl;
}

void printVector(vector<int> *v) {
    cout << "[";
    for(int i = 0; i < v->size(); i++) {
        cout << (*v)[i] << " ";
    }
    cout << "]" << endl;
}



int SaNode::chooseRandomCandidate() {
    cout << "choose random candidate to remove " << endl;
    cout << "current pool size = " << lizardLoc->size() << endl;
    
    int randIdx = randGenerator((int) lizardLoc->size());
    if(!(randIdx < lizardLoc->size())) {
        cout << "ERROR: rand index out of bounds!" << endl;
    }
    
    cout << "random index chosen = " << randIdx << endl;
    cout << "choose lizard at loc = " << (*lizardLoc)[randIdx] << endl;
    
    return (*lizardLoc)[randIdx];
}

void SaNode::removeCandidateOnState(int idx, vector<int> *sstate) {
    cout << "remove lizard at loc = " << idx << endl;
    (*sstate)[idx] = 0;
    
    int count = 0;
    for(int i = 0; i < sstate->size(); i++) {
        if((*sstate)[i] == 1) {
            count++;
        }
    }
    
    if(count == (q - 1)) {
        cout << "state space maintained" << endl;
    } else {
        cout << "ERROR: state space NOT maintained!" << endl;
        cout << "curr count = " << count << endl;
    }
}


int SaNode::chooseRandomNewCandidate() {
    // contain all invalid loc
    // - current lizard loc including the one removed
    // - tree loc
    set<int> invalid;
    for(int i = 0; i < state->size(); i++) {
        if((*state)[i] == 2 || (*state)[i] == 1) {
            invalid.insert(i);
        }
    }
    cout << "invalid locations " << endl;
    printSet(&invalid);
    

    while(true) {
        int randIdx = randGenerator((n * n));
        if(!(randIdx < state->size())) {
            cout << "ERROR: rand index out of bounds" << endl;
        }
        
        if(invalid.find(randIdx) == invalid.end()) {
            cout << "going to insert lizard at = " << randIdx << endl;
            return randIdx;
        }
        
    }
}

void SaNode::insertNewCandidateOnState(int idx, vector<int> *sstate) {
    cout << "add new candidate with loc = " << idx << endl;
    (*sstate)[idx] = 1;
    
    int count = 0;
    for(int i = 0; i < sstate->size(); i++) {
        if((*sstate)[i] == 1) {
            count++;
        }
    }
    
    if(count == q) {
        cout << "state space maintained" << endl;
    } else {
        cout << "ERROR: state space NOT maintained!" << endl;
        cout << "curr count = " << count << endl;
    }
}


SaNode* SaNode::getSuccessor() {
    
    vector<int> *sstate = new vector<int>(*(this->state));
        
    // choose random candidate to remove
    int remove = chooseRandomCandidate();
    
    // choose new candidate to insert
    int insert = chooseRandomNewCandidate();
    
    if(insert == -1) {
        cout << "ERROR: could not find a random loc to insert!" << endl;
        cout << "ABORT!!" << endl;
    }
    
    // remove random candidate on state space
    (void) removeCandidateOnState(remove, sstate);
    
    // insert new candidate
    (void) insertNewCandidateOnState(insert, sstate);
    
    // inherit parent node, but update depth, cost, state
    SaNode *successor = new SaNode(depth + 1, cost, this, n, sstate);
    successor->updateCost();
    
    
    delete sstate;
    return successor;
}

void SaNode::printState() {
    cout << "DEBUG: Print State Space" << endl;
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



// return a rand int between 0 and max - 1
int randGenerator(int max) {
    return rand() % max;
}

bool inColSet(int i, int n, set<int> *s) {
    
    int col = idxToCol(i, n);
    
    set<int>::iterator it;
    for(it = s->begin(); it != s->end(); ++it) {
        int temp = idxToCol((*it), n);
        if(temp == col) {
            return true;
        }
    }
    
    return false;
}

bool inRowSet(int i, int n, set<int> *s) {
    
    int row = idxToRow(i, n);
    
    set<int>::iterator it;
    for(it = s->begin(); it != s->end(); ++it) {
        int temp = idxToRow((*it), n);
        if(temp == row) {
            return true;
        }
    }
    
    return false;
}

// init state space with 'p' lizards in rand loc with range (0 to n^2 - 1)
void SaNodeInitializeState(vector<int> *state, int n, int p) {
    cout << "initializing state space with random lizard loc!" << endl;
    // find tree loc in state space
    set<int> treeLoc;
    for(int idx = 0; idx < (n * n); idx++) {
        if((*state)[idx] == 2) {
            treeLoc.insert(idx); // treeLoc
        }
    }
    
    // find random loc to populate lizard in state space
    set<int> newLoc;
    while(newLoc.size() != p) { // newLoc size equal to num of lizards
        
        int idx = randGenerator((n * n)); // random loc between [0, (n*n)-1]
        if(treeLoc.find(idx) == treeLoc.end()) {
            newLoc.insert(idx);
        } else {
            cout << "re-roll -> tree at " << idx << endl;
        }
        
    }
    
    // insert lizard in random loc found in set<int> newLoc
    set<int>::iterator it;
    for(it = newLoc.begin(); it != newLoc.end(); ++it) {
        int loc = (*it);
        cout << "insert lizard at " << loc << endl;
        
        (*state)[loc] = 1;
    }
    cout << "finished initialization!!" << endl;
}

/* Helper Func Definitions */

int idxToCol(int i, int n) {
    return i % n;
}

int idxToRow(int i, int n) {
    return i / n;
}

int coordToIdx(int row, int col, int n) {
    return ((row * n) + col);
}

int idxRotate90(int i, int w) {
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int row90 = (w - 1) - col;
    int col90 = row;
    
    return coordToIdx(row90, col90, w);
}

int idxRotate180(int i, int w) {
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int row180 = (w - 1) - row;
    int col180 = (w - 1) - col;
    
    return coordToIdx(row180, col180, w);
}

int idxRotate270(int i, int w) {
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int row270 = col;
    int col270 = (w - 1) - row;
    
    return coordToIdx(row270, col270, w);
}

int idxReflectVertical(int i, int w) {
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int rowRV = row;
    int colRV = (w - 1) - col;
    
    return coordToIdx(rowRV, colRV, w);
}

int idxReflectHorizontal(int i, int w) {
    int col = idxToCol(i, w);
    int row = idxToRow(i, w);
    
    int rowRH = (w - 1) - row;
    int colRH = col;
    
    return coordToIdx(rowRH, colRH, w);
}

// look to EAST of curr loc (p) for possible loc to add to frontier
int SaNode::lookEast(int p, int w, vector<int> *state) {
    int r = idxToRow(p, w);
    for(int i = p + 1; i < ((r + 1) * w); i++) {
        // hit a tree; don't return, break
        if((*state)[i] == 2) {
            break;
        // if hit a lizard, conflict, return idx of conflict
        } else if((*state)[i] == 1) {
            return i;
        }
    }
    
    return -1; // no conflicts
}

// look to WEST of curr loc (p) for possible loc to add to frontier
int SaNode::lookWest(int p, int w, vector<int> *state) {
    int r = idxToRow(p, w);
    for(int i = p - 1; i >= (r * w); i--) {
        // hit a tree; don't return, break
        if((*state)[i] == 2) {
            break;
        // if hit a lizard, conflict, return idx of conflict
        } else if((*state)[i] == 1) {
            return i;
        }
    }
    return -1; // no conflicts
}

// look to SOUTH of curr loc (p) for possible loc to add to frontier
int SaNode::lookSouth(int p, int w, vector<int> *state) {
    int c = idxToCol(p, w);
    for(int i = p + w; i <= (c + w * (w - 1)); i += w) {
        // hit a tree; don't return, break
        if((*state)[i] == 2) {
            break;
        // if hit a lizard, conflict, return idx of conflict
        } else if((*state)[i] == 1) {
            return i;
        }
    }
    return -1; // no conflicts
}

// look to NORTH of curr loc (p) for possible loc to add to frontier
int SaNode::lookNorth(int p, int w, vector<int> *state) {
    int c = idxToCol(p, w);
    for(int i = p - w; i >= c; i -= w) {
        // hit a tree; don't return, break
        if((*state)[i] == 2) {
            break;
        // if hit a lizard, conflict, return idx of conflict
        } else if((*state)[i] == 1) {
            return i;
        }
    }
    return -1; // no conflicts
}

// look to SOUTH-EAST of curr loc (p) for possible loc to add to frontier
int SaNode::lookSouthEast(int p, int w, vector<int> *state) {
    // case: not at edge col, else can't look beyond
    if(idxToCol(p, w) != (w - 1)) {
        for(int i = p + w + 1; i < (w * w); i += (w + 1)) {
            
            // hit a tree; don't return, break
            if((*state)[i] == 2) {
                break;
            // if hit a lizard, conflict, return idx of conflict
            } else if((*state)[i] == 1) {
                return i;
            } else {
                // case: edge col, break
                if(idxToCol(i, w) == (w - 1)) {
                    break;
                }
            }
        }
    }
    return -1; // no conflicts
}

// look to NORTH-EAST of curr loc (p) for possible loc to add to frontier
int SaNode::lookNorthEast(int p, int w, vector<int> *state) {
    // case: not at edge col, else can't look beyond
    if(idxToCol(p, w) != (w - 1)) {
        for(int i = p - w + 1; i > 0; i -= (w - 1)) {
            
            // hit a tree; don't return, break
            if((*state)[i] == 2) {
                break;
            // if hit a lizard, conflict, return idx of conflict
            } else if((*state)[i] == 1) {
                return i;
            } else {
                // case: edge col, break
                if(idxToCol(i, w) == (w - 1)) {
                    break;
                }
            }
        }
    }
    return -1; // no conflicts
}

// look to SOUTH-WEST of curr loc (p) for possible loc to add to frontier
int SaNode::lookSouthWest(int p, int w, vector<int> *state) {
    // case: not at edge col, else can't look beyond
    if(idxToCol(p, w) != 0) {
        for(int i = p + w - 1; i < (w * w); i += (w - 1)) {
            
            // hit a tree; don't return, break
            if((*state)[i] == 2) {
                break;
            // if hit a lizard, conflict, return idx of conflict
            } else if((*state)[i] == 1) {
                return i;
            } else {
                // case: edge col, break
                if(idxToCol(i, w) == 0) {
                    break;
                }
            }
        }
    }
    return -1;
}

// look to NORTH-WEST of curr loc (p) for possible loc to add to frontier
int SaNode::lookNorthWest(int p, int w, vector<int> *state) {
    // case: not at edge col, else can't look beyond
    if(idxToCol(p, w) != 0) {
        for(int i = p - w - 1; i >= 0; i -= (w + 1)) {
            
            // hit a tree; don't return, break
            if((*state)[i] == 2) {
                break;
                // if hit a lizard, conflict, return idx of conflict
            } else if((*state)[i] == 1) {
                return i;
            } else {
                
                // case: edge col, break
                if(idxToCol(i, w) == 0) {
                    break;
                }
            }
        }
    }
    return -1;
}

// look to EAST of curr loc (p) for possible loc to add to frontier
void SaNode::checkEast(int p, int w, vector<int> *state, set<int> *frontier) {
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
void SaNode::checkWest(int p, int w, vector<int> *state, set<int> *frontier) {
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
void SaNode::checkSouth(int p, int w, vector<int> *state, set<int> *frontier) {
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
void SaNode::checkNorth(int p, int w, vector<int> *state, set<int> *frontier) {
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
void SaNode::checkSouthEast(int p, int w, vector<int> *state, set<int> *frontier) {
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
void SaNode::checkNorthEast(int p, int w, vector<int> *state, set<int> *frontier) {
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
void SaNode::checkSouthWest(int p, int w, vector<int> *state, set<int> *frontier) {
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
void SaNode::checkNorthWest(int p, int w, vector<int> *state, set<int> *frontier) {
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



