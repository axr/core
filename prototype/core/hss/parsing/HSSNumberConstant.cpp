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
 ********************************************************************/

#include <sstream>
#include "HSSNumberConstant.h"

using namespace AXR;

HSSNumberConstant::HSSNumberConstant(long double value)
: HSSParserNode(HSSParserNodeTypeNumberConstant)
{
    this->value = value;
}

HSSNumberConstant::p HSSNumberConstant::clone() const{
    return boost::static_pointer_cast<HSSNumberConstant, HSSClonable>(this->cloneImpl());
}

HSSNumberConstant::~HSSNumberConstant()
{
    
}

void HSSNumberConstant::setValue(long double newValue)
{
    this->value = newValue;
}

long double HSSNumberConstant::getValue()
{
    return this->value;
}

std::string HSSNumberConstant::toString()
{
    std::string ret = "HSSNumberConstant with value ";
    std::ostringstream tempstream;
    //create a temp stream to convert the float to a string
    tempstream << this->value;
    ret.append(tempstream.str());
    return ret;
}

HSSClonable::p HSSNumberConstant::cloneImpl() const{
    return HSSClonable::p(new HSSNumberConstant(*this));
}


