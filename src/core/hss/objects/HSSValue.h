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

#ifndef HSSVALUE_H
#define HSSVALUE_H

#include "HSSMultipleValueDefinition.h"
#include "HSSObject.h"

namespace AXR
{
    /**
     *  @brief Represents value objects in HSS (\@value).
     */
    class AXR_API HSSValue : public HSSObject
    {
    public:
        typedef QSharedPointer<HSSValue> p;

        /**
         *  Constructor for HSSValue objects.
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
        p clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSValue();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);

        /**
         *  Getter for the definition object of value.
         *  @return A shared pointer to the parser node containing the definition object of value.
         */
        const HSSParserNode::p getDValue() const;

        /**
         *  Setter for the definition object of value. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of value.
         */
        void setDValue(HSSParserNode::p value);

        /**
         *  Since this property accepts multiple values, this allows to append a value instead of replacing
         *  the whole thing.
         *  @param value    A shared pointer to the parser node containing the definition object to be
         *                  added to value.
         */
        void addDValue(HSSParserNode::p value);

        /**
         *  Method to be passed as callback when observing changes that will affect value.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(HSSObservableProperty source, void*data);

    protected:
        //value
        HSSParserNode::p dValue;
        HSSObservable * observedValue;
        HSSObservableProperty observedValueProperty;

    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif
