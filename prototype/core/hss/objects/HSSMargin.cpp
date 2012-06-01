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
 *      Last changed: 2012/03/25
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 5
 *
 *      WARNING!
 *      =================
 *      This class has not been implemented yet. Don't expecti it to work
 *
 ********************************************************************/

#include "HSSMargin.h"

using namespace AXR;

HSSMargin::HSSMargin()
: HSSObject()
{
    this->segments = HSSMultipleValue();
    this->type = HSSObjectTypeMargin;
}

HSSMargin::HSSMargin(const HSSMargin & orig)
: HSSObject(orig)
{
    //fixme?
}

HSSMargin::p HSSMargin::clone() const{
    return boost::static_pointer_cast<HSSMargin, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSMargin::cloneImpl() const{
    return HSSClonable::p(new HSSMargin(*this));
}

HSSMargin::~HSSMargin()
{
    
}

std::string HSSMargin::toString()
{
    return "HSSMargin with size: "+this->size.getStringValue();
}

void HSSMargin::setSize(HSSValue newSize){
    this->size = newSize;
}

HSSValue HSSMargin::getSize()
{
    return this->size;
}




