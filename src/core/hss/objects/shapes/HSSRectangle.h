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

#ifndef HSSRECTANGLE_H
#define HSSRECTANGLE_H

#include "HSSShape.h"

namespace AXR
{
    /**
     *  @brief Shape object, representing the HSS object \@rect.
     */
    class AXR_API HSSRectangle : public HSSShape
    {
    public:
        /**
         *  Constructor for HSSRectangle objects
         */
        HSSRectangle(AXRController * controller);

        /**
         *  Copy constructor for HSSRectangle objects
         */
        HSSRectangle(const HSSRectangle & orig);

        /**

         *  Clones an instance of HSSRectangle and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRectangle
         */
        QSharedPointer<HSSRectangle> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSRectangle();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);

        virtual void createPath(QSharedPointer<HSSPath> &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::list<QSharedPointer<HSSParserNode> > segments);

        virtual void drawStrokes(std::list<QSharedPointer<HSSAbstractStroke> > strokes, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY);

    private:
        inline void _increaseThickness(const QSharedPointer<HSSObject> & parserNode, const HSSUnit size, HSSUnit & top, HSSUnit & right, HSSUnit & bottom, HSSUnit & left) const;
        inline void __increaseThickness(const QSharedPointer<HSSParserNode> & segmentsObj, const HSSUnit size, HSSUnit & top, HSSUnit & right, HSSUnit & bottom, HSSUnit & left) const;
        inline void _hasAll(const QSharedPointer<HSSObject> & segmentsObj, bool & hasAll, bool & hasSegments) const;

        inline void _drawCenteredStrokeBySegments(
            QSharedPointer<HSSPath> & path,
            const QSharedPointer<HSSObject> & segmentsObj,
            HSSUnit & theSize,
            HSSUnit & width,
            HSSUnit & height,
            HSSUnit & offsetX,
            HSSUnit & offsetY,
            HSSUnit & topThickness,
            HSSUnit & topCumulative,
            HSSUnit & topCorrection,
            HSSUnit & rightThickness,
            HSSUnit & rightCumulative,
            HSSUnit & rightCorrection,
            HSSUnit & bottomThickness,
            HSSUnit & bottomCumulative,
            HSSUnit & bottomCorrection,
            HSSUnit & leftThickness,
            HSSUnit & leftCumulative,
            HSSUnit & leftCorrection
       ) const;

        inline void __drawCenteredStrokeBySegments(
            QSharedPointer<HSSPath> & path,
            const AXRString & segment,
            HSSUnit & theSize,
            HSSUnit & width,
            HSSUnit & height,
            HSSUnit & offsetX,
            HSSUnit & offsetY,
            HSSUnit & topThickness,
            HSSUnit & topCumulative,
            HSSUnit & topCorrection,
            HSSUnit & rightThickness,
            HSSUnit & rightCumulative,
            HSSUnit & rightCorrection,
            HSSUnit & bottomThickness,
            HSSUnit & bottomCumulative,
            HSSUnit & bottomCorrection,
            HSSUnit & leftThickness,
            HSSUnit & leftCumulative,
            HSSUnit & leftCorrection
        ) const;
        inline void _drawInsideStrokeBySegments(
            QSharedPointer<HSSPath> & path,
            const QSharedPointer<HSSObject> & segmentsObj,
            HSSUnit & theSize,
            HSSUnit & width,
            HSSUnit & height,
            HSSUnit & offsetX,
            HSSUnit & offsetY,
            HSSUnit & topThickness,
            HSSUnit & topCumulative,
            HSSUnit & topCorrection,
            HSSUnit & rightThickness,
            HSSUnit & rightCumulative,
            HSSUnit & rightCorrection,
            HSSUnit & bottomThickness,
            HSSUnit & bottomCumulative,
            HSSUnit & bottomCorrection,
            HSSUnit & leftThickness,
            HSSUnit & leftCumulative,
            HSSUnit & leftCorrection
        ) const;

        inline void __drawInsideStrokeBySegments(
            QSharedPointer<HSSPath> & path,
            const AXRString & segment,
            HSSUnit & theSize,
            HSSUnit & width,
            HSSUnit & height,
            HSSUnit & offsetX,
            HSSUnit & offsetY,
            HSSUnit & topThickness,
            HSSUnit & topCumulative,
            HSSUnit & topCorrection,
            HSSUnit & rightThickness,
            HSSUnit & rightCumulative,
            HSSUnit & rightCorrection,
            HSSUnit & bottomThickness,
            HSSUnit & bottomCumulative,
            HSSUnit & bottomCorrection,
            HSSUnit & leftThickness,
            HSSUnit & leftCumulative,
            HSSUnit & leftCorrection
        ) const;

        inline void _drawOutsideStrokeBySegments(
            QSharedPointer<HSSPath> & path,
            const QSharedPointer<HSSObject> & segmentsObj,
            HSSUnit & theSize,
            HSSUnit & width,
            HSSUnit & height,
            HSSUnit & offsetX,
            HSSUnit & offsetY,
            HSSUnit & topThickness,
            HSSUnit & topCumulative,
            HSSUnit & topCorrection,
            HSSUnit & rightThickness,
            HSSUnit & rightCumulative,
            HSSUnit & rightCorrection,
            HSSUnit & bottomThickness,
            HSSUnit & bottomCumulative,
            HSSUnit & bottomCorrection,
            HSSUnit & leftThickness,
            HSSUnit & leftCumulative,
            HSSUnit & leftCorrection
        ) const;

        inline void __drawOutsideStrokeBySegments(
            QSharedPointer<HSSPath> & path,
            const AXRString & segment,
            HSSUnit & theSize,
            HSSUnit & width,
            HSSUnit & height,
            HSSUnit & offsetX,
            HSSUnit & offsetY,
            HSSUnit & topThickness,
            HSSUnit & topCumulative,
            HSSUnit & topCorrection,
            HSSUnit & rightThickness,
            HSSUnit & rightCumulative,
            HSSUnit & rightCorrection,
            HSSUnit & bottomThickness,
            HSSUnit & bottomCumulative,
            HSSUnit & bottomCorrection,
            HSSUnit & leftThickness,
            HSSUnit & leftCumulative,
            HSSUnit & leftCorrection
        ) const;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif
