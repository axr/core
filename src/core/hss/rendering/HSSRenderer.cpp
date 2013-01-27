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

#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRError.h"
#include "HSSBorder.h"
#include "HSSContainer.h"
#include "HSSFont.h"
#include "HSSLinearGradient.h"
#include "HSSRadialGradient.h"
#include "HSSRenderer.h"
#include "HSSRgb.h"
#include "HSSRoundedRect.h"
#include "HSSTextBlock.h"
#include <QPainter>

#ifndef QT_NO_DEBUG
#define IFAXRDEBUG(OPERATION) OPERATION
#else
#define IFAXRDEBUG(OPERATION)
#endif

using namespace AXR;

HSSRenderer::HSSRenderer()
: _globalAntialiasingEnabled(true), _repaintAll(true), rootSurface(NULL)
{
}

void HSSRenderer::setDocument(AXRDocument* document)
{
    _document = document;
}

void HSSRenderer::visit(HSSContainer &container)
{
    if (container.visible)
    {
        performLayoutSteps(container);

        if (container._isDirty || _repaintAll)
        {
            axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, AXRString("HSSRenderer: redrawing contents of %1 with x: %2 and y: %3").arg(container.elementName).arg(container.x).arg(container.y));

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
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRenderer: skipping (not visible) " + container.elementName);
    }
}

void HSSRenderer::visit(HSSTextBlock &textBlock)
{
    if (textBlock.visible)
    {
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRenderer: drawing " + textBlock.elementName);
        performLayoutSteps(textBlock);


        if (textBlock._isDirty || _repaintAll)
        {
            axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, AXRString("HSSRenderer: redrawing contents of %1 with x: %2 and y: %3").arg(textBlock.elementName).arg(textBlock.x).arg(textBlock.y));
            textBlock._isDirty = false;
            drawForeground(textBlock);
        }
    }
    else
    {
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRenderer: skipping (not visible) " + textBlock.elementName);
    }
}

void HSSRenderer::drawBorders(HSSContainer &container)
{
    if (_globalAntialiasingEnabled)
        _canvasPainter->setRenderHint(QPainter::Antialiasing);

    // Calculate the combined thickness of all borders
    HSSUnit combinedThickness = 0;
    for (HSSBorder::it it = container.border.begin(); it != container.border.end(); ++it)
    {
        combinedThickness += (*it)->getSize();
    }

    // Correction if needed
    HSSUnit correction;
    if ((int) combinedThickness % 2)
    {
        correction = 0.5;
    }

    // Cumulative combined thickness
    HSSUnit cumulativeThickness = 0;

    // Use a temporary trick for not having path offsets yet (will be fixed in the future)
    bool isRoundedRect = container.shape->isA(HSSShapeTypeRoundedRect);

    // Draw all borders
    for (HSSBorder::it it = container.border.begin(); it != container.border.end(); ++it)
    {
        QSharedPointer<HSSBorder> theBorder = *it;
        HSSUnit theSize = theBorder->getSize();

        HSSUnit offset = (combinedThickness / 2) - cumulativeThickness - (theSize / 2) + correction;

        QPainterPath path;
        if(isRoundedRect){
            QSharedPointer<HSSRoundedRect> roundedRect = qSharedPointerCast<HSSRoundedRect>(container.shape);
            roundedRect->createRoundedRect(path, container.globalX + container.borderBleeding + offset, container.globalY + container.borderBleeding + offset, container.width - offset * 2, container.height - offset * 2, -offset*2);
        } else {
            container.shape->createPath(path, container.globalX + container.borderBleeding + offset, container.globalY + container.borderBleeding + offset, container.width - offset * 2, container.height - offset * 2);
        }
        theBorder->draw(*_canvasPainter, path);

        cumulativeThickness += theSize;
    }
}

