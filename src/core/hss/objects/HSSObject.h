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
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
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
#include <QMap>
#include <QSharedPointer>
#include <QVariant>
#include <QWeakPointer>
#include "HSSClonable.h"
#include "HSSObservable.h"
#include "HSSTypeEnums.h"

namespace AXR
{
    class AXRController;
    class HSSMultipleValueDefinition;
    class HSSDisplayObject;
    class HSSParserNode;
    class HSSSimpleSelection;

    /**
     *  @brief The class from which all HSS objects inherit from. Handles general properties such as
     *  isA.
     *  @see HSSObjectType
     */
    class AXR_API HSSObject : public HSSObservable, public HSSClonable, public QObject
    {
    public:
        /**
         *  Convenience iterator for vectors of shared pointers to this kind of object.
         */
        typedef std::vector<QSharedPointer<HSSObject> >::iterator it;
        /**
         *  Convenience iterator for vectors of shared pointers to this kind of object - const version
         */
        typedef std::vector<QSharedPointer<HSSObject> >::const_iterator const_it;
        /**
         *  Creates a new object based on a string identifying it's type. Check the HSS specification
         *  for the type names.
         *  @param type A string identifying the type for the new object
         *  @return A shared pointer to the new HSSObject
         */
        static QSharedPointer<HSSObject> newObjectWithType(AXRString type, AXRController * controller);

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
        HSSObject(HSSObjectType type, AXRController * controller);
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
        QSharedPointer<HSSObject> clone() const;
        /**
         *  Destructor for HSSObject.
         */
        virtual ~HSSObject();
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
         * @return A map representing the object and it's properties
         */
        virtual QVariantMap toMap() const;
        /**
         *  @return Wether the object is named or is anonymous.
         */
        bool isNamed() const;
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
         *  Getter for the list of computed values for all properties.
         *  @return A map containing variants with pointers to values.
         */
        const QMap<HSSObservableProperty, QVariant> getProperties() const;
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
        size_t getShorthandIndex() const;
        /**
         *  Setter for the shorthand index.
         *  @param newValue     The new shorthand index.
         */
        void setShorthandIndex(size_t newValue);

        /**
         *  Getter for the definition object for the isA property.
         *  @return A shared pointer to the parser node that defines the isA property.
         */
        QSharedPointer<HSSParserNode> getDIsA();
        /**
         *  Setter for the definition object for the isA property.
         *  @param value        A shared pointer to the parser node that defines the isA property.
         */
        void setDIsA(QSharedPointer<HSSParserNode> value);
        void addDIsA(QSharedPointer<HSSParserNode> value);

        /**
         *  Callback to receive notifications of when the isA property has changed.
         *  @warning Currently unimplemented
         *  @param source   The property which triggers the notification
         *  @param data     A pointer to the data that has been passed along with the notification.
         */
        void isAChanged(HSSObservableProperty source, void*data);
        /**
         *  Takes a vector of property names in AXRString form, converts them to observable property
         *  names and passes each to setProperty(HSSObservableProperty, QSharedPointer<HSSParserNode>).
         *  @param name     A vector of strings containing the property names
         *  @param value    The parser node defining the value for the property
         *
         *  @todo this might be not necessary to be virtual
         */
        virtual void setPropertiesWithNames(QVector<AXRString> names, QSharedPointer<HSSParserNode> value);
        /**
         *  Sets the parser node that defines the value of the given property. Each subclass
         *  will call the specific setD\<\<property\>\>() method.
         *  @param name     The observable property name
         *  @param value    The parser node defining the value for the property
         */
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);
        /**
         *  @todo remove this if not used
         */
        virtual void setProperty(HSSObservableProperty name, QVariant);
        /**
         *  When a subclass registers a observable property name with a pointer, it can be
         *  retrieved with this method.
         *  @param name     The observable property name.
         *  @return The parser node defining the value for the given property.
         */
        virtual QVariant getProperty(HSSObservableProperty name);
        /**
         *  Each subclass must register each property name it uses with the pointer to the data,
         *  so that it can easily be accessed later.
         *  @param name     The observable property name.
         *  @param property A variant type wrapping the pointer to the data holding the actual value of the property.
         */
        virtual void registerProperty(HSSObservableProperty name, QVariant property);
        /**
         *  Getter for the current scope this object is operating on.
         *  @return A pointer to the vector of shared pointers to display objects
         */
        QSharedPointer<HSSSimpleSelection> getScope() const;
        /**
         *  Setter for the current scope this object is operating on.
         *  @param newScope     A pointer to a vector of shared pointers to display objects
         */
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
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
        virtual AXRController * getController() const;
        /**
         *  Setter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @param value        A shared pointer to the nearest display object.
         */
        void setThisObj(QSharedPointer<HSSDisplayObject> value);
        /**
         *  Getter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @return A shared pointer to the nearest display object.
         */
        QSharedPointer<HSSDisplayObject> getThisObj();


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

        virtual bool isA(HSSGradientType otherType);
        virtual HSSGradientType getGradientType();

        QSharedPointer<HSSObject> shared_from_this();

    protected:
        QMap<HSSObservableProperty, QVariant> properties;
        std::vector<AXRString> shorthandProperties;
        QMap<AXRString, bool> skipShorthand;
        size_t shorthandIndex;

        QSharedPointer<HSSParserNode> dIsA;

        QSharedPointer<HSSSimpleSelection> scope;
        QSharedPointer<HSSDisplayObject> thisObj;
        AXRController * axrController;

    private:
        HSSObjectType type;
        bool _isNamed;

        virtual QSharedPointer<HSSClonable> cloneImpl() const;

        QWeakPointer<HSSObject> ptr;
    };
}

Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSObject>)
Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSObject> >)
Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSObject> >*)
Q_DECLARE_METATYPE(AXR::HSSEventType)

// Necessary because the comma in the type name would cause it to
// be passed to the below macro as two parameters instead of one
typedef QMap<AXR::HSSEventType, std::vector<QSharedPointer<AXR::HSSObject> > > QMapHSSEventTypeVectorHSSObjectp;
Q_DECLARE_METATYPE(QMapHSSEventTypeVectorHSSObjectp)
Q_DECLARE_METATYPE(QMapHSSEventTypeVectorHSSObjectp*)
Q_DECLARE_METATYPE(AXR::HSSUnit*)
Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSObject>*)

#endif
