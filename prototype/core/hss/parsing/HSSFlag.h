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
 *      Last changed: 2012/03/21
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 1
 *
 ********************************************************************/

#ifndef HSSFLAG_H
#define HSSFLAG_H

#include "HSSParserNode.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "HSSRule.h"

namespace AXR {
    
    class HSSSelectorChain;
    class HSSDisplayObject;
    
    class HSSFlag : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSFlag> p;
        
        HSSFlag();
        p clone() const;
        virtual ~HSSFlag();
        
        virtual std::string toString();
        
        std::string getName();
        void setName(std::string newValue);
        void apply(boost::shared_ptr<HSSDisplayObject> displayObject, bool negating);
        
        void flagChanged(HSSRuleState newStatus);
        
        HSSFlag::p shared_from_this();
        const std::vector<boost::shared_ptr<HSSDisplayObject> > filter(const std::vector<boost::shared_ptr<HSSDisplayObject> > &scope, bool negating);
        
        bool isPurging();
        void setPurging(bool newValue);

    protected:
        std::string _name;
        
    private:
        virtual HSSClonable::p cloneImpl() const;
        bool _purging;
    };
}

#endif
