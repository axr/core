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
 *      Revision: 2
 *
 ********************************************************************/

#ifndef HSSREQUEST_H
#define HSSREQUEST_H

#include "HSSAction.h"
#include "HSSContainer.h"

namespace AXR {
    
    enum HSSRequestModeType
    {
        HSSRequestModeTypeAuto,
        HSSRequestModeTypeReplace,
        HSSRequestModeTypeWrap,
        HSSRequestModeTypePrepend,
        HSSRequestModeTypeAppend,
        HSSRequestModeTypeReturn,
    };
    
    class HSSRequest : public HSSAction
    {
    public:
        /**
         *  Constructor for HSSRequest objects
         */
        HSSRequest();
        /**
         *  Copy constructor for HSSRequest objects
         */
        HSSRequest(const HSSRequest & orig);
        /**
         *  Clones an instance of HSSRequest and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRequest
         */
        p clone() const;
        virtual ~HSSRequest();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        virtual void fire();
        
        //src
        HSSParserNode::p getDSrc();
        void setDSrc(HSSParserNode::p);
        void srcChanged(HSSObservableProperty source, void*data);
        
        //target
        HSSParserNode::p getDTarget();
        void setDTarget(HSSParserNode::p);
        void targetChanged(HSSObservableProperty source, void*data);
        
//        //mode
//        HSSParserNode::p getDMode();
//        void setDMode(HSSParserNode::p);
//        void modeChanged(HSSObservableProperty source, void*data);
//        
//        //contentTarget
//        HSSParserNode::p getDContentTarget();
//        void setDContentTarget(HSSParserNode::p);
//        void contentTargetChanged(HSSObservableProperty source, void*data);
        
        
    private:
        std::string src;
        HSSParserNode::p dSrc;
        HSSObservable * observedSrc;
        HSSObservableProperty observedSrcProperty;
        
        HSSParserNode::p dTarget;
        HSSObservable * observedTarget;
        HSSObservableProperty observedTargetProperty;
        std::vector<HSSDisplayObject::p> target;
        
        HSSRequestModeType mode;
        
    private:
        HSSClonable::p cloneImpl() const;
    };
}

#endif
