//
//  HSSContainer.cpp
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSContainer.h"

string HSSContainer::toString()
{
    if (this->isNamed()) {
        return string("HSSContainer: ").append(this->name);
    } else {
        return "Annonymous HSSContainer";
    }
}