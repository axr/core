//
//  HSSMargin.h
//  mac
//
//  Created by Miro Keller on 22/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSMARGIN_H
#define HSSMARGIN_H

#include "HSSObject.h"
#include "HSSValue.h"
#include "HSSMultipleValue.h"

class HSSMargin : public HSSObject
{
public:
    HSSMultipleValue segments;
    
    HSSMargin();
    ~HSSMargin();
    virtual string toString();
    
    void setSize(HSSValue newSize);
    HSSValue getSize();
    
protected:
    HSSValue size;
};

#endif