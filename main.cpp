//
//  main.cpp
//  search
//
//  Created by Young Kim on 9/4/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

#include <utility> // std::pair
#include <vector>
#include <set>
#include <queue> // std::queue
#include <list> // std::queue

#include <sys/time.h>

#include <math.h>       /* log */

#include "input.h"
#include "node.h"
#include "sanode.h"

using namespace std;

#include <stdlib.h>     /* srand, rand */

const int bigE = 2.71828182845904523536; // e

Node* isExist(Node *n, list<Node*> *q) {
    list<Node*>::iterator it;
    for(it = q->begin(); it != q->end(); ++it) {
        if(n->isEqualState((*it))) {
            return (*it);
        }
    }
    return NULL;
}

string toString(vector<int> *v, int w, int flag) {
    string out = "";
    // case 1: success, find solution
    if(flag == 0) {
        out += "OK\n";
        
        for(int i = 0; i < v->size(); i++) {
            
            if((*v)[i] == -1) {
                out += "0";
            } else if((*v)[i] == 1) {
                out += "1";
            } else if((*v)[i] == 2){
                out += "2";
            } else {
                out += "0";
            }
            
            if((i % w) == (w - 1)) {
                out += "\n";
            }
        }
    // case 2: fail, no solution
    } else {
        out += "FAIL\n";
    }
    return out;
}

void outputGoalNode(Node *goal, int w) {
    string output;
    if(goal != NULL) {
        cout << "found goal!" << endl;
        output = toString(goal->state, w, 0);
    } else {
        cout << "found none!" << endl;
        output = toString(NULL, w, 1);
    }
    cout << output << endl;
    
    writeFile("output.txt", output);
}

int saSearch(Input &input) {
    
    // define initial state space
    int len = input.n * input.n;
    vector<int> *state = new vector<int>(len, 0);
    for(int i = 0; i < input.tree.size(); i++) {
        int loc = (input.tree)[i];
        (*state)[loc] = 2;
    }
    
    
    return 0;
}


double scheduler(int t) {
    /* double init = 5;
    return (init * pow(0.988, t)); */
    
    if(t == 0 || t == 1) {
        return 10;
    } else {
        return (double) (1.0/log((t*1.00000001)));
    }
}

bool pdf(int deltaE, double bigT) {
    double p;
    if(bigT > 0.0000001) {
        p = pow(bigE, (deltaE / bigT));
    } else {
        p = 0;
    }

    int r = rand() % 2;
    
    if(p < 0.0000001) {
        p = 0;
    }
    
    cout << "p = " << p << endl;
    cout << "r = " << r << endl;
    
    if(r < p) {
        cout << "return true with p = " << p << endl;
        return true;
    } else {
        cout << "return false with p = " << p << endl;
        return false;
    }
}

