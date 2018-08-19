//
//  sanode.h
//  search
//
//  Created by Young Kim on 9/15/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#ifndef sanode_h
#define sanode_h

#include <vector>
#include <set>
#include <map>

#include "input.h"


class SaNode {
    
public:
    
    int cost; // # of no conflicts
    int depth;
    
    int n; // n x x (width x height)
    int q; // num of lizard
    
    SaNode *parent;
    SaNode *next;
    
    std::vector<int> *state; // state space
    std::vector<int> *lizardLoc; // loc idx of all lizards
    std::map<int, int> *conflictMap;
    
    /* Constructor */
    SaNode(int depth, int cost, SaNode *parent, int space, std::vector<int> *state);
    /* Destructor */
    ~SaNode();
    
    
    int getValue(); // num of conflicts
    SaNode *getSuccessor();
    
    void insertLizard(int p);
    void removeLizard(int p);
    
    void updateCost();
    void updateStateSpace();
    
    void printState();
    

    int chooseRandomCandidate();
    int chooseRandomNewCandidate();
    void removeCandidateOnState(int idx, std::vector<int> *state);
    void insertNewCandidateOnState(int idx, std::vector<int> *state);
    
    
private:
    
    /* Helper functions */
    
    int randomLoc(int max);
    
    // find conflict idx at idx p
    int lookEast(int p, int w, std::vector<int> *state);
    int lookWest(int p, int w, std::vector<int> *state);
    int lookSouth(int p, int w, std::vector<int> *state);
    int lookNorth(int p, int w, std::vector<int> *state);
    int lookSouthEast(int p, int w, std::vector<int> *state);
    int lookNorthEast(int p, int w, std::vector<int> *state);
    int lookSouthWest(int p, int w, std::vector<int> *state);
    int lookNorthWest(int p, int w, std::vector<int> *state);
    
    
    // find invalid locations at idx p
    void checkEast(int p, int w, std::vector<int> *state, std::set<int> *frontier);
    void checkWest(int p, int w, std::vector<int> *state, std::set<int> *frontier);
    void checkSouth(int p, int w, std::vector<int> *state, std::set<int> *frontier);
    void checkNorth(int p, int w, std::vector<int> *state, std::set<int> *frontier);
    void checkSouthEast(int p, int w, std::vector<int> *state, std::set<int> *frontier);
    void checkNorthEast(int p, int w, std::vector<int> *state, std::set<int> *frontier);
    void checkSouthWest(int p, int w, std::vector<int> *state, std::set<int> *frontier);
    void checkNorthWest(int p, int w, std::vector<int> *state, std::set<int> *frontier);
    
    
};

// Initial State Space to a Random State
void SaNodeInitializeState(std::vector<int> *state, int n, int p);

int randGenerator(int max);

int idxToCol(int i, int n);
int idxToRow(int i, int n);
int coordToIdx(int row, int col, int n);

// used to identify mirror states
int idxRotate90(int i, int w);
int idxRotate180(int i, int w);
int idxRotate270(int i, int w);
int idxReflectVertical(int i, int w);
int idxReflectHorizontal(int i, int w);

#endif /* sanode_h */
