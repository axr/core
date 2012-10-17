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

#ifndef HSSPROPERTYDEFINITION_H
#define HSSPROPERTYDEFINITION_H

#include <QSharedPointer>
#include "HSSObject.h"
#include "HSSStatement.h"

namespace AXR
{
    /**
     *  @brief A property definition holds a value that is paired with the name of the property.
     *
     *  Inside the blocks of rules and object definitions property definitions are defined, to
     *  apply te values to the corresponding property of each object.
     */
    class AXR_API HSSPropertyDefinition : public HSSStatement
    {
    public:
        typedef QSharedPointer<HSSPropertyDefinition> p;

        /**
         *  Iterator for vectors of shared pointers to property definitions
         */
        typedef std::vector<p>::iterator it;

        /**
         *  Iterator for vectors of shared pointers to property definitions, const version.
         */
        typedef std::vector<p>::const_iterator const_it;

        /**
         *  Creates a new instance of a property definition, with an empty name and value.
         */
        HSSPropertyDefinition();

        /**
         *  Creates a new instance of a property definition, with the given name, but empty
         *  value.
         *  @param name     A string containing the name of the property.
         */
        HSSPropertyDefinition(AXRString name);

        /**
         *  Creates a new instance of a property definition, with the given name and value.
         *  @param name     A string containing the name of the property.
         *  @param value    A shared pointer to the parser node that holds the value of this
         *                  property definition.
         */
        HSSPropertyDefinition(AXRString name, HSSParserNode::p value);

        /**
         *  Copy constructor for HSSPropertyDefinition objects. Do not call directly, use clone() instead.
         */
        HSSPropertyDefinition(const HSSPropertyDefinition &orig);

        /**
         *  Clones an instance of HSSPropertyDefinition and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSPropertyDefinition
         */
        p clone() const;

        /**
         *  Destructor for this class.
         *
         *  @todo Make this virtual.
         */
        ~HSSPropertyDefinition();

        AXRString toString();

        /**
         *  Setter for the name of the property.
         *  @param name     A string containing the name of the property.
         */
        void setName(AXRString name);

        /**
         *  Getter for the name of the property.
         *  @return A string containing the name of the property.
         */
        AXRString getName();

        /**
         *  Setter for the value of the property.
         *  @param value    A shared pointer to the parser node that represents the value of the property.
         */
        void setValue(HSSParserNode::p value);

        /**
         *  Adds a value to the property definition. If the value is not already a multiple value definition,
         *  it will create one and add the current value to it too.
         *  @param value    A shared pointer to the parser node that represents the value to add to the property
         *                  definition.
         */
        void addValue(HSSParserNode::p value);

        /**
         *  Getter for the value of the property.
         *  @return A shared pointer to the parser node that represents the value of the property definition.
         */
        HSSParserNode::p getValue();

        virtual void setThisObj(QSharedPointer<HSSDisplayObject> value);

    protected:
        HSSPropertyDefinition::p shared_from_this();

        AXRString name;
        HSSParserNode::p value;

    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif
