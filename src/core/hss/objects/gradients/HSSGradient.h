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

#ifndef HSSGRADIENT_H
#define HSSGRADIENT_H

#include <QSharedPointer>
#include "HSSObject.h"

namespace AXR
{
    class HSSRgb;

    /**
     *  @brief Abstract base class for gradient objects.
     *
     *  Surfaces that can be filled with a color generally also accept a gradient. This class
     *  provides the base functionality that is common to all gradient objects.
     */
    class AXR_API HSSGradient : public HSSObject
    {
    public:
        /**
         *  When logging, you often need a string representation of the gradient type.
         *  @param gradientType   The gradient type to represent as a string.
         *  @return A string representation of the given type.
         */
        static AXRString gradientTypeStringRepresentation(HSSGradientType gradientType);

        typedef std::vector<QSharedPointer<HSSGradient> >::iterator it;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSGradient();

        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        /**
         *  If the start color is transparent, we need to be able to find the next color that is not
         *  transparent, wether it is inside the color stops, or the end color.
         *  @return A shared pointer to the next suitable color from the color stops, the end color, or
         *          transparent black.
         */
        QSharedPointer<HSSRgb> getColorAfterFirst();

        /**
         *  If the end color is transparent, we need to be able to find the previous color that is not
         *  transparent, wether it is inside the color stops, or the end color.
         *  @return A shared pointer to the previous suitable color from the color stops, the start color, or
         *          transparent black.
         */
        QSharedPointer<HSSRgb> getColorBeforeLast();

        /**
         *  When a transparent color stop is found, this method allows to continue iterating through the
         *  color stops to find the next color that is not transparent.
         *  @param it       The current iterator.
         *  @param endIt    The iterator that points to the end of the color stops.
         *  @return A shared pointer to the next suitable color from the color stops, starting at the given
         *          iterator, the end color, or transparent black.
         */
        QSharedPointer<HSSRgb> getNextColorFromStops(QListIterator<QSharedPointer<HSSObject> > it);

        //startColor
        const QSharedPointer<HSSObject> getStartColor() const;
        QSharedPointer<HSSObject> computeStartColor(QSharedPointer<HSSParserNode> parserNode);
        //endColor
        const QSharedPointer<HSSObject> getEndColor() const;
        QSharedPointer<HSSObject> computeEndColor(QSharedPointer<HSSParserNode> parserNode);
        //balance
        const QSharedPointer<HSSObject> getBalance() const;
        //colorStops
        const QSharedPointer<HSSObject> getColorStops() const;
        QSharedPointer<HSSObject> computeColorStops(QSharedPointer<HSSParserNode> parserNode);



        /**
         *  Allows you to check if this gradient is of the given type.
         *  @param  type    The gradient type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSGradientType type);

        /**
         *  @return The gradient type of this instance.
         */
        HSSGradientType getGradientType();

    protected:
        /**
         *  Constructor for HSSGradient objects
         */
        HSSGradient(HSSGradientType type, AXRController * controller);

        /**
         *  Copy constructor for HSSGradient objects
         */
        HSSGradient(const HSSGradient & orig);

    private:
        void _initialize();
        HSSGradientType gradientType;
    };
}

#endif
