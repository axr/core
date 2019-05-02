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

#ifndef HSSRADIALGRADIENT_H
#define HSSRADIALGRADIENT_H

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

        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);

        /**
         *  Getter for the actual value object of centerX.
         *  @return A HSSUnit that contains the actual value of centerX.
         */
        HSSUnit getCenterX() const;

        /**
         *  Getter for the actual value object of centerY.
         *  @return A HSSUnit that contains the actual value of centerY.
         */
        HSSUnit getCenterY() const;

        /**
         *  Getter for the computed value object of offsetX.
         *  @return A HSSUnit that contains the actual value of offsetX.
         */
        HSSUnit getOffsetX() const;

        /**
         *  Getter for the computed value object of offsetY.
         *  @return A HSSUnit that contains the actual value of offsetY.
         */
        HSSUnit getOffsetY() const;

    private:
        void _initialize();
        virtual QSharedPointer<HSSClonable> cloneImpl() const;

    };
}

#endif
