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

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;

    };
}

#endif
