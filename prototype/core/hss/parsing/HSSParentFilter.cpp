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
 *      AUTHORS: Miro Keller, Anando Gopal Chetterjee
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
 *      Last changed: 2012/06/28
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 3
 *
 ********************************************************************/

#include "HSSParentFilter.h"
#include "../objects/HSSContainer.h"

using namespace AXR;

HSSParentFilter::HSSParentFilter()
: HSSFilter()
{
    this->filterType = HSSFilterTypeParent;
}

HSSParentFilter::p HSSParentFilter::clone() const{
    return boost::static_pointer_cast<HSSParentFilter, HSSClonable>(this->cloneImpl());
}

HSSParentFilter::~HSSParentFilter()
{
    
}

std::string HSSParentFilter::toString()
{
    return "Parent Filter";
}


const std::vector<HSSDisplayObject::p> HSSParentFilter::apply(const std::vector<HSSDisplayObject::p> &scope, bool negating)
{
    if(scope.size() > 0){
        std::vector<HSSDisplayObject::p> ret;
        int size=scope.size();
        
        if (!negating)
            for (int i=0; i<size; i++)
                if (HSSContainer::asContainer(scope[i])->getChildren(false).size()>0)
                    ret.push_back(scope[i]);

        else
            for (int i=0; i<size; i++)
                if (HSSContainer::asContainer(scope[i])->getChildren (false).size()==0)
                    ret.push_back(scope[i]);
        return ret;
    } else {
        return scope;
    }
}

HSSClonable::p HSSParentFilter::cloneImpl() const{
    return HSSClonable::p(new HSSParentFilter(*this));
}
