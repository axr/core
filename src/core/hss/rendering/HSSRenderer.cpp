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
#include "HSSLinearGradient.h"
#include "HSSRadialGradient.h"
#include "HSSRect.h"
#include "HSSRenderer.h"
#include "HSSRoundedRect.h"
#include "HSSShape.h"
#include "HSSTextBlock.h"
#include "HSSColorStop.h"
#include "HSSParserNode.h"
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
    if (container.visible)
    {
        performLayoutSteps(container);

        if (container._isDirty || d->repaintAll)
        {
            axr_log(LoggerChannelRendering, AXRString("HSSRenderer: redrawing contents of %1 with x: %2 and y: %3").arg(container.elementName).arg(container.x).arg(container.y));

            container._isDirty = false;
            drawBackground(container);
            if (container.border.size() > 0)
            {
                drawBorders(container);
            }
        }
    }
    else
    {
        axr_log(LoggerChannelRendering, "HSSRenderer: skipping (not visible) " + container.elementName);
    }
}

void HSSRenderer::visit(HSSTextBlock &textBlock)
{
    if (textBlock.visible)
    {
        axr_log(LoggerChannelRendering, "HSSRenderer: drawing " + textBlock.elementName);
        performLayoutSteps(textBlock);

        if (textBlock._isDirty || d->repaintAll)
        {
            axr_log(LoggerChannelRendering, AXRString("HSSRenderer: redrawing contents of %1 with x: %2 and y: %3").arg(textBlock.elementName).arg(textBlock.x).arg(textBlock.y));
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

    container.getShape()->drawBorders(*d->canvasPainter, container.border, container.width, container.height, container.globalX, container.globalY);
}

void HSSRenderer::drawBackground(HSSContainer &container)
{
    if (d->globalAntialiasingEnabled)
        d->canvasPainter->setRenderHint(QPainter::Antialiasing);

    QPainterPath path;

    container.shape->createPath(path, container.globalX, container.globalY, container.width, container.height);

    for (std::vector<QSharedPointer<HSSObject> >::iterator it = container.background.begin(); it != container.background.end(); ++it)
    {
        QSharedPointer<HSSObject> theobj = *it;
        switch (theobj->getObjectType())
        {
            case HSSObjectTypeRgb:
            {
                QSharedPointer<HSSRgb> color = qSharedPointerCast<HSSRgb > (theobj);

                d->canvasPainter->fillPath(path, color ? color->toQColor() : QColor(0, 0, 0, 0));

                break;
            }

            case HSSObjectTypeGradient:
            {
                if (theobj->isA(HSSGradientTypeLinear))
                {
                    QSharedPointer<HSSLinearGradient> grad = qSharedPointerCast<HSSLinearGradient > (theobj);
                    drawLinearGradient(*grad, path, container.globalX, container.globalY);
                }
                else if (theobj->isA(HSSGradientTypeRadial))
                {
                    QSharedPointer<HSSRadialGradient> grad = qSharedPointerCast<HSSRadialGradient > (theobj);
                    drawRadialGradient(*grad, path, container.globalX, container.globalY);
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
}

void HSSRenderer::drawForeground(HSSTextBlock& textBlock)
{
    if (d->globalAntialiasingEnabled)
        d->canvasPainter->setRenderHint(QPainter::TextAntialiasing);

    QSharedPointer<HSSFont> theFont;
    if (textBlock.font.size() > 0)
        theFont = *textBlock.font.begin();

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

    d->canvasPainter->setFont(textBlock.getFont());
    Qt::Alignment flags = 0;
    switch (textBlock.textAlign)
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

    d->canvasPainter->drawText(QRectF(textBlock.globalX, textBlock.globalY, textBlock.width, textBlock.height), textBlock.getText(), QTextOption(flags));

    if (d->globalAntialiasingEnabled)
        d->canvasPainter->setRenderHint(QPainter::Antialiasing);
}

void HSSRenderer::drawLinearGradient(HSSLinearGradient &gradient, const QPainterPath &path, HSSUnit posX, HSSUnit posY)
{
    QSharedPointer<HSSRgb> prevColor;
    QLinearGradient pat(gradient.startX + posX, gradient.startY + posY, gradient.endX + posX, gradient.endY + posY);
    if (gradient.startColor)
    {
        pat.setColorAt(0, gradient.startColor->toQColor());
        prevColor = gradient.startColor;
    }
    else
    {
        QSharedPointer<HSSRgb> nextColor = gradient.getColorAfterFirst();
        pat.setColorAt(0, nextColor->toQColorWithAlpha(0));
        prevColor = nextColor;
    }

    //add color stops
    for (std::vector<QSharedPointer<HSSObject> >::iterator it = gradient.colorStops.begin(); it != gradient.colorStops.end(); ++it)
    {
        QSharedPointer<HSSObject> theStopObj = *it;
        //if it's a color stop
        if (theStopObj->isA(HSSObjectTypeColorStop))
        {
            QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop > (theStopObj);

            //calculate the position
            HSSUnit position;
            if (theStop->getDPosition()->isA(HSSParserNodeTypePercentageConstant))
            {
                position = theStop->getPosition();
            }
            else
            {
                HSSUnit width = (gradient.endX - gradient.startX);
                HSSUnit height = (gradient.endY - gradient.startY);
                HSSUnit hypotenuse = hypot(width, height);
                position = theStop->getPosition() / hypotenuse;
            }

            //determine the color
            QSharedPointer<HSSRgb> theColor = theStop->getColor();
            if (theColor)
            {
                pat.setColorAt(position, theColor->toQColor());
                prevColor = theColor;
            }
            else
            {
                //create two stops:
                //one with the previous color
                pat.setColorAt(position, prevColor->toQColorWithAlpha(0));
                //and one with the next color
                std::vector<QSharedPointer<HSSObject> >::iterator innerIt = it;
                ++innerIt;
                QSharedPointer<HSSRgb> nextColor = gradient.getNextColorFromStops(innerIt, gradient.colorStops.end());
                pat.setColorAt(position, nextColor->toQColorWithAlpha(0));
            }
        }
        //if it's a simple color
        else if (theStopObj->isA(HSSObjectTypeRgb))
        {
            QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb > (theStopObj);
            pat.setColorAt(0.5, theColor->toQColor());
        }
        else
        {
            AXRWarning("HSSLinearGradient", "The color stop had no color defined").raise();
        }

    }

    if (gradient.endColor)
    {
        pat.setColorAt(1, gradient.endColor->toQColor());
    }
    else
    {
        QSharedPointer<HSSRgb> prevColor = gradient.getColorBeforeLast();
        pat.setColorAt(1, prevColor->toQColorWithAlpha(0));
    }

    QBrush brush(pat);
    d->canvasPainter->fillPath(path, brush);
}

void HSSRenderer::drawRadialGradient(HSSRadialGradient &gradient, const QPainterPath &path, HSSUnit posX, HSSUnit posY)
{
    QSharedPointer<HSSRgb> prevColor;
    qreal offset = sqrt((gradient.offsetX*gradient.offsetX)+(gradient.offsetY*gradient.offsetY));
    QRadialGradient pat(gradient.centerX + posX, gradient.centerY + posY, offset, gradient.centerX + posX, gradient.centerY + posY);
    if (gradient.startColor)
    {
        pat.setColorAt(0, gradient.startColor->toQColor());
        prevColor = gradient.startColor;
    }
    else
    {
        QSharedPointer<HSSRgb> nextColor = gradient.getColorAfterFirst();
        pat.setColorAt(0, nextColor->toQColorWithAlpha(0));
        prevColor = nextColor;
    }

    //add color stops
    for (std::vector<QSharedPointer<HSSObject> >::iterator it = gradient.colorStops.begin(); it != gradient.colorStops.end(); ++it)
    {
        QSharedPointer<HSSObject> theStopObj = *it;
        //if it's a color stop
        if (theStopObj->isA(HSSObjectTypeColorStop))
        {
            QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop > (theStopObj);

            //calculate the position
            HSSUnit position;
            if (theStop->getDPosition()->isA(HSSParserNodeTypePercentageConstant))
            {
                position = theStop->getPosition();
            }
            else
            {
                HSSUnit width = (gradient.offsetX - gradient.centerX);
                HSSUnit height = (gradient.offsetY - gradient.centerY);
                HSSUnit hypotenuse = hypot(width, height);
                position = theStop->getPosition() / hypotenuse;
            }

            //determine the color
            QSharedPointer<HSSRgb> theColor = theStop->getColor();
            if (theColor)
            {
                pat.setColorAt(position, theColor->toQColor());
                prevColor = theColor;
            }
            else
            {
                //create two stops:
                //one with the previous color
                pat.setColorAt(position, prevColor->toQColorWithAlpha(0));
                //and one with the next color
                std::vector<QSharedPointer<HSSObject> >::iterator innerIt = it;
                ++innerIt;
                QSharedPointer<HSSRgb> nextColor = gradient.getNextColorFromStops(innerIt, gradient.colorStops.end());
                pat.setColorAt(position, nextColor->toQColorWithAlpha(0));
            }
        }
        //if it's a simple color
        else if (theStopObj->isA(HSSObjectTypeRgb))
        {
            QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb > (theStopObj);
            pat.setColorAt(0.5, theColor->toQColor());
        }
        else
        {
            AXRWarning("HSSRadialGradient", "The color stop had no color defined").raise();
        }
    }

    if (gradient.endColor)
    {
        pat.setColorAt(1, gradient.endColor->toQColor());
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
        HSSUnit windowWidth = d->document->windowWidth();
        HSSUnit windowHeight = d->document->windowHeight();
        if (d->bounds.size.width != windowWidth || d->bounds.size.height != windowHeight)
        {
            axr_log(LoggerChannelRendering, AXRString("HSSVisitorManager: window size changed, setting to width: %1 and height: %2").arg((int)d->bounds.size.width).arg((int)d->bounds.size.height));

            d->bounds.size.width = windowWidth;
            d->bounds.size.height = windowHeight;
            regeneratePainter(windowWidth, windowHeight);

            d->repaintAll = true;
            root->setNeedsRereadRules(true);
        }

        d->canvas.fill(Qt::white);
        //draw the elements
        axr_log(LoggerChannelGeneralSpecific, "HSSVisitorManager: reading object definitions");
        root->recursiveReadDefinitionObjects();
        axr_log(LoggerChannelLayout, "HSSVisitorManager: laying out elements on page");
        root->recursiveLayout();
        if (d->document->showLayoutSteps())
        {
            d->document->resetLayoutTicks();
        }

        axr_log(LoggerChannelRendering, "HSSVisitorManager: regenerating surfaces");
        //regenerateRootSurface();
        axr_log(LoggerChannelRendering, "HSSVisitorManager: drawing tree");
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
