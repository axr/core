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

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);

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
        QSharedPointer<HSSRgb> getNextColorFromStops(std::vector<QSharedPointer<HSSObject> >::iterator it, std::vector<QSharedPointer<HSSObject> >::iterator endIt);

        /**
         *  Getter for the actual value of startColor.
         *  @return A shared pointer to the HSSRgb object that holds the start color information.
         */
        QSharedPointer<HSSRgb> getStartColor();

        /**
         *  Getter for the definition object of startColor.
         *  @return A shared pointer to the parser node containing the definition object of startColor.
         */
        QSharedPointer<HSSParserNode> getDStartColor();

        /**
         *  Setter for the definition object of startColor. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of startColor.
         */
        void setDStartColor(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect startColor.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void startColorChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the definition object of endColor.
         *  @return A shared pointer to the parser node containing the definition object of endColor.
         */
        QSharedPointer<HSSRgb> getEndColor();

        /**
         *  Getter for the definition object of endColor.
         *  @return A shared pointer to the parser node containing the definition object of endColor.
         */
        QSharedPointer<HSSParserNode> getDEndColor();

        /**
         *  Setter for the definition object of endColor. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of endColor.
         */
        void setDEndColor(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect endColor.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void endColorChanged(HSSObservableProperty source, void*data);


        /**
         *  Getter for the actual value of balance.
         *  @return A HSSUnit containing the calculated value of balance.
         */
        HSSUnit getBalance();

        /**
         *  Getter for the definition object of balance.
         *  @return A shared pointer to the parser node containing the definition object of balance.
         */
        QSharedPointer<HSSParserNode> getDBalance();

        /**
         *  Setter for the definition object of balance. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of balance.
         */
        void setDBalance(QSharedPointer<HSSParserNode>);

        /**
         *  Method to be passed as callback when observing changes that will affect balance.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void balanceChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the definition object of colorStops.
         *  @return A shared pointer to the parser node containing the definition object of colorStops.
         */
        QSharedPointer<HSSParserNode> getDColorStops();

        /**
         *  Setter for the definition object of colorStops. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of colorStops.
         */
        void setDColorStops(QSharedPointer<HSSParserNode> value);

        /**
         *  Since this property accepts multiple values, this allows to append a value instead of
         *  replacing the whole thing.
         *  @param value    A shared pointer to the parser node containing the definition object to be added
         *                  to colorStops.
         */
        void addDColorStops(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect textAlign.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void colorStopsChanged(HSSObservableProperty source, void*data);

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

        //startColor
        QSharedPointer<HSSRgb> startColor;
        QSharedPointer<HSSParserNode> dStartColor;
        HSSObservable * observedStartColor;
        HSSObservableProperty observedStartColorProperty;

        //endColor
        QSharedPointer<HSSRgb> endColor;
        QSharedPointer<HSSParserNode> dEndColor;
        HSSObservable * observedEndColor;
        HSSObservableProperty observedEndColorProperty;

        //balance
        HSSUnit balance;
        QSharedPointer<HSSParserNode> dBalance;
        HSSObservable * observedBalance;
        HSSObservableProperty observedBalanceProperty;

        //colorStops
        QSharedPointer<HSSParserNode> dColorStops;
        HSSObservable * observedColorStops;
        HSSObservableProperty observedColorStopsProperty;
        std::vector<QSharedPointer<HSSObject> > colorStops;

        HSSUnit _evaluatePropertyValue(
                                   void(HSSGradient::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty
                                   );

    private:
        HSSGradientType gradientType;
    };
}

#endif
