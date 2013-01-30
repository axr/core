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
#include "HSSRenderTreeBuilder.h"
#include "HSSDisplayObject.h"
#include "HSSTextBlock.h"
#include "HSSContainer.h"
#include "AXRDebugging.h"
#include <QFile>
#include <QTextStream>
#include <QImage>
#include <QPainter>
#include <QXmlStreamWriter>

using namespace AXR;

HSSRenderTreeBuilder::HSSRenderTreeBuilder()
{
    _streamWriter = new QXmlStreamWriter(&_renderTreeString);
    _streamWriter->setAutoFormatting(true);
}

HSSRenderTreeBuilder::HSSRenderTreeBuilder(AXRString outputFile) : _outputFile(outputFile)
{
    _streamWriter = new QXmlStreamWriter(&_renderTreeString);
    _streamWriter->setAutoFormatting(true);
}

HSSRenderTreeBuilder::~HSSRenderTreeBuilder()
{
    delete _streamWriter;
}

void HSSRenderTreeBuilder::setOutputFile(AXRString outputFile)
{
    _outputFile = outputFile;
}
AXRString HSSRenderTreeBuilder::getOutputFile()
{
    return _outputFile;
}

QImage HSSRenderTreeBuilder::getResultAsImage(int width, int height)
{

    QImage image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    QPainter painter(&image);
    QPen pen;
    pen.setColor(QColor(0, 0, 0));
    painter.setPen(pen);

    //painter.setFont(textBlock.getFont());

    painter.drawText(QRectF(0, 0, width, height), _renderTreeString, QTextOption(Qt::AlignLeft));
    return image;
}
AXRString HSSRenderTreeBuilder::getResultAsString()
{
    return _renderTreeString;
}

void HSSRenderTreeBuilder::initializeVisit()
{
    _renderTreeString = "";
    _depthString ="";
    _depth = 0;
    _streamWriter->writeStartDocument();
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
    //open element
    _streamWriter->writeStartElement(name);

    if (displayObject.isA(HSSObjectTypeContainer))
    {
        HSSContainer* asContainer = (HSSContainer*)&displayObject;
        if (asContainer->getChildren(true)->size() > 0)
        {
            //keep element open
            _parentStack.push(&displayObject);
        }
        else
        {
            //otherwise close it
            _streamWriter->writeEndElement();
        }
    }
    else
    {
        //not a container so close the element
        _streamWriter->writeEndElement();
    }

    HSSDisplayObject* currentDisplayObject = &displayObject;
    while (!_parentStack.empty())
    {
        HSSContainer* topAsContainer = (HSSContainer*)_parentStack.top();
        HSSDisplayObject* lastChild = &*topAsContainer->getChildren(true)->back();

        if (currentDisplayObject == lastChild)
        {
            _streamWriter->writeEndElement();
            _parentStack.pop();
            currentDisplayObject = (HSSDisplayObject*)topAsContainer;
        }
        else break;
    }
    //_previousObject = &displayObject;
}


void HSSRenderTreeBuilder::finalizeVisit()
{
    _streamWriter->writeEndDocument();
    while (!_parentStack.empty())
    {
        _parentStack.pop();
    }
    if (!_outputFile.isEmpty())
    {
        QFile file(_outputFile);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << _renderTreeString;
        file.close();
    }
}


void HSSRenderTreeBuilder::reset()
{
    _renderTreeString = "";
}
