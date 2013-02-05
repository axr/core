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

#include <QSharedPointer>
#include "HSSGradient.h"

namespace AXR
{
    /**
     *  @brief The object type representing a linear gradient in HSS.
     *
     *  Linear gradients fade from one color to another in one direction, where
     *  each perpendicular line to the one that defines the direction are the same.
     */
    class AXR_API HSSLinearGradient : public HSSGradient
    {
        friend class HSSRenderer;

    public:
        /**
         *  Creates a new instance of a
         */
        HSSLinearGradient(AXRController * controller);
        /**
         *  Copy constructor for HSSLinearGradient objects
         */
        HSSLinearGradient(const HSSLinearGradient & orig);
        /**
         *  Clones an instance of HSSLinearGradient and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLinearGradient
         */
        QSharedPointer<HSSLinearGradient> clone() const;

        /**
         *  Destructs the gradient after removing itself from its observers.
         */
        virtual ~HSSLinearGradient();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);

        /**
         *  Getter for the actual value object of startX.
         *  @return A HSSUnit that contains the actual value of startX.
         */
        HSSUnit getStartX();

        /**
         *  Getter for the definition object of startX.
         *  @return A shared pointer to the parser node containing the definition object of startX.
         */
        QSharedPointer<HSSParserNode> getDStartX();

        /**
         *  Setter for the definition object of startX. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of startX.
         */
        void setDStartX(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect startX.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void startXChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of startY.
         *  @return A HSSUnit that contains the actual value of startY.
         */
        HSSUnit getStartY();

        /**
         *  Getter for the definition object of startY.
         *  @return A shared pointer to the parser node containing the definition object of startY.
         */
        QSharedPointer<HSSParserNode> getDStartY();

        /**
         *  Setter for the definition object of startY. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of startY.
         */
        void setDStartY(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect startY.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void startYChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of endX.
         *  @return A HSSUnit that contains the actual value of endX.
         */
        HSSUnit getEndX();

        /**
         *  Getter for the definition object of endX.
         *  @return A shared pointer to the parser node containing the definition object of endX.
         */
        QSharedPointer<HSSParserNode> getDEndX();

        /**
         *  Setter for the definition object of endX. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of endX.
         */
        void setDEndX(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect endX.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void endXChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of endY.
         *  @return A HSSUnit that contains the actual value of endY.
         */
        HSSUnit getEndY();

        /**
         *  Getter for the definition object of endY.
         *  @return A shared pointer to the parser node containing the definition object of endY.
         */
        QSharedPointer<HSSParserNode> getDEndY();

        /**
         *  Setter for the definition object of endY. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of endY.
         */
        void setDEndY(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect endY.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void endYChanged(HSSObservableProperty source, void *data);

    protected:
        //startX
        HSSUnit startX;
        QSharedPointer<HSSParserNode> dStartX;
        HSSObservable * observedStartX;
        HSSObservableProperty observedStartXProperty;

        //startY
        HSSUnit startY;
        QSharedPointer<HSSParserNode> dStartY;
        HSSObservable * observedStartY;
        HSSObservableProperty observedStartYProperty;

        //endX
        HSSUnit endX;
        QSharedPointer<HSSParserNode> dEndX;
        HSSObservable * observedEndX;
        HSSObservableProperty observedEndXProperty;

        //endY
        HSSUnit endY;
        QSharedPointer<HSSParserNode> dEndY;
        HSSObservable * observedEndY;
        HSSObservableProperty observedEndYProperty;

        HSSUnit _evaluatePropertyValue(
                                   void(HSSLinearGradient::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedProperty,
                                   HSSObservable * observedObject,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty
                                   );
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;

    };
}

#endif
