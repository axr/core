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
 *      Last changed: 2011/04/10
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 3
 *
 ********************************************************************/

#include "HSSMultipleValue.h"
#include "../../AXR.h"
#include <iostream>

using namespace AXR;

HSSMultipleValue::HSSMultipleValue()
{
    
}

HSSMultipleValue::~HSSMultipleValue()
{
    this->valueList.clear();
}

std::string HSSMultipleValue::toString()
{
    std::string tempstr = std::string("HSSMultipleValue");
    
    const int vcount = this->size();
    if(vcount > 0){
        tempstr.append(" with the following values: \n");
        unsigned j;
        for (j=0; j<vcount; j++) {
            tempstr.append("   ").append(this->valueList[j]->toString()).append("\n");
        }
    }
    
    return tempstr;
}

void HSSMultipleValue::add(HSSValue::p newValue)
{
    if(newValue != NULL)
    {
        std_log3("Added value to HSSMultipleValue: " << newValue->toString());
        this->valueList.push_back(newValue);
    }
}

void HSSMultipleValue::removeLast()
{
    this->valueList.pop_back();
}

HSSValue::p HSSMultipleValue::last()
{
    return this->valueList.back();
}

const int HSSMultipleValue::size()
{
    return this->valueList.size();
}