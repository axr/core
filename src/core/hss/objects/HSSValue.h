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

#ifndef HSSVALUE_H
#define HSSVALUE_H

#include "HSSObject.h"

namespace AXR
{
    /**
     *  @brief Represents value objects in HSS (\@value).
     */
    class AXR_API HSSValue : public HSSObject
    {
    public:
        /**
         *  Convenience constructor, to set a value directly
         *  @param controller   A pointer to the controller
         *  @param parserNode   The value to be used
         */
        static QSharedPointer<HSSValue> valueFromParserNode(AXRController * controller, QSharedPointer<HSSParserNode> parserNode, QSharedPointer<HSSDisplayObject> thisObj, QSharedPointer<HSSSimpleSelection> scope);
        /**
         *  Constructor for HSSValue objects.
         *  @param controller   A pointer to the controller
         */
        HSSValue(AXRController * controller);
        /**
         *  Copy constructor for HSSValue objects. Do not use directly, use clone() instead.
         */
        HSSValue(const HSSValue & orig);
        /**
         *  Clones an instance of HSSValue and gives a shared pointer of the newly instanciated
         *  object.
         *  @return A shared pointer to the new HSSValue.
         */
        QSharedPointer<HSSValue> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSValue();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);

        /**
         *  Getter for the definition object of value.
         *  @return A shared pointer to the parser node containing the definition object of value.
         */
        const QSharedPointer<HSSParserNode> getValue() const;

        void setValue(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect value.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj);

        const HSSUnit getNumber() const;
        const bool getBool() const;
        const AXRString getString() const;
        const QSharedPointer<HSSObject> getObject() const;

        //see HSSObject.h for documentation on this method
        void setScope(QSharedPointer<HSSSimpleSelection> newScope);

        //see HSSObject.h for documentation on this method
        void setThisObj(QSharedPointer<HSSDisplayObject> value);

    protected:
        //value
        QSharedPointer<HSSParserNode> value;

    private:
        const HSSUnit _getNumber(const QSharedPointer<HSSParserNode> & parserNode) const;
        const bool _getBool(const QSharedPointer<HSSParserNode> & parserNode) const;
        const AXRString _getString(const QSharedPointer<HSSParserNode> & parserNode) const;
        void _initialize();
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif
