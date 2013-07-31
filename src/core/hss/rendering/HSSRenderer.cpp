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

#include <cmath>
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRError.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSBorder.h"
#include "HSSContainer.h"
#include "HSSFont.h"
#include "HSSKeywordConstant.h"
#include "HSSLinearGradient.h"
#include "HSSMultipleValue.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRadialGradient.h"
#include "HSSRect.h"
#include "HSSRenderer.h"
#include "HSSRoundedRect.h"
#include "HSSShape.h"
#include "HSSTextBlock.h"
#include "HSSColorStop.h"
#include "HSSParserNode.h"
#include "HSSValue.h"
#include <QLinearGradient>
#include <QPainter>
#include <QSharedPointer>

using namespace AXR;

class HSSRenderer::Private
{
public:
    Private() : canvas(), canvasPainter(), globalAntialiasingEnabled(true),
    repaintAll(true), document(), rootSurface(), rootSurfaceFinal(),
    dirtyRect(), bounds(), hasOutputBoundsRect(), outputBoundsRect(),
    outputBoundsObject(), hasOutputBoundsObject()
    {
    }

    QImage canvas;
    QPainter *canvasPainter;
    bool globalAntialiasingEnabled;
    bool repaintAll;
    AXRDocument *document;
    QImage *rootSurface;
    QImage rootSurfaceFinal;
    HSSRect dirtyRect;
    HSSRect bounds;
    bool hasOutputBoundsRect;
    QRect *outputBoundsRect;
    QSharedPointer<HSSDisplayObject> outputBoundsObject;
    bool hasOutputBoundsObject;
};

HSSRenderer::HSSRenderer()
: d(new Private)
{
    this->setFilterFlags(HSSVisitorFilterRendering);
}

HSSRenderer::~HSSRenderer()
{
    delete d;
}

void HSSRenderer::setDirtyRect(const HSSRect &dirtyRect)
{
    d->dirtyRect = dirtyRect;
}

void HSSRenderer::setDocument(AXRDocument* document)
{
    d->document = document;
}

void HSSRenderer::visit(HSSContainer &container)
{
    if (container.getVisible())
    {
        performLayoutSteps(container);

        if (container._isDirty || d->repaintAll)
        {
            axr_log(LoggerChannelRendering, AXRString("HSSRenderer: redrawing contents of %1 with width: %2, height: %3, x: %4 and y: %5").arg(container.elementName).arg(container.getWidth()).arg(container.getHeight()).arg(container.globalX).arg(container.globalY));

            container._isDirty = false;
            drawBackground(container);
            drawBorders(container);
        }
    }
    else
    {
        axr_log(LoggerChannelRendering, "HSSRenderer: skipping (not visible) " + container.elementName);
    }
}

void HSSRenderer::visit(HSSTextBlock &textBlock)
{
    if (textBlock.getVisible())
    {
        axr_log(LoggerChannelRendering, "HSSRenderer: drawing " + textBlock.getText());
        performLayoutSteps(textBlock);

        if (textBlock._isDirty || d->repaintAll)
        {
            axr_log(LoggerChannelRendering, AXRString("HSSRenderer: redrawing contents of %1 with width: %2, height: %3, x: %4 and y: %5").arg(textBlock.elementName).arg(textBlock.getWidth()).arg(textBlock.getHeight()).arg(textBlock.x).arg(textBlock.y));
            textBlock._isDirty = false;
            drawForeground(textBlock);
        }
    }
    else
    {
        axr_log(LoggerChannelRendering, "HSSRenderer: skipping (not visible) " + textBlock.elementName);
    }
}

