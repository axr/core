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

#ifndef HSSLINEARGRADIENT_H
#define HSSLINEARGRADIENT_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "HSSGradient.h"
#include "HSSRgb.h"

namespace AXR {

    /**
     *  @brief The object type representing a linear gradient in HSS.
     *
     *  Linear gradients fade from one color to another in one direction, where
     *  each perpendicular line to the one that defines the direction are the same.
     */
    class HSSLinearGradient : public HSSGradient
    {
    public:
        typedef boost::shared_ptr<HSSLinearGradient> p;

        /**
         *  Creates a new instance of a
         */
        HSSLinearGradient();
        /**
         *  Copy constructor for HSSLinearGradient objects
         */
        HSSLinearGradient(const HSSLinearGradient & orig);
        /**
         *  Clones an instance of HSSLinearGradient and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLinearGradient
         */
        p clone() const;

        /**
         *  Destructs the gradient after removing itself from its observers.
         */
        virtual ~HSSLinearGradient();

        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);

        /**
         *  Getter for the actual value object of startX.
         *  @return A long double that contains the actual value of startX.
         */
        long double getStartX();

        /**
         *  Getter for the definition object of startX.
         *  @return A shared pointer to the parser node containing the definition object of startX.
         */
        HSSParserNode::p getDStartX();

        /**
         *  Setter for the definition object of startX. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of startX.
         */
        void setDStartX(HSSParserNode::p value);

        /**
         *  Method to be passed as callback when observing changes that will affect startX.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void startXChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of startY.
         *  @return A long double that contains the actual value of startY.
         */
        long double getStartY();

        /**
         *  Getter for the definition object of startY.
         *  @return A shared pointer to the parser node containing the definition object of startY.
         */
        HSSParserNode::p getDStartY();

        /**
         *  Setter for the definition object of startY. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of startY.
         */
        void setDStartY(HSSParserNode::p value);

        /**
         *  Method to be passed as callback when observing changes that will affect startY.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void startYChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of endX.
         *  @return A long double that contains the actual value of endX.
         */
        long double getEndX();

        /**
         *  Getter for the definition object of endX.
         *  @return A shared pointer to the parser node containing the definition object of endX.
         */
        HSSParserNode::p getDEndX();

        /**
         *  Setter for the definition object of endX. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of endX.
         */
        void setDEndX(HSSParserNode::p value);

        /**
         *  Method to be passed as callback when observing changes that will affect endX.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void endXChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of endY.
         *  @return A long double that contains the actual value of endY.
         */
        long double getEndY();

        /**
         *  Getter for the definition object of endY.
         *  @return A shared pointer to the parser node containing the definition object of endY.
         */
        HSSParserNode::p getDEndY();

        /**
         *  Setter for the definition object of endY. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of endY.
         */
        void setDEndY(HSSParserNode::p value);

        /**
         *  Method to be passed as callback when observing changes that will affect endY.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void endYChanged(HSSObservableProperty source, void *data);

        virtual void draw(cairo_t * cairo);

    protected:
        //startX
        long double startX;
        HSSParserNode::p dStartX;
        HSSObservable * observedStartX;
        HSSObservableProperty observedStartXProperty;

        //startY
        long double startY;
        HSSParserNode::p dStartY;
        HSSObservable * observedStartY;
        HSSObservableProperty observedStartYProperty;

        //endX
        long double endX;
        HSSParserNode::p dEndX;
        HSSObservable * observedEndX;
        HSSObservableProperty observedEndXProperty;

        //endY
        long double endY;
        HSSParserNode::p dEndY;
        HSSObservable * observedEndY;
        HSSObservableProperty observedEndYProperty;

        long double _setLDProperty(
                                   void(HSSLinearGradient::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p       value,
                                   long double            percentageBase,
                                   HSSObservableProperty  observedProperty,
                                   HSSObservable *        observedObject,
                                   HSSObservableProperty  observedSourceProperty,
                                   HSSObservable *        &observedStore,
                                   HSSObservableProperty  &observedStoreProperty
                                   );
    private:
        virtual HSSClonable::p cloneImpl() const;

    };
}


#endif
