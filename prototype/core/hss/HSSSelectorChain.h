//
//  HSSSelectorChain.h
//  mac
//
//  Created by Miro Keller on 16/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSSELECTORCHAIN_H
#define HSSSELECTORCHAIN_H

#include "HSSParserNode.h"
#include <string>
#include <vector>
using namespace std;

class HSSSelectorChain : public HSSParserNode
{
public:
    HSSSelectorChain();
    ~HSSSelectorChain();
    string toString();
    //adds a pointer to newNode to the selector chain
    //this function acquires ownership of the parser node
    void add(HSSParserNode * newNode);
    //removes last newNode from the chain and then deletes it
    void removeLast();
    //returns a pointer to the last node in the chain
    HSSParserNode * last();
    //returns how many nodes there are in the selector chain
    const int size();
    
protected:
    vector<HSSParserNode *> nodeList;
};

#endif