void HSSRenderer::drawBorders(HSSContainer &container)
{
    if (d->globalAntialiasingEnabled)
        d->canvasPainter->setRenderHint(QPainter::Antialiasing);

    QSharedPointer<HSSObject> borderObj = container.getBorder();
    QList<QSharedPointer<HSSBorder> > borders;
    if (borderObj)
    {
        if (borderObj->isA(HSSObjectTypeBorder))
        {
            borders.append(qSharedPointerCast<HSSBorder>(borderObj));
        }
        else if (borderObj->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(const QSharedPointer<HSSObject> bObj, qSharedPointerCast<HSSMultipleValue>(borderObj)->getValues())
            {
                if (bObj->isA(HSSObjectTypeBorder))
                {
                    borders.append(qSharedPointerCast<HSSBorder>(bObj));
                }
            }
        }
    }
    if (borders.size() > 0)
    {
        container.getShape()->drawBorders(*d->canvasPainter, borders, container.getWidth(), container.getHeight(), container.globalX, container.globalY);
    }
}

void HSSRenderer::drawBackground(HSSContainer &container)
{
    if (d->globalAntialiasingEnabled)
        d->canvasPainter->setRenderHint(QPainter::Antialiasing);

    QSharedPointer<HSSObject> background = container.getComputedValue("background");
    if (background)
    {
        QPainterPath path;
        container.getShape()->createPath(path, container.globalX, container.globalY, container.getWidth(), container.getHeight());
        if (background->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(QSharedPointer<HSSObject> theobj, qSharedPointerCast<HSSMultipleValue>(background)->getValues())
            {
                this->_drawBackground(path, theobj, container.globalX, container.globalY);
            }
        }
        else
        {
            this->_drawBackground(path, background, container.globalX, container.globalY);
        }

    }
}

void HSSRenderer::_drawBackground(QPainterPath & path, QSharedPointer<HSSObject> theobj, HSSUnit globalX, HSSUnit globalY)
{
    switch (theobj->getObjectType())
    {
        case HSSObjectTypeRgb:
        {
            QSharedPointer<HSSRgb> color = qSharedPointerCast<HSSRgb > (theobj);
            if (!color)
            {
                d->canvasPainter->fillPath(path, QColor(0, 0, 0, 0));
            }
            else
            {
                d->canvasPainter->fillPath(path, color->toQColor());
            }
            break;
        }

        case HSSObjectTypeGradient:
        {
            if (theobj->isA(HSSGradientTypeLinear))
            {
                QSharedPointer<HSSLinearGradient> grad = qSharedPointerCast<HSSLinearGradient > (theobj);
                drawLinearGradient(*grad, path, globalX, globalY);
            }
            else if (theobj->isA(HSSGradientTypeRadial))
            {
                QSharedPointer<HSSRadialGradient> grad = qSharedPointerCast<HSSRadialGradient > (theobj);
                drawRadialGradient(*grad, path, globalX, globalY);
            }
            else
            {
                AXRError("HSSRenderer", "Unknown gradient type.").raise();
            }

            break;
        }

        default:
            break;
    }
}

void HSSRenderer::drawForeground(HSSTextBlock& textBlock)
{
    if (d->globalAntialiasingEnabled)
        d->canvasPainter->setRenderHint(QPainter::TextAntialiasing);

    QSharedPointer<HSSFont> theFont;
    QSharedPointer<HSSObject> fontObj = textBlock.getFont();
    if (fontObj && fontObj->isA(HSSObjectTypeFont))
    {
        theFont = qSharedPointerCast<HSSFont>(fontObj);
    }

    QPen pen;
    if (theFont && theFont->getColor())
    {
        QSharedPointer<HSSRgb> textColor = qSharedPointerCast<HSSRgb>(theFont->getColor());
        pen.setColor(QColor(textColor->getRed(), textColor->getGreen(), textColor->getBlue(), textColor->getAlpha()));
    }
    else
    {
        pen.setColor(QColor(0, 0, 0));
    }

    d->canvasPainter->setPen(pen);

    d->canvasPainter->setFont(textBlock.getQFont());
    Qt::Alignment flags = Qt::AlignLeft;
    switch (textBlock.getTextAlign())
    {
        case HSSTextAlignTypeLeft:
            flags = Qt::AlignLeft;
            break;
        case HSSTextAlignTypeRight:
            flags = Qt::AlignRight;
            break;
        case HSSTextAlignTypeCenter:
            flags = Qt::AlignCenter;
            break;
        case HSSTextAlignTypeJustify:
            flags = Qt::AlignJustify;
            break;
        default:
            break;
    }

    d->canvasPainter->drawText(QRectF(textBlock.globalX, textBlock.globalY, textBlock.getWidth(), textBlock.getHeight()), textBlock.getText(), QTextOption(flags));

    if (d->globalAntialiasingEnabled)
        d->canvasPainter->setRenderHint(QPainter::Antialiasing);
}

