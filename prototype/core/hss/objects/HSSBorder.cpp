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
 *      Last changed: 2011/10/09
 *      HSS version: 1.0
 *      Core version: 0.4
 *      Revision: 7
 *
 ********************************************************************/

#include "HSSBorder.h"

using namespace AXR;

HSSBorder::HSSBorder()
:HSSObject()
{
    this->type = HSSObjectTypeBorderGeneric;
}

HSSBorder::~HSSBorder()
{
    
}

std::string HSSBorder::toString()
{
    if (this->isNamed()) {
        return std::string("HSSBorder: ").append(this->name);
    } else {
        return "Annonymous HSSBorder";
    }
}

std::string HSSBorder::defaultObjectType()
{
    return "border";
}

std::string HSSBorder::defaultObjectType(std::string property)
{
    if (property == "effects"){
        return "shadow";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSBorder::isKeyword(std::string value, std::string property)
{
    if (value == "inside" || value == "centered" || value == "outside"){
        if (property == "position") {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}



