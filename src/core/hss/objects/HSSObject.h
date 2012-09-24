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
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
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

#ifndef HSSOBJECT_H
#define HSSOBJECT_H

#include <vector>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <QMap>
#include "AXRDebugging.h"
#include "HSSObservable.h"
#include "HSSParserNode.h"
#include "HSSTypeEnums.h"

namespace AXR
{
    class AXRController;
    class HSSMultipleValueDefinition;
    class HSSDisplayObject;

    /**
     *  @brief The class from which all HSS objects inherit from. Handles general properties such as
     *  isA.
     *  @see HSSObjectType
     */
    class AXR_API HSSObject : public HSSObservable, public HSSClonable, public boost::enable_shared_from_this<HSSObject>
    {
    public:
        /**
         *  The shared pointer to this kind of object.
         */
        typedef boost::shared_ptr<HSSObject> p;
        /**
         *  Convenience iterator for vectors of shared pointers to this kind of object.
         */
        typedef std::vector<p>::iterator it;
        /**
         *  Convenience iterator for vectors of shared pointers to this kind of object - const version
         */
        typedef std::vector<p>::const_iterator const_it;
        /**
         *  Creates a new object based on a string identifying it's type. Check the HSS specification
         *  for the type names.
         *  @param type A string identifying the type for the new object
         *  @return A shared pointer to the new HSSObject
         */
        static HSSObject::p newObjectWithType(AXRString type);

        /**
         *  This stores the name of the object.
         *
         *  @todo this should be protected and with getter/setter
         */
        AXRString name;

