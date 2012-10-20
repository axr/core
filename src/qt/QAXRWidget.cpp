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

#include <QPaintEvent>
#include "AXRDocument.h"
#include "QAXRWidget.h"

using namespace AXR;

class QAXRWidget::Private
{
public:
    Private(AXRDocument *doc = NULL) : document(doc), backgroundFillColor(QColor(Qt::white)) { }

    AXRDocument *document;
    QColor backgroundFillColor;
};

QAXRWidget::QAXRWidget(AXRDocument *document, QWidget *parent)
: QWidget(parent), d(new Private(document))
{
    this->setMouseTracking(true);
}

QAXRWidget::QAXRWidget(QWidget *parent)
: QWidget(parent), d(new Private)
{
    this->setMouseTracking(true);
}

QAXRWidget::~QAXRWidget()
{
    delete d;
}

AXRDocument* QAXRWidget::document() const
{
    return d->document;
}

void QAXRWidget::setDocument(AXRDocument *document)
{
    d->document = document;
}

QColor QAXRWidget::backgroundFillColor() const
{
    return d->backgroundFillColor;
}

void QAXRWidget::setBackgroundFillColor(const QColor &color)
{
    d->backgroundFillColor = color;
}

void QAXRWidget::paintEvent(QPaintEvent *e)
{
    QRect paintRect = rect();

    // Fill the view with our background color...
    QPainter painter(this);
    painter.fillRect(paintRect, d->backgroundFillColor);

    if (!d->document)
        return;

    // Render the AXR document
    AXRRender::p renderer = d->document->getRender();
    if (renderer && d->document->getController()->getRoot())
    {
        // Render the final composite on to the screen
        d->document->drawInRectWithBounds(e->rect(), paintRect);
        painter.drawImage(paintRect.topLeft(), renderer->surface());
    }
}

void QAXRWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    HSSContainer::p root = d->document->getController()->getRoot();
    if (root)
    {
        HSSPoint thePoint(e->pos());
        root->handleEvent(AXR::HSSEventTypeDoubleClick, (void*)&thePoint);
        if (d->document->needsDisplay())
        {
            this->update();
            d->document->setNeedsDisplay(false);
        }
    }
}

void QAXRWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    HSSContainer::p root = d->document->getController()->getRoot();
    if (root)
    {
        HSSPoint thePoint(e->pos());
        root->handleEvent(AXR::HSSEventTypeMouseMove, (void*)&thePoint);
        if (d->document->needsDisplay())
        {
            this->update();
            d->document->setNeedsDisplay(false);
        }
    }
}

void QAXRWidget::mousePressEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    HSSContainer::p root = d->document->getController()->getRoot();
    if (root)
    {
        HSSPoint thePoint(e->pos());
        root->handleEvent(AXR::HSSEventTypeMouseDown, (void*)&thePoint);
        if (d->document->needsDisplay())
        {
            this->update();
            d->document->setNeedsDisplay(false);
        }
    }
}

void QAXRWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    HSSContainer::p root = d->document->getController()->getRoot();
    if (root)
    {
        HSSPoint thePoint(e->pos());
        root->handleEvent(AXR::HSSEventTypeMouseUp, (void*)&thePoint);
        root->handleEvent(AXR::HSSEventTypeClick, (void*)&thePoint);
        if (d->document->needsDisplay())
        {
            this->update();
            d->document->setNeedsDisplay(false);
        }
    }
}
