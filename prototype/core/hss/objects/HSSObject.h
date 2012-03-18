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
 *      Revision: 20
 *
 ********************************************************************/

#ifndef HSSOBJECT_H
#define HSSOBJECT_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "../various/HSSObservable.h"
#include "../parsing/HSSParserNode.h"
#include <boost/unordered_map.hpp>
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include <vector>

namespace AXR {
    
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSObjectType
     *  The type of the object. Some types have specific sub-types that are
     *  specific to their kind, such as gradients or shapes. In those cases
     *  they all share a generic object type.
     */
    enum HSSObjectType
    {
        HSSObjectTypeNone = 0, /**< Error state. */
        HSSObjectTypeGeneric, /**< Abstract type for generic objects, probably error state. */
        HSSObjectTypeValue, /**< Values store values. */
        HSSObjectTypeMultipleValue, /**< Multiple values separated by commas passed to a property are wrapped in these. */
        HSSObjectTypeDisplayObject, /**< Abstract type for display objects, probably error state. */
        HSSObjectTypeContainer, /**< Containers can contain other display objects. */
        HSSObjectTypeTextBlock, /**< Each container has at least one text block. It's task is to render the content text. */
        HSSObjectTypeBorder, /**< All borders share this type */
        HSSObjectTypeMargin, /**< All margins share this type */
        HSSObjectTypeRgb, /**< Colors in RGB */
        HSSObjectTypeGradient, /**< All gradients share this type */
        HSSObjectTypeFont, /**< Not font files, but HSS @font objects */
        HSSObjectTypeFunction, /**< All functions share this type */
        HSSObjectTypeShape, /**< All shapes share this type */
        HSSObjectTypeEvent, /**< All events share this type */
        HSSObjectTypeAction, /**< All actions share this type */
        HSSObjectTypeColorStop, /**< Color stops are used inside gradients */
    };
    /** @} */
    
    class AXRController;
    class HSSMultipleValueDefinition;
    class HSSDisplayObject;
    
    /**
     *  @brief The class from which all HSS objects inherit from. Handles general properties such as
     *  isA.
     *  @see HSSObjectType
     */
    class HSSObject : public HSSObservable, public boost::enable_shared_from_this<HSSObject>
    {
    public:
        /**
         *  The shared pointer to this kind of object.
         */
        typedef boost::shared_ptr<HSSObject> p;
        /**
         *  Convenience iterator for vectors of shared pointers to HSSObjects
         */
        typedef std::vector<p>::iterator it;
        /**
         *  Convenience iterator for vectors of shared pointers to HSSObjects - const version
         */
        typedef std::vector<p>::const_iterator const_it;
        /**
         *  Creates a new object based on a string identifying it's type. Check the HSS specification
         *  for the type names.
         *  @param type A string identifying the type for the new object
         *  @return A shared pointer to the new HSSObject
         */
        static HSSObject::p newObjectWithType(std::string type);
        
        /**
         *  This stores the name of the object.
         *  //FIXME: this should be protected and with getter/setter
         */
        std::string name;
        
        /**
         *  Constructor for HSSObject.
         */
        HSSObject();
        /**
         *  Destructor for HSSObject.
         */
        ~HSSObject();
        /**
         *  Call this to know if a HSSObject subclass is of type otherType.
         *  @param otherType    The HSSObjectType to compare against.
         *  @return Wether the object is an object of given type.
         */
        bool isA(HSSObjectType otherType);
        /**
         *  @return The object type.
         */
        HSSObjectType getType();
        /**
         *  All objects define their own keywords depending on each property, so you need
         *  to call this method if you need to know wether an identifier is a keyword or not.
         *  @param value        The identifier.
         *  @param property     The property to which the identifier is being passed.
         *  @return Wether the identifier is a keyword for the given property.
         */
        virtual bool isKeyword(std::string value, std::string property);
        /**
         *  All objects define their own functions depending on each property, so you need
         *  to call this method if you need to know wether an identifier is a function name
         *  or not. //FIXME: is this really needed?
         *  @param value        The identifier.
         *  @param property     The property to which the identifier is being passed.
         *  @return Wether the identifier is a keyword for the given property.
         */
        virtual bool isFunction(std::string value, std::string property);
        /**
         *  Each object overrides this method to print itself to text when needed.
         *  @return A textual representation of the object.
         */
        virtual std::string toString();
        /**
         *  @return Wether the object is named or is annonymous.
         */
        bool isNamed();
        /**
         *  Sets the name of the object. After calling this, isNamed() will return TRUE.
         *  @param newName     A string with the new name.
         */
        void setName(std::string newName);
        /**
         *  Getter for the name of the object.
         *  @return A string containing the name of the object.
         */
        std::string getName();
        /**
         *  Sets the object to be annomymous. After calling this, the name will be empty and
         *  isNamed() will return FALSE.
         */
        void dropName();
        /**
         *  Each subclass sets its own default object type. This is the object type used when
         *  nesting object defninitions and use just a @.
         *  @return The default HSS object type for this object.
         */
        virtual std::string defaultObjectType();
        /**
         *  Each subclass sets its own default object type for a given property. This is the
         *  object type used when writing just @ while setting a value for a property in HSS.
         *  @param property     The property to which the object definition is being passed.
         *  @return The default HSS object type for the given property, for this object.
         */
        virtual std::string defaultObjectType(std::string property);
        /**
         *  Call this method when you need to get the property name when using shorthand notation
         *  in HSS. This will advance the shorhand index, so each time this method is called,
         *  it will return the property names in order. The order and names returned is specific
         *  to each subclass.
         *  @return The current property name.
         */
        std::string getPropertyForCurrentValue();
        /**
         *  Setter for the shorthand properties array.
         *  @param newValues        A STL vector of property names
         */
        void setShorthandProperties(std::vector<std::string> newValues);
        /**
         *  In HSS, property names already used are excluded from the shorthands.
         *  @param propertyName     The property name to be skipped
         */
        void shorthandSkip(std::string propertyName);
        /**
         *  Increases the shorthand index.
         */
        bool shorthandNext();
        /**
         *  Resets the shorthand index. //FIXME: shouldn't this empty the skipShorthand array too?
         */
        void shorthandReset();
        /**
         *  Getter for the shorthand index.
         *  @return The shorthand index.
         */
        unsigned getShorthandIndex();
        /**
         *  Setter for the shorthand index.
         *  @param newValue     The new shorthand index.
         */
        void setShorthandIndex(unsigned newValue);
        