void HSSRenderer::drawLinearGradient(HSSLinearGradient &gradient, const QPainterPath &path, HSSUnit posX, HSSUnit posY)
{
    QSharedPointer<HSSRgb> prevColor;
    QLinearGradient pat(gradient.getStartX() + posX, gradient.getStartY() + posY, gradient.getEndX() + posX, gradient.getEndY() + posY);
    QSharedPointer<HSSObject> startColorObj = gradient.getStartColor();
    if (startColorObj && startColorObj->isA(HSSObjectTypeRgb))
    {
        QSharedPointer<HSSRgb> startColor = qSharedPointerCast<HSSRgb>(startColorObj);
        pat.setColorAt(0, startColor->toQColor());
        prevColor = startColor;
    }
    else
    {
        QSharedPointer<HSSRgb> nextColor = gradient.getColorAfterFirst();
        pat.setColorAt(0, nextColor->toQColorWithAlpha(0));
        prevColor = nextColor;
    }

    //add color stops
    const QSharedPointer<HSSObject> colorStops = gradient.getColorStops();
    //workaround for https://bugreports.qt-project.org/browse/QTBUG-3793.
    QList<HSSUnit> positions;
    if (colorStops)
    {
        if (colorStops->isA(HSSObjectTypeMultipleValue))
        {
            QSharedPointer<HSSMultipleValue> multiValue = qSharedPointerCast<HSSMultipleValue>(colorStops);
            QListIterator<QSharedPointer<HSSObject> > it(multiValue->getValues());
            while (it.hasNext())
            {
                this->_addColorStops(pat, gradient, prevColor, positions, it.next(), colorStops);
            }
        }
        else
        {
            this->_addColorStops(pat, gradient, prevColor, positions, colorStops, colorStops);
        }
    }

    QSharedPointer<HSSObject> endColorObj = gradient.getEndColor();
    if (endColorObj && endColorObj->isA(HSSObjectTypeRgb))
    {
        QSharedPointer<HSSRgb> endColor = qSharedPointerCast<HSSRgb>(endColorObj);
        pat.setColorAt(1, endColor->toQColor());
    }
    else
    {
        QSharedPointer<HSSRgb> nextColor = gradient.getColorBeforeLast();
        pat.setColorAt(1, nextColor->toQColorWithAlpha(0));
    }

    QBrush brush(pat);
    d->canvasPainter->fillPath(path, brush);
}

