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

#ifndef HSSRENDERER_H
#define HSSRENDERER_H
#include <QImage>
#include "IHSSVisitor.h"
#include "HSSDisplayObject.h"
#include "HSSUnits.h"

template <class T> class QSharedPointer;

namespace AXR
{
    class AXRDocument;

    class AXR_API HSSRenderer : public IHSSVisitor
    {
    public:
        HSSRenderer();
        virtual void initializeVisit();
        virtual void visit(HSSContainer &container);
        virtual void visit(HSSTextBlock &textBlock);
        virtual void finalizeVisit();
        virtual void reset();

        QImage* getFinalFrame();

        void setDirtyRect(const HSSRect &dirtyRect)
        {
            _dirtyRect = dirtyRect;
        }

        void setDocument(AXRDocument* document);

        void setGlobalAntialiasingEnabled(bool enable);
        bool getGlobalAntialiasingEnabled();

        void setOutputBoundsToObject(QSharedPointer<HSSDisplayObject> outputBoundsObject);

    protected:
        QImage* _canvas = NULL;
        QPainter* _canvasPainter = NULL;
        bool _globalAntialiasingEnabled;
        bool _repaintAll;
        AXRDocument* _document;
        QImage *rootSurface = NULL;
        QImage rootSurfaceFinal;
        HSSRect _dirtyRect;
        HSSRect _bounds;
        bool _hasOutputBoundsRect = false;
        QRect* _outputBoundsRect = NULL;
        QSharedPointer<HSSDisplayObject> _outputBoundsObject;
        bool _hasOutputBoundsObject = false;
        bool globalAntialiasingEnabled() const;
        void regeneratePainter(int width, int height);

        void performLayoutSteps(HSSDisplayObject &displayObject);

        void drawBackground(HSSContainer &container);

        void drawBorders(HSSContainer &container);

        void drawForeground(HSSContainer &container);
        void drawForeground(HSSTextBlock &textBlock);

    };
}
#endif /* defined(HSSRENDERER_H) */