//
//  HSSSelectorChain.cpp
//  mac
//
//  Created by Miro Keller on 16/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSSelectorChain.h"
#include "AXR.h"

HSSSelectorChain::HSSSelectorChain()
{
    
}

HSSSelectorChain::~HSSSelectorChain()
{
    for (unsigned i = 0; i<this->nodeList.size(); i++) {
        delete this->nodeList[i];
    }
    this->nodeList.clear();
}

string HSSSelectorChain::toString()
{
    string tempstr = "HSSSelectorChain with the following nodes:\n";
    int sccount = (int)this->nodeList.size();
    if(sccount > 0){
        
        unsigned i;
        for (i=0; i<sccount; i++) {
            tempstr.append("      ").append(this->nodeList[i]->toString()).append("\n");
        }
    }

    
    return tempstr;
}

void HSSSelectorChain::add(HSSParserNode * newNode)
{
    if(newNode != NULL)
    {
        std_log3("Added node of type " << newNode->toString());
        this->nodeList.push_back(newNode);
    }
}

void HSSSelectorChain::removeLast()
{
    delete nodeList.back();
    this->nodeList.pop_back();
}

HSSParserNode * HSSSelectorChain::last()
{
    return this->nodeList.back();
}

const int HSSSelectorChain::size()
{
    return this->nodeList.size();
}
