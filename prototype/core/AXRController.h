//
//  AXRController.h
//  mac
//
//  Created by Miro Keller on 02/04/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef AXRCONTROLLER_H
#define AXRCONTROLLER_H

#include "hss/HSSContainer.h"

namespace AXR {
    class AXRController {
    public:
        AXRController();
        virtual ~AXRController();
        
        virtual std::string toString();
        
        HSSContainer * getRoot();
        //this acquires ownership of the pointer
        void setRoot(HSSContainer * newRoot);
        
        void add(HSSContainer * newContainer);
        HSSContainer * getCurrent();
        void setCurrent(HSSContainer * newCurrent);
        
    protected:
        HSSContainer * root;
        //this is a weak pointer
        HSSContainer * current;
    };
}



#endif //AXRCONTROLLER_H