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
 *      Last changed: 2012/03/15
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 5
 *
 ********************************************************************/

#include "HSSPercentageConstant.h"
#include <sstream>
#include "../objects/HSSContainer.h"
#include "../objects/HSSContainer.h"

using namespace AXR;

HSSPercentageConstant::HSSPercentageConstant(long double value)
: HSSParserNode()
{
    this->nodeType = HSSParserNodeTypePercentageConstant;
    this->value = value / 100.;
}

HSSPercentageConstant::p HSSPercentageConstant::clone() const{
    return boost::static_pointer_cast<HSSPercentageConstant, HSSClonable>(this->cloneImpl());
}

HSSPercentageConstant::~HSSPercentageConstant()
{
    
}

std::string HSSPercentageConstant::toString()
{
    std::string ret = "HSSPercentageConstant with value ";
    std::ostringstream tempstream;
    //create a temp stream to convert the float to a string
    tempstream << this->value;
    ret.append(tempstream.str());
    ret.append("%");
    return ret;
}

void HSSPercentageConstant::setValue(long double newValue)
{
    this->value = value / 100.;
}

long double HSSPercentageConstant::getValue(long double baseValue)
{
    return this->value * baseValue;
}

HSSClonable::p HSSPercentageConstant::cloneImpl() const{
    return HSSClonable::p(new HSSPercentageConstant(*this));
}

