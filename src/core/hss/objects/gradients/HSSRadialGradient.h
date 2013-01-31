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

#ifndef HSSRADIALGRADIENT_H
#define HSSRADIALGRADIENT_H

#include <QSharedPointer>
#include "HSSGradient.h"
#include "HSSRgb.h"

namespace AXR
{
    /**
     *  @brief The object type representing a radial gradient in HSS.
     *
     *  Radial gradients draw concentric circles that fade from one color to another
     */
    class AXR_API HSSRadialGradient : public HSSGradient
    {
        friend class HSSRenderer;

    public:
        typedef QSharedPointer<HSSRadialGradient> p;

        /**
         *  Creates a new instance of a radial gradient
         */
        HSSRadialGradient(AXRController * controller);
        /**
         *  Copy constructor for HSSRadialGradient objects
         */
        HSSRadialGradient(const HSSRadialGradient & orig);
        /**
         *  Clones an instance of HSSRadialGradient and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRadialGradient
         */
        p clone() const;

        /**
         *  Destructs the gradient after removing itself from its observers.
         */
        virtual ~HSSRadialGradient();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);

        /**
         *  Getter for the actual value object of centerX.
         *  @return A HSSUnit that contains the actual value of centerX.
         */
        HSSUnit getCenterX();

        /**
         *  Getter for the definition object of centerX.
         *  @return A shared pointer to the parser node containing the definition object of centerX.
         */
        QSharedPointer<HSSParserNode> getDCenterX();

        /**
         *  Setter for the definition object of centerX. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of centerX.
         */
        void setDCenterX(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect centerX.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void centerXChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of centerY.
         *  @return A HSSUnit that contains the actual value of centerY.
         */
        HSSUnit getCenterY();

        /**
         *  Getter for the definition object of centerY.
         *  @return A shared pointer to the parser node containing the definition object of centerY.
         */
        QSharedPointer<HSSParserNode> getDCenterY();

        /**
         *  Setter for the definition object of centerY. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of centerY.
         */
        void setDCenterY(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect centerY.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void centerYChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of offsetX.
         *  @return A HSSUnit that contains the actual value of offsetX.
         */
        HSSUnit getOffsetX();

        /**
         *  Getter for the definition object of offsetX.
         *  @return A shared pointer to the parser node containing the definition object of offsetX.
         */
        QSharedPointer<HSSParserNode> getDOffsetX();

        /**
         *  Setter for the definition object of offsetX. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of offsetX.
         */
        void setDOffsetX(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect offsetX.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void offsetXChanged(HSSObservableProperty source, void *data);

        /**
         *  Getter for the actual value object of offsetY.
         *  @return A HSSUnit that contains the actual value of offsetY.
         */
        HSSUnit getOffsetY();

        /**
         *  Getter for the definition object of offsetY.
         *  @return A shared pointer to the parser node containing the definition object of offsetY.
         */
        QSharedPointer<HSSParserNode> getDOffsetY();

        /**
         *  Setter for the definition object of offsetY. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of offsetY.
         */
        void setDOffsetY(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect offsetY.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void offsetYChanged(HSSObservableProperty source, void *data);

    protected:
        //centerX
        HSSUnit centerX;
        QSharedPointer<HSSParserNode> dCenterX;
        HSSObservable * observedCenterX;
        HSSObservableProperty observedCenterXProperty;

        //centerY
        HSSUnit centerY;
        QSharedPointer<HSSParserNode> dCenterY;
        HSSObservable * observedCenterY;
        HSSObservableProperty observedCenterYProperty;

        //offsetX
        HSSUnit offsetX;
        QSharedPointer<HSSParserNode> dOffsetX;
        HSSObservable * observedOffsetX;
        HSSObservableProperty observedOffsetXProperty;

        //offsetY
        HSSUnit offsetY;
        QSharedPointer<HSSParserNode> dOffsetY;
        HSSObservable * observedOffsetY;
        HSSObservableProperty observedOffsetYProperty;

        // TODO: roundness

        HSSUnit _evaluatePropertyValue(
                                       void(HSSRadialGradient::*callback)(HSSObservableProperty property, void* data),
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
