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

#ifndef HSSEVENT_H
#define HSSEVENT_H

#include "HSSObject.h"
#include "../parsing/HSSMultipleValueDefinition.h"
#include "HSSAction.h"

namespace AXR {
    
    /**
     *  @brief Represents all event objects in HSS.
     *  
     *  All event objects, such as \@click, \@keyDown, etc, have the same behavior, they
     *  fire their assigned action, so only one class is needed.
     */
    class HSSEvent : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSEvent> p;
        
        /**
         *  Print the type as a string, useful for logging.
         *  @param  eventType   The event type that will be printed as a string.
         */
        static std::string eventTypeStringRepresentation(HSSEventType eventType);
        
        /**
         *  Constructor for HSSEvent objects.
         *  @param  type    The event type that this event will respond to.
         */
        HSSEvent(HSSEventType type);
        /**
         *  Copy constructor for HSSEvent objects. Do not use directly, use clone() instead.
         */
        HSSEvent(const HSSEvent & orig);
        /**
         *  Clones an instance of HSSEvent and gives a shared pointer of the newly instanciated
         *  object.
         *  @return A shared pointer to the new HSSEvent.
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSEvent();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        
        /**
         *  Allows you to check if this event is of the given type.
         *  @param  type    The event type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSEventType type);
        
        /**
         *  @return The event type of this instance.
         */
        HSSEventType getEventType();
        
        /**
         *  Getter for the actual values of action, HSSAction objects.
         *  @return A vector of shared pointers to the action objects.
         */
        std::vector<HSSAction::p> getAction();
        
        /**
         *  Getter for the definition object of action.
         *  @return A shared pointer to the parser node containing the definition object of action.
         */
        const HSSParserNode::p getDAction() const;
        
        /**
         *  Setter for the definition object of action. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of action.
         */
        void setDAction(HSSParserNode::p value);
        
        /**
         *  Since this property accepts multiple values, this allows to append a value instead of replacing
         *  the whole thing.
         *  @param value    A shared pointer to the parser node containing the definition object to be
         *                  added to action.
         */
        void addDAction(HSSParserNode::p value);
        
        /**
         *  Method to be passed as callback when observing changes that will affect action.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void actionChanged(HSSObservableProperty source, void*data);
        
        /**
         *  Executes the action which is stored in this event object.
         */
        virtual void fire();
        
    protected:
        HSSEventType eventType;
        
        //action
        HSSParserNode::p dAction;
        HSSObservable * observedAction;
        HSSObservableProperty observedActionProperty;
        std::vector<HSSAction::p> action;
        
    private:
        HSSClonable::p cloneImpl() const;
    };
}


#endif
