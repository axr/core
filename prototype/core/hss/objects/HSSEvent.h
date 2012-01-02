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
 *      Last changed: 2011/11/08
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 2
 *
 ********************************************************************/

#ifndef HSSEVENT_H
#define HSSEVENT_H

#include "HSSObject.h"
#include "../parsing/HSSMultipleValueDefinition.h"
#include "HSSAction.h"

namespace AXR {
    
    enum HSSEventType
    {
        HSSEventTypeNone = 0,
        HSSEventTypeLoad,
        HSSEventTypeClick,
        HSSEventTypeDoubleClick,
        HSSEventTypeTripleClick,
        HSSEventTypeMouseDown,
        HSSEventTypeMouseUp,
        HSSEventTypeMouseOver,
        HSSEventTypeMouseOut,
        HSSEventTypeMouseHold,
        HSSEventTypeMouseMove,
        HSSEventTypeClickSecondary,
        HSSEventTypeClickTertiary,
        HSSEventTypeScroll
    };
    
    class HSSEvent : public HSSObject
    {
    public:
        
        static std::string eventTypeStringRepresentation(HSSEventType eventType);
        
        HSSEvent(HSSEventType type);
        virtual ~HSSEvent();
        
        typedef boost::shared_ptr<HSSEvent> p;
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        bool isA(HSSEventType type);
        HSSEventType getEventType();
        
        //action
        std::vector<HSSAction::p> getAction();
        const HSSParserNode::p getDAction() const;
        void setDAction(HSSParserNode::p value);
        void addDAction(HSSParserNode::p value);
        void actionChanged(HSSObservableProperty source, void*data);
        
        virtual void fire();
        
        
    protected:
        HSSEventType eventType;
        
        
        //action
        HSSParserNode::p dAction;
        HSSObservable * observedAction;
        HSSObservableProperty observedActionProperty;
        std::vector<HSSAction::p> action;
        
        
    };
}


#endif
