//
//  HSSSelector.cpp
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSSelector.h"

HSSSelector::HSSSelector(string elementName)
{
    this->elementName = elementName;
}

string HSSSelector::toString(){
    return "Simple selector targeting an element with name "+this->elementName;
}