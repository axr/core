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

#include <QPainter>
#include <QPaintEvent>
#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "HSSContainer.h"
#include "HSSRect.h"
#include "HSSRenderer.h"
#include "HSSVisitorManager.h"
#include "QAXRWidget.h"

using namespace AXR;

class QAXRWidget::Private
{
public:
    Private(AXRDocument *doc = NULL) : document(doc), backgroundFillColor(QColor(Qt::white)) { }

    AXRDocument *document;
    QColor backgroundFillColor;

    HSSRenderer* renderVisitor;
};

QAXRWidget::QAXRWidget(AXRDocument *document, QWidget *parent)
: QWidget(parent), d(new Private(document))
{
    this->setMouseTracking(true);
    d->renderVisitor = new HSSRenderer();
    setDocument(document);
}

QAXRWidget::QAXRWidget(QWidget *parent)
: QWidget(parent), d(new Private)
{
    this->setMouseTracking(true);
    d->renderVisitor = new HSSRenderer();
}

QAXRWidget::~QAXRWidget()
{
    delete d;
}

AXRDocument* QAXRWidget::document() const
{
    return d->document;
}

HSSRenderer* QAXRWidget::renderer() const
{
    return d->renderVisitor;
}

void QAXRWidget::setDocument(AXRDocument *document)
{
    d->document = document;
    if (!d->document)
        return;

    d->renderVisitor->setDocument(document);
    d->document->getVisitorManager()->addVisitor(d->renderVisitor);

    this->update();
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
    d->document->setWindowSize(this->width(), this->height());

    // Fill the view with our background color...
    QPainter painter(this);
    painter.fillRect(paintRect, d->backgroundFillColor);

    if (!d->document)
        return;

    // Render the AXR document
    QSharedPointer<HSSVisitorManager> visitorManager = d->document->getVisitorManager();
    if (visitorManager && d->document->getController()->getRoot())
    {
        // Render the final image to the screen
        d->renderVisitor->setDirtyRect(rect());
        visitorManager->runVisitors(HSSAbstractVisitor::VisitorFilterAll);

        QImage* finalFrame = d->renderVisitor->getFinalFrame();
        if (finalFrame)
            painter.drawImage(paintRect.topLeft(), *finalFrame);
    }
}

void QAXRWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    QSharedPointer<HSSContainer> root = d->document->getController()->getRoot();
    if (root)
    {
        HSSPoint thePoint(e->pos());
        root->handleEvent(AXR::HSSEventTypeDoubleClick, (void*)&thePoint);
        if (d->document->needsDisplay())
        {
            this->update();
        }
    }
}

void QAXRWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    QSharedPointer<HSSContainer> root = d->document->getController()->getRoot();
    if (root)
    {
        HSSPoint thePoint(e->pos());
        root->handleEvent(AXR::HSSEventTypeMouseMove, (void*)&thePoint);
        if (d->document->needsDisplay())
        {
            this->update();
        }
    }
}

void QAXRWidget::mousePressEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    QSharedPointer<HSSContainer> root = d->document->getController()->getRoot();
    if (root)
    {
        HSSPoint thePoint(e->pos());
        root->handleEvent(AXR::HSSEventTypeMouseDown, (void*)&thePoint);
        if (d->document->needsDisplay())
        {
            this->update();
        }
    }
}

void QAXRWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    QSharedPointer<HSSContainer> root = d->document->getController()->getRoot();
    if (root)
    {
        HSSPoint thePoint(e->pos());
        root->handleEvent(AXR::HSSEventTypeMouseUp, (void*)&thePoint);
        root->handleEvent(AXR::HSSEventTypeClick, (void*)&thePoint);
        if (d->document->needsDisplay())
        {
            this->update();
        }
    }
}

void QAXRWidget::resizeEvent(QResizeEvent *)
{
    if (!d->document)
        return;
    d->document->setNeedsDisplay(true);
}
