/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MA:MMD                                                         
 *            :: VD
 *           ::  º                                                         
 *          ::                                                              
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM                   
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      TMMM MMMM                                                           
 *     TMMM  .MMM                                         
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     TMM    MMA       ====================================                              
 *     TMN    MM                               
 *      MN    ZM                       
 *            MM,
 *
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
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/06/12
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 3
 *
 ********************************************************************/

#ifndef HSSMULTIPLEVALUE_H
#define HSSMULTIPLEVALUE_H

#include "HSSObject.h"
#include "../parsing/HSSParserNode.h"
#include <vector>
#include <boost/shared_ptr.hpp>

namespace AXR {
    class HSSMultipleValue : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSMultipleValue> p;
        
        HSSMultipleValue();
        virtual ~HSSMultipleValue();
        virtual std::string toString();
        
        //adds a pointer to newStatement to the properties list
        //this function acquires ownership of the statement
        void add(HSSParserNode::p newValue);
        //removes last statement from the list and then deletes it
        void removeLast();
        //returns a pointer to the last statement in the list
        HSSParserNode::p last();
        //returns how many statements there are in the properties list
        const int size();
        
        const std::vector<HSSParserNode::p> getValueList() const;
        
    protected:
        std::vector<HSSParserNode::p>valueList;
    };
}


#endif