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
#include "HSSContainer.h"
#include "HSSMouseEvent.h"
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
    setFocusPolicy(Qt::StrongFocus);
}

QAXRWidget::QAXRWidget(QWidget *parent)
: QWidget(parent), d(new Private)
{
    this->setMouseTracking(true);
    d->renderVisitor = new HSSRenderer();
    setFocusPolicy(Qt::StrongFocus);
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

    d->renderVisitor->setDocument(document);

    if ((d->document = document) && d->document->visitorManager())
    {
        d->document->visitorManager()->addVisitor(d->renderVisitor);
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

void QAXRWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(QFont("Helvetica Light", 36));

    if (!d->document || !d->document->isFileLoaded())
    {
        painter.drawText(0, 0, width(), height(), Qt::AlignCenter, "No document loaded");
        return;
    }

    QRect paintRect = rect();

    // Render the AXR document
    QImage image;
    if(d->document->needsDisplay())
    {
        d->document->setWindowSize(this->width(), this->height());

        // Fill the view with our background color...
        painter.fillRect(paintRect, d->backgroundFillColor);

        // Render the AXR document
        QSharedPointer<HSSVisitorManager> visitorManager = d->document->visitorManager();
        if (visitorManager)
        {
            // Render the final image to the screen
            d->renderVisitor->setDirtyRect(rect());
            visitorManager->runVisitors(HSSAbstractVisitor::VisitorFilterAll);

            image = d->renderVisitor->getFinalFrame();
        }
        else
        {
            axr_log(LoggerChannelRendering, "Document has no visitor manager");
        }
    } else {
        image = d->renderVisitor->getFinalFrame();
    }

    if (!image.isNull() && !image.size().isEmpty())
    {
        painter.drawImage(paintRect.topLeft(), image);
    }
    else
    {
        painter.drawText(0, 0, width(), height(), Qt::AlignCenter, "Internal rendering error");
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
