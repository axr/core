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

#include "HSSContainer.h"
#include "HSSRenderTreeBuilder.h"
#include "HSSTextBlock.h"
#include <stack>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QTextStream>
#include <QXmlStreamWriter>

using namespace AXR;

class HSSRenderTreeBuilder::Private
{
public:
    Private() : outputFile(), previousObject(), parentStack(), renderTreeString(), streamWriter(), depthString(), depth()
    {
        streamWriter = new QXmlStreamWriter(&renderTreeString);
        streamWriter->setAutoFormatting(true);
    }

    ~Private()
    {
        delete streamWriter;
    }

    AXRString outputFile;
    HSSDisplayObject *previousObject;
    std::stack<HSSDisplayObject*> parentStack;
    AXRString renderTreeString;
    QXmlStreamWriter* streamWriter;
    AXRString depthString;
    int depth;
};

HSSRenderTreeBuilder::HSSRenderTreeBuilder()
: d(new Private)
{
}

HSSRenderTreeBuilder::HSSRenderTreeBuilder(const AXRString &outputFile)
: d(new Private)
{
    d->outputFile = outputFile;
}

HSSRenderTreeBuilder::~HSSRenderTreeBuilder()
{
    delete d;
}

AXRString HSSRenderTreeBuilder::getOutputFile()
{
    return d->outputFile;
}

void HSSRenderTreeBuilder::setOutputFile(AXRString outputFile)
{
    d->outputFile = outputFile;
}

QImage HSSRenderTreeBuilder::getResultAsImage(int width, int height)
{
    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    QPainter painter(&image);
    QPen pen;
    pen.setColor(QColor(0, 0, 0));
    painter.setPen(pen);

    //painter.setFont(textBlock.getFont());

    painter.drawText(QRectF(0, 0, width, height), d->renderTreeString, QTextOption(Qt::AlignLeft));
    return image;
}

AXRString HSSRenderTreeBuilder::getResultAsString()
{
    return d->renderTreeString;
}

void HSSRenderTreeBuilder::initializeVisit()
{
    d->renderTreeString = AXRString();
    d->depthString = AXRString();
    d->depth = 0;
    d->streamWriter->writeStartDocument();
}

void HSSRenderTreeBuilder::visit(HSSContainer &container)
{
    addTextForObject(container);
}

void HSSRenderTreeBuilder::visit(HSSTextBlock &textBlock)
{
    addTextForObject(textBlock);
}

void HSSRenderTreeBuilder::addTextForObject(HSSDisplayObject &displayObject)
{
    AXRString name = displayObject.getElementName();
    if (name.isEmpty())
    {
        if (displayObject.isA(HSSObjectTypeContainer))
            name = "container";
        else if (displayObject.isA(HSSObjectTypeTextBlock))
            name = "textblock";
    }

    // open element
    d->streamWriter->writeStartElement(name);

    if (displayObject.isA(HSSObjectTypeContainer))
    {
        HSSContainer* asContainer = (HSSContainer*)&displayObject;
        if (asContainer->getChildren(true)->size() > 0)
        {
            // keep element open
            d->parentStack.push(&displayObject);
        }
        else
        {
            // otherwise close it
            d->streamWriter->writeEndElement();
        }
    }
    else
    {
        // not a container so close the element
        d->streamWriter->writeEndElement();
    }

    HSSDisplayObject* currentDisplayObject = &displayObject;
    while (!d->parentStack.empty())
    {
        HSSContainer* topAsContainer = (HSSContainer*)d->parentStack.top();
        HSSDisplayObject* lastChild = &*topAsContainer->getChildren(true)->back();

        if (currentDisplayObject == lastChild)
        {
            d->streamWriter->writeEndElement();
            d->parentStack.pop();
            currentDisplayObject = (HSSDisplayObject*)topAsContainer;
        }
        else
            break;
    }

    //_previousObject = &displayObject;
}

void HSSRenderTreeBuilder::finalizeVisit()
{
    d->streamWriter->writeEndDocument();
    while (!d->parentStack.empty())
    {
        d->parentStack.pop();
    }

    if (!d->outputFile.isEmpty())
    {
        QFile file(d->outputFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << d->renderTreeString;
        }
    }
}

void HSSRenderTreeBuilder::reset()
{
    d->renderTreeString = AXRString();
}
