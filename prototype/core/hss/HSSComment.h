//
//  HSSComment.h
//  mac
//
//  Created by Miro Keller on 28/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSCOMMENT_H
#define HSSCOMMENT_H

#include "HSSStatement.h"

class HSSComment : public HSSStatement {
    string value;
    
public:
    HSSComment(string value);
    string getValue();
    void setValue(string newValue);
    virtual string toString();
};

#endif