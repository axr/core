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

#ifndef HSSRGB_H
#define HSSRGB_H

#include <QSharedPointer>
#include "HSSObject.h"

namespace AXR
{
    /**
     *  @brief The object type representing a \@rgb color in HSS.
     *
     *  Many objects accept color objects for their properties, e.g. HSSContainer's background property.
     *
     *  This object encapsulates
     */
    class AXR_API HSSRgb : public HSSObject
    {
    public:
        //virtual bool isKeyword(AXRString value, AXRString property);

        friend class HSSParser;

        /**
         *  Creates a new instance of a black color.
         *  @return A shared pointer to the new color object.
         */
        static QSharedPointer<HSSRgb> blackColor(AXRController * controller);

        /**
         *  Creates a new instance of a white color.
         *  @return A shared pointer to the new color object.
         */
        static QSharedPointer<HSSRgb> whiteColor(AXRController * controller);

        /**
         *  Constructor for HSSRgb objects
         */

        /**
         *  Copy constructor for HSSRgb objects
         */
        HSSRgb(const HSSRgb & orig);

        /**
         *  Clones an instance of HSSRgb and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRgb
         */
        QSharedPointer<HSSRgb> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSRgb();

        inline static QSharedPointer<HSSRgb> transparentColor(AXRController * controller) { return QSharedPointer<HSSRgb>(new HSSRgb(controller, 0, 0, 0, 0)); }
        inline static QSharedPointer<HSSRgb> defaultColor(AXRController * controller) { return QSharedPointer<HSSRgb>(new HSSRgb(controller)); }

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);

        QColor toQColor() const;
        QColor toQColorWithAlpha(int alpha) const;

    private:
        void _initialize();
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSRgb>)
Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSRgb>*)

#endif