void HSSRenderer::_addColorStops(QLinearGradient & pat, HSSLinearGradient &gradient, QSharedPointer<HSSRgb> & prevColor, QList<HSSUnit> & positions, QSharedPointer<HSSObject> theStopObj, const QSharedPointer<HSSObject> &colorStops)
{
    switch (theStopObj->getObjectType())
    {
        case HSSObjectTypeColorStop:
        {
            QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop > (theStopObj);

            //calculate the position
            HSSUnit width = (gradient.getEndX() - gradient.getStartX());
            HSSUnit height = (gradient.getEndY() - gradient.getStartY());
            HSSUnit hypotenuse = hypot(width, height);
            HSSUnit position;
            if (theStop->positionIsPercentage()) {
                QSharedPointer<HSSObject> percentageValue = theStop->getComputedValue("position");
                if (!percentageValue->isA(HSSObjectTypeValue) || !qSharedPointerCast<HSSValue>(percentageValue)->getValue()->isA(HSSParserNodeTypePercentageConstant))
                {
                    AXRError("HSSRenderer", "Position is percentage, bad object type while reading percentage constant. Please report this bug.");
                }
                QSharedPointer<HSSPercentageConstant> percentageConstant = qSharedPointerCast<HSSPercentageConstant>(qSharedPointerCast<HSSValue>(percentageValue)->getValue());
                position = percentageConstant->getNumber();
            }
            else
            {
                position = theStop->getPosition() / hypotenuse;
            }

            if (position < 0) position = 0;
            if (position > 1) position = 1;

            //determine the color
            QSharedPointer<HSSObject> theColorObj = theStop->getColor();
            if (theColorObj && theColorObj->isA(HSSObjectTypeRgb))
            {
                QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                pat.setColorAt(this->_nextFreePosition(positions, position), theColor->toQColor());
                prevColor = theColor;
            }
            else
            {
                //create two stops:
                //one with the previous color
                pat.setColorAt(this->_nextFreePosition(positions, position), prevColor->toQColorWithAlpha(0));
                //and one with the next color
                if (colorStops->isA(HSSObjectTypeColorStop))
                {
                    QSharedPointer<HSSObject> endColorObj = gradient.getEndColor();
                    if (endColorObj->isA(HSSObjectTypeRgb))
                    {
                        QSharedPointer<HSSRgb> endColor = qSharedPointerCast<HSSRgb>(endColorObj);
                        pat.setColorAt(this->_nextFreePosition(positions, 0.5), endColor->toQColor());
                    }

                }
                else if (colorStops->isA(HSSObjectTypeMultipleValue))
                {
                    QListIterator<QSharedPointer<HSSObject> > innerIt(qSharedPointerCast<HSSMultipleValue>(colorStops)->getValues());
                    innerIt.findNext(theStopObj);
                    QSharedPointer<HSSRgb> nextColor = gradient.getNextColorFromStops(innerIt);
                    pat.setColorAt(this->_nextFreePosition(positions, position), nextColor->toQColorWithAlpha(0));
                }
            }
            break;
        }

        case HSSObjectTypeRgb:
        {
            QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb > (theStopObj);
            pat.setColorAt(this->_nextFreePosition(positions, 0.5), theColor->toQColor());
            break;
        }
        case HSSObjectTypeValue:
        {
            QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(theStopObj);
            QSharedPointer<HSSParserNode> parserNode = valueObj->getValue();
            if (parserNode->isA(HSSParserNodeTypeKeywordConstant) && qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "transparent") {
                //create two stops:
                //one with the previous color
                pat.setColorAt(this->_nextFreePosition(positions, 0.5), prevColor->toQColorWithAlpha(0));
                pat.setColorAt(this->_nextFreePosition(positions, 0.5), gradient.getColorBeforeLast()->toQColorWithAlpha(0));
                break;
            }
        }
        default:
            AXRWarning("HSSLinearGradient", "The color stop had no color defined").raise();
            break;
    }
}

HSSUnit HSSRenderer::_nextFreePosition(QList<HSSUnit> &positions, HSSUnit position)
{
    HSSUnit ret = position;
    while(positions.contains(ret))
    {
        ret += 0.0000000001;
    }
    positions.append(ret);
    return ret;
}

