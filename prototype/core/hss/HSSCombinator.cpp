//
//  HSSCombinator.cpp
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSCombinator.h"


HSSCombinator::HSSCombinator(HSSCombinatorType type)
{
    this->type = type;
}

bool HSSCombinator::isA(HSSCombinatorType type)
{
    return this->type == type;
}

string HSSCombinator::toString(){
    return "HSSCombinator of type  "+this->combinatorStringRepresentation(this->type);
}

string HSSCombinator::combinatorStringRepresentation(HSSCombinatorType type){
    string types[20];
	types[ HSSSiblingsCombinator ] =          "HSSSiblingsCombinator";
	types[ HSSPreviousSiblingsCombinator ] =  "HSSPreviousSiblingsCombinator";
	types[ HSSNextSiblingsCombinator ] =      "HSSNextSiblingsCombinator";
	types[ HSSChildrenCombinator ] =          "HSSChildrenCombinator";
	types[ HSSDescendantsCombinator ] =       "HSSDescendantsCombinator";
	types[ HSSAllDescendantsCombinator ] =    "HSSAllDescendantsCombinator";
	types[ HSSTextSelectionCombinator ] =     "HSSTextSelectionCombinator";
    return types[type];
}