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

#include "axr.h"
#include "HSSRenderer.h"

using namespace AXR;

class HSSRenderer::Private
{
public:
    Private() :
    repaintAll(true), document(),
    dirtyRect(), bounds()
    {
    }

    

    bool repaintAll;
    AXRDocument *document;
    HSSRect dirtyRect;
    HSSRect bounds;
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
            axr_log(LoggerChannelRendering, HSSString::format("HSSRenderer: redrawing contents of %s with width: %f, height: %f, x: %f and y: %f", container.elementName.chardata(), container.getWidth(), container.getHeight(), container.globalX, container.globalY));

            container._isDirty = false;
            drawBackground(container);
            drawStrokes(container);
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
            axr_log(LoggerChannelRendering, HSSString::format("HSSRenderer: redrawing contents of %s with width: %f, height: %f, x: %f and y: %f", textBlock.elementName.chardata(), textBlock.getWidth(), textBlock.getHeight(), textBlock.x, textBlock.y));
            textBlock._isDirty = false;
            drawForeground(textBlock);
        }
    }
    else
    {
        axr_log(LoggerChannelRendering, "HSSRenderer: skipping (not visible) " + textBlock.elementName);
    }
}

void HSSRenderer::drawStrokes(HSSContainer &container)
{
    QSharedPointer<HSSObject> strokeObj = container.getStroke();
    std::list<QSharedPointer<HSSAbstractStroke> > strokes;
    if (strokeObj)
    {
        if (strokeObj->isA(HSSObjectTypeStroke))
        {
            strokes.push_back(qSharedPointerCast<HSSAbstractStroke>(strokeObj));
        }
        else if (strokeObj->isA(HSSObjectTypeMultipleValue))
        {
            std::vector<QSharedPointer<HSSObject> >::const_iterator it;
            const std::vector<QSharedPointer<HSSObject> > & values = qSharedPointerCast<HSSMultipleValue>(strokeObj)->getValues();
            for (it = values.begin(); it != values.end(); ++it)
            {
                const QSharedPointer<HSSObject> & bObj = *it;
                if (bObj->isA(HSSObjectTypeStroke))
                {
                    strokes.push_back(qSharedPointerCast<HSSAbstractStroke>(bObj));
                }
            }
        }
    }
    if (strokes.size() > 0)
    {
        container.getShape()->drawStrokes(strokes, container.getWidth(), container.getHeight(), container.globalX, container.globalY);
    }
}

void HSSRenderer::drawBackground(HSSContainer &container)
{
    QSharedPointer<HSSObject> background = container.getBackground();
    if (background)
    {
        QSharedPointer<HSSPath> path;
        container.getShape()->createPath(path, container.globalX, container.globalY, container.getWidth(), container.getHeight());
        if (background->isA(HSSObjectTypeMultipleValue))
        {
            std::vector<QSharedPointer<HSSObject> >::const_iterator it;
            const std::vector<QSharedPointer<HSSObject> > & values = qSharedPointerCast<HSSMultipleValue>(background)->getValues();
            for (it = values.begin(); it != values.end(); ++it)
            {
                const QSharedPointer<HSSObject> & theObj = *it;
                this->_drawBackground(path, theObj, container.globalX, container.globalY);
            }
        }
        else
        {
            this->_drawBackground(path, background, container.globalX, container.globalY);
        }

    }
}

void HSSRenderer::_drawBackground(QSharedPointer<HSSPath> & path, QSharedPointer<HSSObject> theobj, HSSUnit globalX, HSSUnit globalY)
{
    switch (theobj->getObjectType())
    {
        case HSSObjectTypeRgb:
        {
            QSharedPointer<HSSRgb> color = qSharedPointerCast<HSSRgb > (theobj);
            d->document->platform()->fillPath(path, color);
            break;
        }

        case HSSObjectTypeGradient:
        {
            if (theobj->isA(HSSGradientTypeLinear))
            {
                QSharedPointer<HSSLinearGradient> grad = qSharedPointerCast<HSSLinearGradient > (theobj);
                drawLinearGradient(grad, path, globalX, globalY);
            }
            else if (theobj->isA(HSSGradientTypeRadial))
            {
                QSharedPointer<HSSRadialGradient> grad = qSharedPointerCast<HSSRadialGradient > (theobj);
                drawRadialGradient(grad, path, globalX, globalY);
            }
            else
            {
                AXRError("HSSRenderer", "Unknown gradient type.").raise();
            }

            break;
        }

        default:
            AXRError("HSSRenderer", "Invalid value type in background property.").raise();
            break;
    }
}