int main(int argc, const char * argv[]) {
    
    srand((unsigned int) time(NULL));
    
    Input input;
    readFile("input.txt", input);
    
    cout << "MSG: Printing values from input.txt" << endl;
    cout << "\t type: " << input.type << endl;
    cout << "\t n: " << input.n << endl;
    cout << "\t p: " << input.p << endl;
    if(input.tree.size() > 0) {
        for(int i = 0; i < input.tree.size(); i++) {
            cout << "\t tree at: " << (input.tree)[i] << endl;
        }
    }
    
    
    int queueType;
    const char* bfs = "BFS";
    const char* dfs = "DFS";
    const char* sa = "SA";
    
    
    if(strncmp(input.type.c_str(), bfs, 3) == 0) {
        queueType = 1;
    } else if(strncmp(input.type.c_str(), dfs, 3) == 0) {
        queueType = 2;
    } else if(strncmp(input.type.c_str(), sa, 2) == 0){
        queueType = 3;
    } else {
        cout << "DEBUG: Error in search type (BFS, DFS, SA)!" << endl;
        return -1;
    }
    
    Node *goal = NULL;
    
    // define initial state space
    int len = input.n * input.n;
    vector<int> *state = new vector<int>(len, 0);
    for(int i = 0; i < input.tree.size(); i++) {
        int loc = (input.tree)[i];
        (*state)[loc] = 2;
    }
    
    
    // simulated annealing
    if(queueType == 3) {
        
        struct timeval t1, t2;
        gettimeofday(&t1, NULL); // start timer
        
        
        cout << "TESTING SA SA SA!! PREMATURE RETURN 5" << endl;
        
        SaNodeInitializeState(state, input.n, input.p);
        
        SaNode *root = new SaNode(0, 0, NULL, input.n, state);
        root->updateCost();
        root->printState();
        
        cout << "Testing conflicts!" << endl;
        cout << "num of conflicts = " << root->getValue() << endl;
        
        cout << "TESSTING LOOP" << endl;
        /* SaNode *next = root->getSuccessor();
        next->printState();
        cout << "num of conflicts = " << next->getValue() << endl; */

        
        
        
        SaNode *curr = root;
        int currConflict = curr->getValue();
        double bigT = 0;
        
        
        int goodChange = 0;
        int badChangeHighProb = 0;
        int badChangeLowProb = 0;
        
        for(int i = 0; ; i++) {
            cout << "=== LOOP === " << i << endl;
            
            gettimeofday(&t2, NULL); // end timer
            long delta = t2.tv_sec - t1.tv_sec;
            cout << "time = " << delta << " sec " << endl;
            
            /*if(delta >= 60) {
                cout << scheduler(i, input.n, delta);
                return -1;
            }*/
            
            bigT = scheduler(i);
            
            if(currConflict == 0 || delta > 250) {
                curr->printState();
                
                if(currConflict == 0) {
                    cout << "YES!! FOUND SOL!!" << endl;
                } else {
                    cout << "NO!! FAIL excceded time limit!!" << endl;
                }
                
                cout << "Printing STATS: " << endl;
                cout << "good change = " << goodChange << endl;
                cout << "bad change with HIGH P = " << badChangeHighProb << endl;
                cout << "bad change with LOW P = " << badChangeLowProb << endl;
                
                return 0;
            }
            
            SaNode *next = curr->getSuccessor();
            
            int nextVal = (input.n * input.n) - next->getValue();
            int currVal = (input.n * input.n) - curr->getValue();
            
            cout << "next cost = " << next->getValue() << " curr cost = " << currConflict << endl;
            
            int deltaE = nextVal - currVal;
            
            cout << "deltaE = " << deltaE << endl;
            cout << "bigT = " << bigT << endl;
            cout << "pdf(deltaE, bigT) = " << endl;
            
            if(deltaE > 0) {
                cout << "lower cost change" << endl;
                curr = next;
                currConflict = curr->getValue();
                
                goodChange++;
            } else {
                if(pdf(deltaE, bigT)) {
                    cout << "change even if bad" << endl;
                    curr = next;
                    currConflict = curr->getValue();
                    badChangeHighProb++;
                } else {
                    // no change
                    cout << "no change" << endl;
                    badChangeLowProb++;
                }
            }
        }
        
        
        
        return 5;
    }
    
    
    
    
    
    
    
    // create search queue
    list<Node*> *searchQueue = new list<Node*>();
    list<Node*> *closedQueue = new list<Node*>();
    
    // create root node
    Node *root = new Node(-1, 0, 0, NULL, input.n, state);
    
    delete state;
    root->printState();
    
    cout << "MSG: Push Root Node" << endl;
    // push root node into inital search queue
    searchQueue->push_front(root); // BFS
    
    
    struct timeval t1, t2;
    double elapsedTime = 0;
    gettimeofday(&t1, NULL); // start timer
    
    int flag = 0;
    int c = 0;
    while(flag != 1) {
        cout << "q size = " << searchQueue->size() << endl;
        
        if(searchQueue->empty()) {
            cout << "DEBUG: Search queue is empty!" << endl;
            cout << "FAIL!!!" << endl;
            goal = NULL;
            
            break;
        } else {
            // remove front node from search queue
            Node *n = searchQueue->front();
            searchQueue->pop_front();
            
            n->printNode();

            // apply goal test to node
            if(n->goalTest() == input.p) {
                cout << "MSG: goalTest SUCCEESS!!!" << endl;
                flag = 1;
                goal = n;
            // if goal test fail -> expand node
            } else {
                
                closedQueue->push_back(n); // already explored queue
                
                int numExpanded = n->expandNode();
                cout << "DEBUG: num nodes expanded = " << numExpanded << endl;
                
                // enqueue expanded (children) nodes to search queue
                vector<Node*> *children = n->children;
                for(int i = 0; i < children->size(); i++) {
                    Node *child = (*children)[i];
                    
                    Node *existInSearch = isExist(child, searchQueue);
                    Node *existInClosed = isExist(child, closedQueue);
                    
                    // case 1: no node in search or closed
                    if(existInSearch == NULL && existInClosed == NULL) {
                        
                        if(queueType == 1) {
                            searchQueue->push_back(child); // BFS
                        } else if(queueType == 2) {
                            searchQueue->push_front(child); // DFS
                        }
                        
                    // case 2: child node exist in search queue -> same depth with mirror or rotation
                    } else if(existInSearch != NULL) {
                        
                        // child num of conflicts < exist num of conflicts, remove exist
                        // small num of conflicts at same depth, more lizard locations
                        
                        if((child->cost) < (existInSearch->cost)) {
                            searchQueue->remove(existInSearch);
                            // delete existInSearch;
                            
                            if(queueType == 1) {
                                searchQueue->push_back(child); // BFS
                            } else if(queueType == 2) {
                                searchQueue->push_front(child); // DFS
                            }
                        }
                    // case 3: child node exist in closed queue (already visited)
                    } else if(existInClosed != NULL) {
                        
                        // there exist a child with equal state that we've visit
                        // but that child has better cost then add to the search queue
                        
                        if((child->cost) < (existInClosed->cost)) {
                            closedQueue->remove(existInClosed);
                            // delete existInClosed;
                            
                            if(queueType == 1) {
                                searchQueue->push_back(child); // BFS
                            } else if(queueType == 2) {
                                searchQueue->push_front(child); // DFS
                            }
                        }
                    }
                }
                cout << "i = " << c << " num child nodes added to search queue = " << searchQueue->size() << endl;
                list<Node*>::iterator it;
                for(it = searchQueue->begin(); it != searchQueue->end(); ++it) {
                    cout << (*it)->nodeID << " ";
                }
                cout << endl;
            }
        }
        c++;
    }
    
    
    if(goal != NULL) {
        cout << endl;
        goal->printNode();
        cout << endl;
    }
    
    
    cout << "size of searchQueue = " << searchQueue->size() << endl;
    cout << "size of closedQueue = " << closedQueue->size() << endl;
    
    
    gettimeofday(&t2, NULL); // end timer
    long delta = t2.tv_sec - t1.tv_sec;
    elapsedTime += delta;   // us to ms
    cout << "STATUS: elapsedTime = " << elapsedTime << endl;
    
    // write goal node to output.txt
    outputGoalNode(goal, input.n);
    
    delete searchQueue;
    delete closedQueue;
    
    
    return 0;
}