        /**
         *  Constructor for HSSObject.
         *  @param type     The type of this object subclass for identification purposes.
         */
        HSSObject(HSSObjectType type);
        /**
         *  Copy constructor for HSSObject. Doesn't copy registered properties or shorhand positions. If your
         *  subclass needs that, you have to copy them yourself after the construction.
         */
        HSSObject(const HSSObject & orig);
        /**
         *  Clones an instance of HSSObject and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSObject
         */
        p clone() const;
        /**
         *  Destructor for HSSObject.
         */
        ~HSSObject();
        /**
         *  All objects define their own keywords depending on each property, so you need
         *  to call this method if you need to know wether an identifier is a keyword or not.
         *  @param value        The identifier.
         *  @param property     The property to which the identifier is being passed.
         *  @return Wether the identifier is a keyword for the given property.
         */
        virtual bool isKeyword(AXRString value, AXRString property);
        /**
         *  All objects define their own functions depending on each property, so you need
         *  to call this method if you need to know wether an identifier is a function name
         *  or not.
         *  @param value        The identifier.
         *  @param property     The property to which the identifier is being passed.
         *  @return Wether the identifier is a keyword for the given property.
         *
         *  @todo is this really needed?
         */
        virtual bool isFunction(AXRString value, AXRString property);
        /**
         *  Each object overrides this method to print itself to text when needed.
         *  @return A textual representation of the object.
         */
        virtual AXRString toString();
        /**
         *  @return Wether the object is named or is annonymous.
         */
        bool isNamed();
        /**
         *  Sets the name of the object. After calling this, isNamed() will return TRUE.
         *  @param newName     A string with the new name.
         */
        void setName(AXRString newName);
        /**
         *  Getter for the name of the object.
         *  @return A string containing the name of the object.
         */
        AXRString getName();
        /**
         *  Sets the object to be annomymous. After calling this, the name will be empty and
         *  isNamed() will return FALSE.
         */
        void dropName();
        /**
         *  Each subclass sets its own default object type. This is the object type used when
         *  nesting object defninitions and use just a @ in HSS.
         *  @return The default HSS object type for this object.
         */
        virtual AXRString defaultObjectType();
        /**
         *  Each subclass sets its own default object type for a given property. This is the
         *  object type used when writing just @ while setting a value for a property in HSS.
         *  @param property     The property to which the object definition is being passed.
         *  @return The default HSS object type for the given property, for this object.
         */
        virtual AXRString defaultObjectType(AXRString property);
        /**
         *  Call this method when you need to get the property name when using shorthand notation
         *  in HSS. This will advance the shorhand index, so each time this method is called,
         *  it will return the property names in order. The order and names returned is specific
         *  to each subclass.
         *  @return The current property name.
         */
        AXRString getPropertyForCurrentValue();
        /**
         *  Setter for the shorthand properties array.
         *  @param newValues        A STL vector of property names
         */
        void setShorthandProperties(std::vector<AXRString> newValues);
        /**
         *  In HSS, property names already used are excluded from the shorthands.
         *  @param propertyName     The property name to be skipped
         */
        void shorthandSkip(AXRString propertyName);
        /**
         *  Increases the shorthand index.
         */
        bool shorthandNext();
        /**
         *  Resets the shorthand index.
         *
         *  @todo shouldn't this empty the skipShorthand array too?
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
        void addDIsA(HSSParserNode::p value);

        /**
         *  Callback to receive notifications of when the isA property has changed.
         *  @warning Currently unimplemented
         *  @param source   The property which triggers the notification
         *  @param data     A pointer to the data that has been passed along with the notification.
         */
        void isAChanged(HSSObservableProperty source, void*data);
        /**
         *  Convenience method that gets the HSSObservableProperty for a property name in string
         *  form and passes it to setProperty(HSSObservableProperty, HSSParserNode::p).
         *  @param name     A string containing the property name
         *  @param value    The parser node defining the value for the property
         *
         *  @todo this might be not necessary to be virtual
         */
        virtual void setPropertyWithName(AXRString name, HSSParserNode::p value);
        /**
         *  Sets the parser node that defines the value of the given property. Each subclass
         *  will call the specific setD\<\<property\>\>() method.
         *  @param name     The observable property name
         *  @param value    The parser node defining the value for the property
         */
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        /**
         *  @todo remove this if not used
         */
        virtual void setProperty(HSSObservableProperty name, boost::any);
        /**
         *  When a subclass registers a observable property name with a pointer, it can be
         *  retrieved with this method.
         *  @param name     The observable property name.
         *  @return The parser node defining the value for the given property.
         */
        virtual boost::any getProperty(HSSObservableProperty name);
        /**
         *  Each subclass must register each property name it uses with the pointer to the data,
         *  so that it can easily be accessed later.
         *  @param name     The observable property name.
         *  @param property A boost any wrapping the pointer to the data holding the actual value of the property.
         */
        virtual void registerProperty(HSSObservableProperty name, boost::any property);
        /**
         *  Getter for the current scope this object is operating on.
         *  @return A pointer to the vector of shared pointers to display objects
         *
         *  @todo this is probably a memory management nightmare
         */
        const std::vector<boost::shared_ptr<HSSDisplayObject> > * getScope() const;
        /**
         *  Setter for the current scope this object is operating on.
         *  @param newScope     A pointer to a vector of shared pointers to display objects
         *
         *  @todo this is probably a memory management nightmare.
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


        /**
         *  Call this to know if a HSSObject subclass is of type otherType.
         *  @param otherType    The HSSObjectType to compare against.
         *  @return Wether the object is an object of given type.
         */
        bool isA(HSSObjectType otherType);
        /**
         *  @return The object type.
         */
        HSSObjectType getObjectType();

        virtual bool isA(HSSShapeType otherType);
        virtual HSSShapeType getShapeType();

        virtual bool isA(HSSRequestModeType otherType);
        virtual HSSRequestModeType getRequestModeType();

        virtual bool isA(HSSEventType otherType);
        virtual HSSEventType getEventType();

        virtual bool isA(HSSActionType otherType);
        virtual HSSActionType getActionType();

    protected:
        QMap<HSSObservableProperty, boost::any> properties;
        std::vector<AXRString> shorthandProperties;
        QMap<AXRString, bool> skipShorthand;
        unsigned shorthandIndex;

        HSSParserNode::p dIsA;

        const std::vector<boost::shared_ptr<HSSDisplayObject> > * scope;
        boost::shared_ptr<HSSDisplayObject> thisObj;
        AXRController * axrController;

    private:
        HSSObjectType type;
        bool _isNamed;

        virtual HSSClonable::p cloneImpl() const;

    };
}

#endif