void HSSRenderer::drawForeground(HSSTextBlock& textBlock)
{
    HSSRect textRect;
    textRect.size.width = textBlock.getWidth();
    textRect.size.height = textBlock.getHeight();
    textRect.origin.x = textBlock.globalX;
    textRect.origin.y = textBlock.globalY;
    
    QSharedPointer<HSSFont> theFont;
    QSharedPointer<HSSObject> fontObj = textBlock.getFont();
    if (fontObj && fontObj->isA(HSSObjectTypeFont))
    {
        theFont = qSharedPointerCast<HSSFont>(fontObj);
    }
    d->document->platform()->renderText(textBlock.getText(), textRect, textBlock.getTextAlign(), theFont);
}

void HSSRenderer::drawLinearGradient(const QSharedPointer<HSSLinearGradient> &gradient, const QSharedPointer<HSSPath> &path, HSSUnit posX, HSSUnit posY)
{
    QSharedPointer<HSSRgb> prevColor;
    QSharedPointer<HSSRenderGradient> pat(new HSSRenderGradient());
    pat->setStartX(gradient->getStartX() + posX);
    pat->setStartY(gradient->getStartY() + posY);
    pat->setEndX(gradient->getEndX() + posX);
    pat->setEndY(gradient->getEndY() + posY);

    QSharedPointer<HSSObject> startColorObj = gradient->getStartColor();
    if (startColorObj && startColorObj->isA(HSSObjectTypeRgb))
    {
        QSharedPointer<HSSRgb> startColor = qSharedPointerCast<HSSRgb>(startColorObj);
        pat->setColorAt(0, startColor);
        prevColor = startColor;
    }
    else
    {
        QSharedPointer<HSSRgb> nextColor = gradient->getColorAfterFirst();
        pat->setColorAt(0, nextColor);
        prevColor = nextColor;
    }

    //add color stops
    const QSharedPointer<HSSObject> colorStops = gradient->getColorStops();
    //workaround for https://bugreports.qt-project.org/browse/QTBUG-3793.
    std::vector<HSSUnit> positions;
    if (colorStops)
    {
        if (colorStops->isA(HSSObjectTypeMultipleValue))
        {
            QSharedPointer<HSSMultipleValue> multiValue = qSharedPointerCast<HSSMultipleValue>(colorStops);
            std::vector<QSharedPointer<HSSObject> > valuesList = multiValue->getValues();
            std::vector<QSharedPointer<HSSObject> >::iterator it;
            for (it = valuesList.begin(); it != valuesList.end(); ++it)
            {
                QSharedPointer<HSSObject> theVal = *it;
                this->_addColorStops(pat, gradient, prevColor, positions, theVal, colorStops);
            }
        }
        else
        {
            this->_addColorStops(pat, gradient, prevColor, positions, colorStops, colorStops);
        }
    }

    QSharedPointer<HSSObject> endColorObj = gradient->getEndColor();
    if (endColorObj && endColorObj->isA(HSSObjectTypeRgb))
    {
        QSharedPointer<HSSRgb> endColor = qSharedPointerCast<HSSRgb>(endColorObj);
        pat->setColorAt(1, endColor);
    }
    else
    {
        QSharedPointer<HSSRgb> nextColor = gradient->getColorBeforeLast();
        pat->setColorAt(1, nextColor);
    }

    d->document->platform()->fillPathGradient(path, pat);
}

