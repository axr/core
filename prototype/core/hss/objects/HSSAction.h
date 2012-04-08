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
 *      Revision: 2
 *
 ********************************************************************/

#ifndef HSSACTION_H
#define HSSACTION_H

#include "HSSObject.h"
#include "../parsing/HSSMultipleValueDefinition.h"

namespace AXR {
    
    enum HSSActionType
    {
        HSSActionTypeNone = 0,
        HSSActionTypeRequest,
        HSSActionTypeSetProperty,
        HSSActionTypeJavascript,
        HSSActionTypeJsFunction,
        HSSActionTypeAlert,
        HSSActionTypeLog,
        HSSActionTypeFlag
    };
    
    class HSSAction : public HSSObject
    {
    public:
        
        typedef boost::shared_ptr<HSSAction> p;
        
        static std::string actionTypeStringRepresentation(HSSActionType actionType);
        
        /**
         *  Constructor for HSSAction objects
         *  @param  type    The HSSActionType that this action corresponds to. Usually set
         *                  from withing a subclass' constructor method.
         */
        HSSAction(HSSActionType type);
        /**
         *  Copy constructor for HSSAction objects
         */
        HSSAction(const HSSAction & orig);
        /**
         *  Clones an instance of HSSAction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSAction
         */
        p clone() const;
        
        virtual ~HSSAction();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        bool isA(HSSActionType type);
        HSSActionType getActionType();
        
        virtual void fire();
        
        
    protected:
        HSSActionType actionType;
        
    private:
        HSSClonable::p cloneImpl() const;
    };
}


#endif
