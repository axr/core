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

#ifndef HSSBORDER_H
#define HSSBORDER_H

#include <QSharedPointer>
#include "HSSObject.h"

class QPainter;
class QPainterPath;

namespace AXR
{
    /**
     *  @brief Abstract base class for all border objects.
     */
    class AXR_API HSSBorder : public HSSObject
    {
    public:
        typedef std::vector<QSharedPointer<HSSBorder> >::iterator it;

        /**
         *  Destructor for HSSBorder objects.
         */
        virtual ~HSSBorder();

        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        /**
         *  Each type of border implements its own drawing routines. Call this method
         *  when you need to draw the border on a surface.
         *  @param painter A QPainter context used to draw onto a surface.
         *  @param path The path comprising the object shape.
         */
        virtual void draw(QPainter &painter, const QPainterPath &path) = 0;

        //size
        HSSUnit getSize() const;
        void setSize(HSSUnit value, HSSUnit specificity);
        //position
        HSSBorderPosition getPosition() const;
        //segments
        QSharedPointer<HSSObject> getSegments() const;

    protected:
        /**
         *  Creates a new HSSBorder object.
         */
        HSSBorder(AXRController * controller);

        /**
         *  Copy constructor for HSSBorder objects.
         */
        HSSBorder(const HSSBorder & orig);
    };
}

Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSBorder> >)
Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSBorder> >*)
Q_DECLARE_METATYPE(AXR::HSSBorderPosition*)

#endif