void HSSRenderer::drawRadialGradient(HSSRadialGradient &gradient, const QPainterPath &path, HSSUnit posX, HSSUnit posY)
{
    QSharedPointer<HSSRgb> prevColor;
    qreal offset = sqrt((gradient.getOffsetX()*gradient.getOffsetX())+(gradient.getOffsetY()*gradient.getOffsetY()));
    QRadialGradient pat(gradient.getCenterX() + posX, gradient.getCenterY() + posY, offset, gradient.getCenterX() + posX, gradient.getCenterY() + posY);
    QSharedPointer<HSSObject> startColorObj = gradient.getStartColor();
    if (startColorObj && startColorObj->isA(HSSObjectTypeRgb))
    {
        QSharedPointer<HSSRgb> startColor = qSharedPointerCast<HSSRgb>(startColorObj);
        pat.setColorAt(0, startColor->toQColor());
        prevColor = startColor;
    }
    else
    {
        QSharedPointer<HSSRgb> nextColor = gradient.getColorAfterFirst();
        pat.setColorAt(0, nextColor->toQColorWithAlpha(0));
        prevColor = nextColor;
    }

    QSharedPointer<HSSObject> endColorObj = gradient.getEndColor();
    if (endColorObj && endColorObj->isA(HSSObjectTypeRgb))
    {
        QSharedPointer<HSSRgb> endColor = qSharedPointerCast<HSSRgb>(endColorObj);
        pat.setColorAt(1, endColor->toQColor());
    }
    else
    {
        QSharedPointer<HSSRgb> prevColor = gradient.getColorBeforeLast();
        pat.setColorAt(1, prevColor->toQColorWithAlpha(0));
    }

    QBrush brush(pat);
    d->canvasPainter->fillPath(path, brush);
}

void HSSRenderer::performLayoutSteps(HSSDisplayObject &displayObject)
{
    axr_log(LoggerChannelLayout, "DISPLAYOBJECT: " + displayObject.name);
    AXRDocument *document = displayObject.getController()->document();
    if (document->showLayoutSteps())
    {
        document->nextLayoutTick();
        if (document->layoutChildDone())
            return;
    }
}

void HSSRenderer::initializeVisit()
{
    axr_log(LoggerChannelRendering, "INITIALIZE_RENDER");
    //prepare values
    QSharedPointer<HSSContainer> root = d->document->controller()->root();

    if (root)
    {
        if (!d->globalAntialiasingEnabled)
            d->canvasPainter->setRenderHint(QPainter::NonCosmeticDefaultPen);
        ///@todo find out what objects lie in that rect

        // If the window size has changed, make new size
        int windowWidth = d->document->windowWidth();
        int windowHeight = d->document->windowHeight();
        if (d->bounds.size.width != windowWidth || d->bounds.size.height != windowHeight)
        {
            axr_log(LoggerChannelRendering, AXRString("HSSRenderer: window size changed, setting to width: %1 and height: %2").arg((int)windowWidth).arg((int)windowHeight));

            d->bounds.size.width = windowWidth;
            d->bounds.size.height = windowHeight;
            regeneratePainter(windowWidth, windowHeight);

            d->repaintAll = true;
        }

        d->canvas.fill(Qt::white);
        if (d->document->showLayoutSteps())
        {
            d->document->resetLayoutTicks();
        }
        d->document->nextLayoutChild();
    }
    else
    {
        AXRError("HSSVisitorManager", "Fatal error: No root").raise();
    }
}

void HSSRenderer::setOutputBoundsToObject(QSharedPointer<HSSDisplayObject> outputBoundsObject)
{
    d->hasOutputBoundsObject = true;
    d->outputBoundsObject = outputBoundsObject;
}

void HSSRenderer::finalizeVisit()
{
    axr_log(LoggerChannelRendering, "FINALIZE_RENDER");
}

void HSSRenderer::reset()
{
    d->bounds.size.width = 0;
    d->bounds.size.height = 0;
}

bool HSSRenderer::isGlobalAntialiasingEnabled() const
{
    return d->globalAntialiasingEnabled;
}

void HSSRenderer::setGlobalAntialiasingEnabled(bool enable)
{
    if (d->globalAntialiasingEnabled != enable)
    {
        d->globalAntialiasingEnabled = enable;
        d->repaintAll = true;
    }
}

void HSSRenderer::regeneratePainter(int width, int height)
{
    if (d->canvasPainter)
        delete d->canvasPainter;

    d->canvas = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    d->canvasPainter = new QPainter(&d->canvas);
}

QImage HSSRenderer::getFinalFrame() const
{
    return d->canvas;
}

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>

CGImageRef HSSRenderer::getFinalFrameAsCGImageRef() const
{
    return QPixmap::fromImage(getFinalFrame()).toMacCGImageRef();
}
#endif
