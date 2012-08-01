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

#include "HSSOddFilter.h"

using namespace AXR;

HSSOddFilter::HSSOddFilter()
: HSSFilter(HSSFilterTypeFirst)
{
    
}

HSSOddFilter::p HSSOddFilter::clone() const{
    return boost::static_pointer_cast<HSSOddFilter, HSSClonable>(this->cloneImpl());
}

HSSOddFilter::~HSSOddFilter()
{
    
}

std::string HSSOddFilter::toString()
{
    return "Odd Filter";
}


const std::vector<HSSDisplayObject::p> HSSOddFilter::apply(const std::vector<HSSDisplayObject::p> &scope, bool processing)
{
    std::vector<HSSDisplayObject::p> ret;
    unsigned i, size;
    for (i=0, size=scope.size(); i<size; i++) {
        if(this->getNegating()){
            if(i%2 != 0){
                ret.push_back(scope[i]);
            } 
        } else {
            if(i%2 == 0){
                ret.push_back(scope[i]);
            }
        }
    }
    return ret;
}

HSSClonable::p HSSOddFilter::cloneImpl() const{
    return HSSClonable::p(new HSSOddFilter(*this));
}