void HSSRenderer::drawBackground(HSSContainer &container)
{
    if (_globalAntialiasingEnabled)
        _canvasPainter->setRenderHint(QPainter::Antialiasing);

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

                HSSUnit r = 0., g = 0., b = 0., a = 0;
                if (color)
                {
                    r = color->getRed();
                    g = color->getGreen();
                    b = color->getBlue();
                    a = color->getAlpha();
                }

                _canvasPainter->fillPath(path, QColor(r, g, b, a));

                break;
            }

            case HSSObjectTypeGradient:
            {
                if (theobj->isA(HSSGradientTypeLinear)){
                    QSharedPointer<HSSLinearGradient> grad = qSharedPointerCast<HSSLinearGradient > (theobj);
                    grad->draw(*_canvasPainter, path);
                } else if (theobj->isA(HSSGradientTypeRadial)){
                    QSharedPointer<HSSRadialGradient> grad = qSharedPointerCast<HSSRadialGradient > (theobj);
                    grad->draw(*_canvasPainter, path);
                } else {
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
    if (_globalAntialiasingEnabled)
        _canvasPainter->setRenderHint(QPainter::TextAntialiasing);

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

    _canvasPainter->setPen(pen);

    _canvasPainter->setFont(textBlock.getFont());
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
    _canvasPainter->drawText(QRectF(textBlock.globalX, textBlock.globalY, textBlock.width, textBlock.height), textBlock.getText(), QTextOption(flags));

    if (_globalAntialiasingEnabled)
        _canvasPainter->setRenderHint(QPainter::Antialiasing);
}

void HSSRenderer::performLayoutSteps(HSSDisplayObject &displayObject)
{
    axr_log(AXR_DEBUG_CH_EVENTS_SPECIFIC, "DISPLAYOBJECT: " + displayObject.name);
    AXRDocument *document = displayObject.getController()->document();
    if (document->showLayoutSteps())
    {
        document->nextLayoutTick();
        if (document->layoutChildDone()) return;
    }
}

void HSSRenderer::initializeVisit()
{
    axr_log(AXR_DEBUG_CH_EVENTS_SPECIFIC, "INITIALIZE_RENDER");
    //prepare values
    QSharedPointer<HSSContainer> root = _document->getController()->getRoot();

    if (root)
    {
        if (!_globalAntialiasingEnabled)
            _canvasPainter->setRenderHint(QPainter::NonCosmeticDefaultPen);
        ///@todo find out what objects lie in that rect

        // If the window size has changed, make new size
        HSSUnit windowWidth = _document->getWindowWidth();
        HSSUnit windowHeight = _document->getWindowHeight();
        if (_bounds.size.width != windowWidth || _bounds.size.height != windowHeight)
        {
            axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, AXRString("HSSVisitorManager: window size changed, setting to width: %1 and height: %2").arg((int)_bounds.size.width).arg((int)_bounds.size.height));

            _bounds.size.width = windowWidth;
            _bounds.size.height = windowHeight;
            regeneratePainter(windowWidth, windowHeight);

            _repaintAll = true;
            root->setNeedsRereadRules(true);
        }
        _canvas->fill(Qt::white);
        //draw the elements
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSVisitorManager: reading object definitions");
        root->recursiveReadDefinitionObjects();
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSVisitorManager: laying out elements on page");
        root->recursiveLayout();
        if (_document->showLayoutSteps())
        {
            _document->resetLayoutTicks();
        }

        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSVisitorManager: regenerating surfaces");
        //regenerateRootSurface();
        axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSVisitorManager: drawing tree");
        _document->nextLayoutChild();
    }
    else
    {
        AXRError("HSSVisitorManager", "Fatal error: No root").raise();
    }
}

void HSSRenderer::setOutputBoundsToObject(QSharedPointer<HSSDisplayObject> outputBoundsObject)
{
    _hasOutputBoundsObject = true;
    _outputBoundsObject = outputBoundsObject;
}

void HSSRenderer::finalizeVisit()
{
    axr_log(AXR_DEBUG_CH_EVENTS_SPECIFIC, "FINALIZE_RENDER");
}

void HSSRenderer::reset()
{
    _bounds.size.width = 0;
    _bounds.size.height = 0;
}

bool HSSRenderer::globalAntialiasingEnabled() const
{
    return this->_globalAntialiasingEnabled;
}

bool HSSRenderer::getGlobalAntialiasingEnabled()
{
    return _globalAntialiasingEnabled;
}

void HSSRenderer::setGlobalAntialiasingEnabled(bool enable)
{
    if (this->_globalAntialiasingEnabled != enable)
    {
        this->_globalAntialiasingEnabled = enable;
        this->_repaintAll = true;
    }
}

void HSSRenderer::regeneratePainter(int width, int height)
{
    if (_canvasPainter)
        delete _canvasPainter;
    if (_canvas)
        delete _canvas;

    _canvas = new QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    _canvasPainter = new QPainter(_canvas);
}

QImage* HSSRenderer::getFinalFrame()
{
    return _canvas;
}
