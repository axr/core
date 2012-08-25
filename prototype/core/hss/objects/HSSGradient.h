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

#ifndef HSSGRADIENT_H
#define HSSGRADIENT_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <cairo/cairo.h>
#include "HSSObject.h"
#include "HSSRgb.h"

namespace AXR {

    /**
     *  @brief Abstract base class for gradient objects. Do not use directly, but one of its
     *  subclasses instead.
     *
     *  Surfaces that can be filled with a color generally also accept a gradient. This class
     *  provides the base functionality that is common to all gradient objects.
     */
    class HSSGradient : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSGradient> p;
        typedef std::vector<HSSGradient::p>::iterator it;

        /**
         *  Constructor for HSSGradient objects
         */
        HSSGradient();
        /**
         *  Copy constructor for HSSGradient objects
         */
        HSSGradient(const HSSGradient & orig);

        /**
         *  Destructor for this class.
         */
        virtual ~HSSGradient();

        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);

        /**
         *  Call this method when you need to draw the gradient. Each specific
         *  subclass implements its own way of drawing the gradient.
         */
        virtual void draw(cairo_t * cairo) = 0;

        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);

        /**
         *  Getter for the actual value of startColor.
         *  @return A shared pointer to the HSSRgb object that holds the start color information.
         */
        HSSRgb::p getStartColor();

        /**
         *  Getter for the definition object of startColor.
         *  @return A shared pointer to the parser node containing the definition object of startColor.
         */
        HSSParserNode::p getDStartColor();

        /**
         *  Setter for the definition object of startColor. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of startColor.
         */
        void setDStartColor(HSSParserNode::p);

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
        HSSRgb::p getEndColor();

        /**
         *  Getter for the definition object of endColor.
         *  @return A shared pointer to the parser node containing the definition object of endColor.
         */
        HSSParserNode::p getDEndColor();

        /**
         *  Setter for the definition object of endColor. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of endColor.
         */
        void setDEndColor(HSSParserNode::p);

        /**
         *  Method to be passed as callback when observing changes that will affect endColor.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void endColorChanged(HSSObservableProperty source, void*data);


        /**
         *  Getter for the actual value of balance.
         *  @return A long double containing the calculated value of balance.
         */
        long double getBalance();

        /**
         *  Getter for the definition object of balance.
         *  @return A shared pointer to the parser node containing the definition object of balance.
         */
        HSSParserNode::p getDBalance();

        /**
         *  Setter for the definition object of balance. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of balance.
         */
        void setDBalance(HSSParserNode::p);

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
        HSSParserNode::p getDColorStops();

        /**
         *  Setter for the definition object of colorStops. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of colorStops.
         */
        void setDColorStops(HSSParserNode::p value);

        /**
         *  Since this property accepts multiple values, this allows to append a value instead of
         *  replacing the whole thing.
         *  @param value    A shared pointer to the parser node containing the definition object to be added
         *                  to colorStops.
         */
        void addDColorStops(HSSParserNode::p value);

        /**
         *  Method to be passed as callback when observing changes that will affect textAlign.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void colorStopsChanged(HSSObservableProperty source, void*data);

    protected:
        //startColor
        HSSRgb::p startColor;
        HSSParserNode::p dStartColor;
        HSSObservable * observedStartColor;
        HSSObservableProperty observedStartColorProperty;

        //endColor
        HSSRgb::p endColor;
        HSSParserNode::p dEndColor;
        HSSObservable * observedEndColor;
        HSSObservableProperty observedEndColorProperty;

        //balance
        long double balance;
        HSSParserNode::p dBalance;
        HSSObservable * observedBalance;
        HSSObservableProperty observedBalanceProperty;

        //colorStops
        HSSParserNode::p dColorStops;
        HSSObservable * observedColorStops;
        HSSObservableProperty observedColorStopsProperty;
        std::vector<HSSObject::p> colorStops;

        long double _setLDProperty(
                                   void(HSSGradient::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty
                                   );
    };
}


#endif
