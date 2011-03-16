//
//  HSSCombinator.h
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSCOMBINATOR_H
#define HSSCOMBINATOR_H

#include "HSSParserNode.h"
#include <string>
using namespace std;

enum HSSCombinatorType
{
    HSSSiblingsCombinator,
    HSSPreviousSiblingsCombinator,
    HSSNextSiblingsCombinator,
    HSSChildrenCombinator,
    HSSDescendantsCombinator,
    HSSAllDescendantsCombinator,
    HSSTextSelectionCombinator
};

class HSSCombinator : public HSSParserNode {
    
public:
    HSSCombinatorType type;
    
    static string combinatorStringRepresentation(HSSCombinatorType type);
    
    HSSCombinator(HSSCombinatorType type);
    bool isA(HSSCombinatorType type);
    
    virtual string toString();
    
};

#endif