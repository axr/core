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

#ifndef HSSABSTRACTSTROKE_H
#define HSSABSTRACTSTROKE_H

#include "HSSObject.h"

class QPainter;
class QPainterPath;

namespace AXR
{
    /**
     *  @brief Abstract base class for all stroke objects.
     */
    class AXR_API HSSAbstractStroke : public HSSObject
    {
    public:
        typedef std::vector<QSharedPointer<HSSAbstractStroke> >::iterator it;

        HSSAbstractStroke(HSSStrokeType type, AXRController * controller);
        
        /**
         *  Destructor for HSSAbstractStroke objects.
         */
        virtual ~HSSAbstractStroke();
        
        bool isA(HSSStrokeType strokeType) const;
        HSSStrokeType getStrokeType() const;

        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        //size
        HSSUnit getSize() const;
        void setSize(HSSUnit value, HSSUnit specificity);
        //position
        HSSStrokePosition getPosition() const;
        //segments
        QSharedPointer<HSSObject> getSegments() const;

    protected:
        /**
         *  Creates a new HSSAbstractStroke object.
         */
        HSSAbstractStroke(AXRController * controller);

        /**
         *  Copy constructor for HSSAbstractStroke objects.
         */
        HSSAbstractStroke(const HSSAbstractStroke & orig);

    private:
        HSSStrokeType strokeType;
    };
}

#endif