        /**
         *  Getter for the definition object for the isA property.
         *  @return A shared pointer to the parser node that defines the isA property.
         */
        HSSParserNode::p getDIsA();
        /**
         *  Setter for the definition object for the isA property.
         *  @param value        A shared pointer to the parser node that defines the isA property.
         */
        void setDIsA(HSSParserNode::p value);
        /**
         *  Callback to receive notifications of when the isA property has changed.
         *  @param source   The property which triggers the notification
         *  @param data     A pointer to the data that has been passed along with the notification.
         */
        void isAChanged(HSSObservableProperty source, void*data);
        /**
         *  Convenience method that gets the HSSObservableProperty for a property name in string
         *  form and passes it to setProperty(HSSObservableProperty, HSSParserNode::p).
         *  //FIXME: this might be not necessary to be virtual
         *  @param name     A string containing the property name
         *  @param value    The parser node defining the value for the property
         */
        virtual void setPropertyWithName(std::string name, HSSParserNode::p value);
        /**
         *  Sets the parser node that defines the value of the given property. Each subclass
         *  will call the specific setD<<property>>() method.
         *  @param name     The observable property name
         *  @param value    The parser node defining the value for the property
         */
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        /**
         *  //FIXME: remove this
         */
        virtual void setProperty(HSSObservableProperty name, void* value);
        /**
         *  When a subclass registers a observable property name with a pointer, it can be
         *  retrieved with this method.
         *  //FIXME: change this to return a HSSParserNode::p instead
         *  @param name     The observable property name.
         *  @return The parser node defining the value for the given property.
         */
        virtual void * getProperty(HSSObservableProperty name);
        /**
         *  Each subclass must register each property name it uses with the pointer to the data,
         *  so that it can easily be accessed later.
         *  @param name     The observable property name.
         *  @return The pointer to the data holding the actual value of the property.
         */
        virtual void registerProperty(HSSObservableProperty name, void* property);
        /**
         *  Getter for the current scope this object is operating on.
         *  //FIXME: this is probably a memory management nightmare.
         *  @return A pointer to the vector of shared pointers to display objects
         */
        const std::vector<boost::shared_ptr<HSSDisplayObject> > * getScope() const;
        /**
         *  Setter for the current scope this object is operating on.
         *  //FIXME: this is probably a memory management nightmare.
         *  @param newScope     A pointer to a vector of shared pointers to display objects
         */
        void setScope(const std::vector<boost::shared_ptr<HSSDisplayObject> > * newScope);
        /**
         *  Setter for the controller. The controller needs to be propagated across all
         *  HSSObject subclasses, so they get access to the DOM and such.
         *  @param controller       A pointer to the AXRController that owns this object
         */
        virtual void setController(AXRController * controller);
        /**
         *  Getter for the controller.
         *  @return A pointer to the AXRController that owns this object
         */
        virtual AXRController * getController();
        /**
         *  //FIXME: this is a duplicate of getType(), we probably don't need it.
         *  @return The object type.
         */
        HSSObjectType getObjectType();
        /**
         *  Setter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @param value        A shared pointer to the nearest display object.
         */
        void setThisObj(boost::shared_ptr<HSSDisplayObject> value);
        /**
         *  Getter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @return A shared pointer to the nearest display object.
         */
        boost::shared_ptr<HSSDisplayObject> getThisObj();
        
    protected:
        HSSObjectType type;
        boost::unordered_map<HSSObservableProperty, void*> properties;
        std::vector<std::string> shorthandProperties;
        boost::unordered_map<std::string, bool> skipShorthand;
        unsigned shorthandIndex;
        
        boost::shared_ptr<HSSMultipleValueDefinition> dIsA;
        
        const std::vector<boost::shared_ptr<HSSDisplayObject> > * scope;
        boost::shared_ptr<HSSDisplayObject> thisObj;
        AXRController * axrController;
        
    private:
        bool _isNamed;
    };

}

#endif