void HSSRenderer::_addColorStops(const QSharedPointer<HSSRenderGradient> & pat, const QSharedPointer<HSSLinearGradient> & gradient, QSharedPointer<HSSRgb> & prevColor, std::vector<HSSUnit> & positions, QSharedPointer<HSSObject> theStopObj, const QSharedPointer<HSSObject> &colorStops)
{
    switch (theStopObj->getObjectType())
    {
        case HSSObjectTypeColorStop:
        {
            QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop > (theStopObj);

            //calculate the position
            HSSUnit width = (gradient->getEndX() - gradient->getStartX());
            HSSUnit height = (gradient->getEndY() - gradient->getStartY());
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
                pat->setColorAt(this->_nextFreePosition(positions, position), theColor);
                prevColor = theColor;
            }
            else
            {
                //create two stops:
                //one with the previous color
                pat->setColorAt(this->_nextFreePosition(positions, position), prevColor);
                //and one with the next color
                if (colorStops->isA(HSSObjectTypeColorStop))
                {
                    QSharedPointer<HSSObject> endColorObj = gradient->getEndColor();
                    if (endColorObj->isA(HSSObjectTypeRgb))
                    {
                        QSharedPointer<HSSRgb> endColor = qSharedPointerCast<HSSRgb>(endColorObj);
                        pat->setColorAt(this->_nextFreePosition(positions, 0.5), endColor);
                    }

                }
                else if (colorStops->isA(HSSObjectTypeMultipleValue))
                {
                    std::vector<QSharedPointer<HSSObject> > valuesList = qSharedPointerCast<HSSMultipleValue>(colorStops)->getValues();
                    QSharedPointer<HSSRgb> nextColor = gradient->getNextColorFromStops(theStopObj, valuesList);
                    pat->setColorAt(this->_nextFreePosition(positions, position), nextColor);
                }
            }
            break;
        }

        case HSSObjectTypeRgb:
        {
            QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb > (theStopObj);
            pat->setColorAt(this->_nextFreePosition(positions, 0.5), theColor);
            break;
        }
        case HSSObjectTypeValue:
        {
            QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(theStopObj);
            QSharedPointer<HSSParserNode> parserNode = valueObj->getValue();
            AXRString kwValue = "";
            if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
            {
                kwValue = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
                if (kwValue == "transparent")
                {
                    //create two stops:
                    //one with the previous color
                    pat->setColorAt(this->_nextFreePosition(positions, 0.5), prevColor);
                    pat->setColorAt(this->_nextFreePosition(positions, 0.5), gradient->getColorBeforeLast());
                    break;
                }
                else if (kwValue == "no")
                {
                    //do nothing
                    break;
                }
            }
        }
        default:
            AXRWarning("HSSLinearGradient", "The color stop had no color defined").raise();
            break;
    }
}

HSSUnit HSSRenderer::_nextFreePosition(std::vector<HSSUnit> &positions, HSSUnit position)
{
    HSSUnit ret = position;
    while(std::find(positions.begin(), positions.end(), ret) != positions.end())
    {
        ret += 0.0000000001;
    }
    positions.push_back(ret);
    return ret;
}

void HSSRenderer::drawRadialGradient(const QSharedPointer<HSSRadialGradient> &gradient, const QSharedPointer<HSSPath> &path, HSSUnit posX, HSSUnit posY)
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
        ///@todo find out what objects lie in that rect

        // If the window size has changed, make new size
        int windowWidth = d->document->windowWidth();
        int windowHeight = d->document->windowHeight();
        if (d->bounds.size.width != windowWidth || d->bounds.size.height != windowHeight)
        {
            axr_log(LoggerChannelRendering, HSSString::format("HSSRenderer: window size changed, setting to width: %d and height: %d", (int)windowWidth, (int)windowHeight));

            d->bounds.size.width = windowWidth;
            d->bounds.size.height = windowHeight;
            d->document->platform()->regeneratePainter(windowWidth, windowHeight);

            d->repaintAll = true;
        }

        d->document->platform()->initializeRender();
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

void HSSRenderer::finalizeVisit()
{
    axr_log(LoggerChannelRendering, "FINALIZE_RENDER");
}

void HSSRenderer::reset()
{
    d->bounds.size.width = 0;
    d->bounds.size.height = 0;
}

