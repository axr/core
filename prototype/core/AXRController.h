/**********************************************************
 *
 *      d8888 Y88b   d88P 8888888b.  
 *      d88888  Y88b d88P  888   Y88b 
 *      d88P888   Y88o88P   888    888 
 *      d88P 888    Y888P    888   d88P 
 *      88P  888    d888b    8888888P"  
 *      d88P   888   d88888b   888 T88b   
 *      d8888888888  d88P Y88b  888  T88b  
 *      d88P     888 d88P   Y88b 888   T88b 
 *      
 *      ARBITRARY·······XML········RENDERING
 *      ====================================
 *
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 **********************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 **********************************************************/

#ifndef AXRCONTROLLER_H
#define AXRCONTROLLER_H

#include "hss/HSSContainer.h"
#include <vector>

namespace AXR {
    class AXRController {
    public:
        AXRController();
        virtual ~AXRController();
        
        virtual std::string toString();
        
        HSSContainer * getRoot();
        //this acquires ownership of the pointer
        void setRoot(HSSContainer * newRoot);
        
        //this acquires ownership of the pointer
        void add(HSSContainer * newContainer);
        HSSContainer * getCurrent();
        void setCurrent(HSSContainer * newCurrent);
        
        //this acquires ownership of the pointer
        void objectTreeAdd(HSSObject * newObject);
        void objectTreeRemove(unsigned index);
        HSSObject * objectTreeGet(unsigned index);
        
    protected:
        HSSContainer * root;
        //this is a weak pointer
        HSSContainer * current;
        
        vector<HSSObject *>objectTree;
    };
}



#endif //AXRCONTROLLER_H