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

#ifndef HSSRENDERER_H
#define HSSRENDERER_H

#include "HSSAbstractVisitor.h"

template <class T> class QSharedPointer;

#ifdef __APPLE__
typedef struct CGImage *CGImageRef;
#import <CoreGraphics/CoreGraphics.h>
#endif

namespace AXR
{
    class AXRDocument;
    class HSSColorStop;
    class HSSLinearGradient;
    class HSSPath;
    class HSSRadialGradient;
    class HSSRenderGradient;
    class HSSRect;
    class HSSRgb;

    class AXR_API HSSRenderer : public HSSAbstractVisitor
    {
    public:
        HSSRenderer();
        virtual ~HSSRenderer();

        virtual void initializeVisit();
        virtual void visit(HSSContainer &container);
        virtual void visit(HSSTextBlock &textBlock);
        virtual void finalizeVisit();
        virtual void reset();

        void setDirtyRect(const HSSRect &dirtyRect);

        void setDocument(AXRDocument* document);

    private:
        void regeneratePainter(int width, int height);

        void performLayoutSteps(HSSDisplayObject &displayObject);

        void drawBackground(HSSContainer &container);

        void drawLinearGradient(const QSharedPointer<HSSLinearGradient> &gradient, const QSharedPointer<HSSPath> &path, HSSUnit posX, HSSUnit posY);

        void drawRadialGradient(const QSharedPointer<HSSRadialGradient> &gradient, const QSharedPointer<HSSPath> &path, HSSUnit posX, HSSUnit posY);

        void drawStrokes(HSSContainer &container);
        void drawStrokes(QSharedPointer<HSSShape> shape, std::list<QSharedPointer<HSSAbstractStroke> > strokes, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY);

        void drawForeground(HSSTextBlock &textBlock);

        void _addColorStops(const QSharedPointer<HSSRenderGradient> & pat, const QSharedPointer<HSSLinearGradient> & gradient, QSharedPointer<HSSRgb> & prevColor, std::vector<HSSUnit> & positions, QSharedPointer<HSSObject> theStopObj, const QSharedPointer<HSSObject> &colorStops);
        static HSSUnit _nextFreePosition(std::vector<HSSUnit> &positions, HSSUnit position);
        void _drawBackground(QSharedPointer<HSSPath> & path, QSharedPointer<HSSObject> theobj, HSSUnit globalX, HSSUnit globalY);

        class Private;
        Private *d;
    };
}

#endif // HSSRENDERER_H
