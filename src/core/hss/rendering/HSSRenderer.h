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

namespace AXR
{
    class AXRDocument;
    class HSSLinearGradient;
    class HSSRadialGradient;

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

        QImage* getFinalFrame();

        void setDirtyRect(const HSSRect &dirtyRect);

        void setDocument(AXRDocument* document);

        bool isGlobalAntialiasingEnabled() const;
        void setGlobalAntialiasingEnabled(bool enable);

        void setOutputBoundsToObject(QSharedPointer<HSSDisplayObject> outputBoundsObject);

    private:
        void regeneratePainter(int width, int height);

        void performLayoutSteps(HSSDisplayObject &displayObject);

        void drawBackground(HSSContainer &container);

        void drawLinearGradient(HSSLinearGradient &gradient, const QPainterPath &path, HSSUnit posX, HSSUnit posY);

        void drawRadialGradient(HSSRadialGradient &gradient, const QPainterPath &path, HSSUnit posX, HSSUnit posY);

        void drawBorders(HSSContainer &container);

        void drawForeground(HSSTextBlock &textBlock);

        class Private;
        Private *d;
    };
}

#endif // HSSRENDERER_H
