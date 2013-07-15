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
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "HSSCascader.h"
#include "HSSContainer.h"
#include "HSSLayout.h"
#include "HSSMouseEvent.h"
#include "HSSRect.h"
#include "HSSRenderer.h"
#include "HSSVisitorManager.h"
#include "QAXRWidget.h"

namespace AXR
{
    class QAXRWidgetPrivate
    {
    public:
        QAXRWidgetPrivate(QAXRWidget *);
        QAXRWidget *q_ptr;
        AXRDocument *document;
        HSSCascader* cascadeVisitor;
        HSSLayout* layoutVisitor;
        HSSRenderer* renderVisitor;
        QColor backgroundFillColor;
    };
}

using namespace AXR;

QAXRWidgetPrivate::QAXRWidgetPrivate(QAXRWidget *q)
: q_ptr(q)
, document(0)
, renderVisitor(new HSSRenderer)
, layoutVisitor(new HSSLayout)
, cascadeVisitor(new HSSCascader)
, backgroundFillColor(QColor(Qt::white))
{
    q_ptr->setMouseTracking(true);
    q_ptr->setFocusPolicy(Qt::StrongFocus);
}

QAXRWidget::QAXRWidget(AXRDocument *document, QWidget *parent)
: QWidget(parent), d(new QAXRWidgetPrivate(this))
{
    setDocument(document);
}

QAXRWidget::QAXRWidget(QWidget *parent)
: QWidget(parent), d(new QAXRWidgetPrivate(this))
{
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
    if (d->document == document)
        return;

    d->cascadeVisitor->setDocument(document);
    d->layoutVisitor->setDocument(document);
    d->renderVisitor->setDocument(document);

    if ((d->document = document) && d->document->visitorManager())
    {
        QSharedPointer<HSSVisitorManager> vm = d->document->visitorManager();
        vm->addVisitor(d->cascadeVisitor);
        vm->addVisitor(d->layoutVisitor);
        vm->addVisitor(d->renderVisitor);
    }

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
    if (!d->document || !d->document->isFileLoaded())
        return;

    QPainter painter(this);
    QRect paintRect = rect();

    // Fill the view with the default fill color
    painter.fillRect(paintRect, d->backgroundFillColor);

    // Resize and composite the AXR document if necessary
    if (d->document->needsDisplay())
    {
        d->document->setWindowSize(this->width(), this->height());

        // Render the AXR document
        QSharedPointer<HSSVisitorManager> visitorManager = d->document->visitorManager();
        if (visitorManager)
        {
            //force values
            QSharedPointer<HSSDisplayObject> root = d->document->controller()->root();
            root->setComputedBool("visible", true);
            root->setComputedValue("width", d->document->windowWidth());
            root->setComputedValue("height", d->document->windowHeight());

            // Render the final image to the screen
            d->renderVisitor->setDirtyRect(e->rect());
            visitorManager->runVisitors();
        }
        else
        {
            axr_log(LoggerChannelRendering, "Document has no visitor manager");
        }
    }

    // Present the final composited surface to the screen
    const QImage &image = d->renderVisitor->getFinalFrame();
    if (!image.isNull() && !image.size().isEmpty())
    {
        painter.drawImage(paintRect.topLeft(), image);
    }
    else
    {
        axr_log(LoggerChannelRendering, "Internal rendering error - display buffer null or size zero");
    }

    d->document->setNeedsDisplay(false);
}

void QAXRWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    HSSMouseEvent mouseEvent(HSSEventTypeDoubleClick, e->pos());
    d->document->handleEvent(&mouseEvent);
    if (d->document->needsDisplay())
    {
        this->update();
    }
}

void QAXRWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    HSSMouseEvent mouseEvent(HSSEventTypeMouseMove, e->pos());
    d->document->handleEvent(&mouseEvent);
    if (d->document->needsDisplay())
    {
        this->update();
    }
}

void QAXRWidget::mousePressEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    HSSMouseEvent mouseEvent(HSSEventTypeMouseDown, e->pos());
    d->document->handleEvent(&mouseEvent);
    if (d->document->needsDisplay())
    {
        this->update();
    }
}

void QAXRWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (!d->document)
        return;

    HSSMouseEvent upMouseEvent(HSSEventTypeMouseUp, e->pos());
    HSSMouseEvent clickMouseEvent(HSSEventTypeClick, e->pos());
    d->document->handleEvent(&upMouseEvent);
    d->document->handleEvent(&clickMouseEvent);
    if (d->document->needsDisplay())
    {
        this->update();
    }
}

void QAXRWidget::resizeEvent(QResizeEvent *)
{
    if (!d->document)
        return;

    d->document->setNeedsDisplay(true);
    update();
}
