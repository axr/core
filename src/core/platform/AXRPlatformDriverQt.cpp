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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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

#include "axr_platform_config.h"
#ifdef AXR_PLATFORM_QT

#include "axr.h"
#include "AXRPlatformDriverQt.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <QUrl>

using namespace AXR;

namespace AXR
{
    class AXRPlatformDriverQtPrivate
    {
        friend class AXRPlatformDriverQt;
        
        AXRPlatformDriverQtPrivate()
        : canvas()
        , painter()
        , globalAntialiasingEnabled(true)
        {
        }
        
        QImage canvas;
        QPainter *painter;
        bool globalAntialiasingEnabled;
    };
}

AXRPlatformDriverQt::AXRPlatformDriverQt()
: d(new AXRPlatformDriverQtPrivate)
{
    
}

AXRPlatformDriverQt::~AXRPlatformDriverQt()
{
    delete d;
}

AXRString AXRPlatformDriverQt::resourcesPath() const
{
    // TODO: it's best to do this in a different way entirely... the library
    // shouldn't be using the client process's directory to determine where
    // to load resources from. For example, two different browsers will be
    // at different file paths, while the resources path will be independent
    // of either of them. Perhaps an AXR configuration file at a standard
    // location? /etc/axr.conf on Unix and %WINDIR%/axr.ini on Windows?
    QDir dir(QCoreApplication::applicationDirPath());
#ifdef Q_OS_MAC
#ifndef Q_OS_IOS
    dir.cdUp();
#endif
    dir.cd("Resources");
#elif defined(Q_OS_UNIX)
    dir.cdUp();
    dir.cd("share");
    dir.cd("axr");
#endif
    dir.cd("resources");
    return AXRPlatformDriverQt::_qstringToString(dir.canonicalPath());
}

AXRString AXRPlatformDriverQt::getDirectory(QSharedPointer<AXRBuffer> file) const
{
    QUrl filePath = AXRPlatformDriverQt::_stringToQtString(file->sourceUrl());
    QDir dir(QFileInfo(filePath.toLocalFile()).canonicalPath());
    return this->_qstringToString(dir.path());
}

AXRString AXRPlatformDriverQt::_qstringToString(QString input)
{
    return HSSString(input.toStdString());
}

QString AXRPlatformDriverQt::_stringToQtString(const HSSString & input)
{
    return QString(input.data().c_str());
}

bool AXRPlatformDriverQt::urlIsValid(const HSSString & url) const
{
    return QUrl(AXRPlatformDriverQt::_stringToQtString(url)).isValid();
}

void AXRPlatformDriverQt::readFile(const HSSString & filePath, HSSString * buffer, HSSString * sourceUrl, bool * isValid)
{
    QFileInfo fileInfo = QFileInfo(AXRPlatformDriverQt::_stringToQtString(filePath));
    QFile file(fileInfo.canonicalFilePath());
    if (file.open(QIODevice::ReadOnly))
    {
        *buffer = AXRPlatformDriverQt::_qstringToString(file.readAll());
        *sourceUrl = AXRPlatformDriverQt::_qstringToString(QUrl::fromLocalFile(fileInfo.canonicalFilePath()).toString());
        *isValid = buffer->length() == file.size();
    }
    else
    {
        axr_log(LoggerChannelIO, HSSString::format("AXRBuffer: Could not open file '%s' - %s", AXRPlatformDriverQt::_qstringToString(fileInfo.canonicalFilePath()).chardata(), AXRPlatformDriverQt::_qstringToString(file.errorString()).chardata()));
    }
}

bool AXRPlatformDriverQt::fileExists(const HSSString & filePath)
{
    QFileInfo fileInfo = QFileInfo(AXRPlatformDriverQt::_stringToQtString(filePath));
    return fileInfo.exists();
}

void AXRPlatformDriverQt::initializeRender()
{
    d->canvas.fill(Qt::white);
    this->_initializeRenderHints();
}

void AXRPlatformDriverQt::regeneratePainter(HSSUnit width, HSSUnit height)
{
    if (d->painter)
        delete d->painter;
    
    d->canvas = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    d->painter = new QPainter(&d->canvas);
}

void AXRPlatformDriverQt::fillPath(QSharedPointer<HSSPath> path, QSharedPointer<HSSRgb> color)
{
    QPainterPath fillPath = AXRPlatformDriverQt::_pathToQtPath(path);
    QColor fillColor = AXRPlatformDriverQt::_colorToQtColor(color);
    d->painter->fillPath(fillPath, fillColor);
}

void AXRPlatformDriverQt::fillPathGradient(QSharedPointer<HSSPath> path, QSharedPointer<HSSRenderGradient> gradient)
{
    QLinearGradient pat(gradient->startX(), gradient->startY(), gradient->endX(), gradient->endY());
    Q_FOREACH(QSharedPointer<HSSRenderColorStop> colorStop, gradient->getColorStops())
    {
        pat.setColorAt(colorStop->position(), AXRPlatformDriverQt::_colorToQtColor(colorStop->color()));
    }
    QBrush brush(pat);
    d->painter->fillPath(AXRPlatformDriverQt::_pathToQtPath(path), brush);
}

void AXRPlatformDriverQt::strokePath(QSharedPointer<HSSPath> path, QSharedPointer<HSSAbstractStroke> stroke)
{
    QPainterPathStroker stroker;
    stroker.setWidth(stroke->getSize());
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setCapStyle(Qt::FlatCap);
    QPainterPath strokePath = stroker.createStroke(this->_pathToQtPath(path));
    
    QSharedPointer<HSSObject> colorObj;
    if (stroke->isA(HSSStrokeTypeLine))
    {
        QSharedPointer<HSSStroke> lineStroke = qSharedPointerCast<HSSStroke>(stroke);
        colorObj = lineStroke->getColor();
    }
    
    QColor usedColor;
    if (colorObj && colorObj->isA(HSSObjectTypeRgb))
    {
        usedColor = AXRPlatformDriverQt::_colorToQtColor(qSharedPointerCast<HSSRgb>(colorObj));
    }
    else
    {
        usedColor = QColor(Qt::black);
    }
    d->painter->fillPath(strokePath, usedColor);
}

void AXRPlatformDriverQt::renderText(AXRString text, HSSRect rect, HSSTextAlignType alignment, QSharedPointer<HSSFont> font)
{
    this->_setTextRenderHints();
    
    QPen pen;
    if (font->getColor())
    {
        QSharedPointer<HSSRgb> textColor = qSharedPointerCast<HSSRgb>(font->getColor());
        pen.setColor(this->_colorToQtColor(textColor));
    }
    else
    {
        pen.setColor(QColor(0, 0, 0));
    }
    
    d->painter->setPen(pen);
    
    d->painter->setFont(AXRPlatformDriverQt::_fontToQtFont(font));
    Qt::Alignment flags = Qt::AlignLeft;
    switch (alignment)
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
    
    d->painter->drawText(QRectF(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height), AXRPlatformDriverQt::_stringToQtString(text), QTextOption(flags));
    
    this->_resetTextRenderHints();

}

HSSRect AXRPlatformDriverQt::getTextBounds(AXRString text, QSharedPointer<AXR::HSSFont> font, HSSUnit allowedWidth, HSSTextAlignType alignment)
{
    int flags = Qt::AlignLeft;
    switch (alignment)
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
    
    
    QFontMetrics fontMetrics(AXRPlatformDriverQt::_fontToQtFont(font));
    
    QRect bounds = fontMetrics.boundingRect(0, 0, static_cast<int>(allowedWidth), std::numeric_limits<int>::max(), flags | Qt::TextWordWrap, AXRPlatformDriverQt::_stringToQtString(text));
    
    HSSRect ret;
    ret.size.width = bounds.width();
    ret.size.height = bounds.height();
    ret.origin.x = bounds.x();
    ret.origin.y = bounds.y();
    return ret;
}

QImage AXRPlatformDriverQt::getFinalFrame() const
{
    return d->canvas;
}

QRect AXRPlatformDriverQt::_rectToQtRect(HSSRect rect)
{
    QRect ret;
    ret.setWidth(rect.size.width);
    ret.setHeight(rect.size.height);
    ret.setX(rect.origin.x);
    ret.setY(rect.origin.y);
    return ret;
}

QPointF AXRPlatformDriverQt::_pointToQtPoint(HSSPoint point)
{
    return QPointF(point.x, point.y);
}

QFont AXRPlatformDriverQt::_fontToQtFont(QSharedPointer<HSSFont> theFont)
{
    QFont font_description;
    
    // Get the first font available
    if (theFont && !theFont->getFace().isEmpty())
        font_description.setFamily(AXRPlatformDriverQt::_stringToQtString(theFont->getFace()));
    else
        font_description.setFamily("monospace");
    
    // Set the weight of the font (bold, italic, etc.) if available
    if (theFont && theFont->getWeight() != "")
        font_description.setWeight(AXRPlatformDriverQt::_weightToQtWeight(theFont->getWeight()));
    else
        font_description.setWeight(QFont::Normal);
    
    font_description.setPointSize(theFont ? theFont->getSize() : HSSFont::DEFAULT_SIZE);
    
    return font_description;
}

QPainterPath AXRPlatformDriverQt::_pathToQtPath(QSharedPointer<HSSPath> path)
{
    QPainterPath ret;
    const std::vector<QSharedPointer<HSSPathCommand> > & cmds = path->getCommands();
    std::vector<QSharedPointer<HSSPathCommand> >::const_iterator it;
    for (it = cmds.begin(); it != cmds.end(); ++it) {
        const QSharedPointer<HSSPathCommand> & cmd = *it;
        switch (cmd->getCommandType())
        {
            case HSSPathCommandTypeMoveTo:
            {
                QSharedPointer<HSSPathMoveTo> cmdMoveTo = qSharedPointerCast<HSSPathMoveTo>(cmd);
                ret.moveTo(cmdMoveTo->getX(), cmdMoveTo->getY());
                break;
            }
                
            case HSSPathCommandTypeLineTo:
            {
                QSharedPointer<HSSPathLineTo> cmdLineTo = qSharedPointerCast<HSSPathLineTo>(cmd);
                ret.lineTo(cmdLineTo->getX(), cmdLineTo->getY());
                break;
            }
                
            case HSSPathCommandTypeArcTo:
            {
                QSharedPointer<HSSPathArcTo> cmdArcTo = qSharedPointerCast<HSSPathArcTo>(cmd);
                ret.arcTo(cmdArcTo->getX(), cmdArcTo->getY(), cmdArcTo->getWidth(), cmdArcTo->getHeight(), cmdArcTo->getAngle(), cmdArcTo->getSweepLength());
                break;
            }
                
            case HSSPathCommandTypeSubtract:
            {
                QSharedPointer<HSSPathSubtract> cmdSubtract = qSharedPointerCast<HSSPathSubtract>(cmd);
                ret = ret.subtracted(AXRPlatformDriverQt::_pathToQtPath(cmdSubtract->getOtherPath()));
                break;
            }
                
            case HSSPathCommandTypeCloseSubpath:
            {
                QSharedPointer<HSSPathCloseSubpath> cmdCloseSubpath = qSharedPointerCast<HSSPathCloseSubpath>(cmd);
                ret.closeSubpath();
                break;
            }
            case HSSPathCommandTypeAddEllipse:
            {
                QSharedPointer<HSSPathAddEllipse> cmdAddEllipse = qSharedPointerCast<HSSPathAddEllipse>(cmd);
                ret.addEllipse(AXRPlatformDriverQt::_rectToQtRect(cmdAddEllipse->getRect()));
                break;
            }
            case HSSPathCommandTypeAddPolygon:
            {
                QSharedPointer<HSSPathAddPolygon> cmdAddPolygon = qSharedPointerCast<HSSPathAddPolygon>(cmd);
                const std::vector<HSSPoint> & points = cmdAddPolygon->getPoints();
                std::vector<HSSPoint>::const_iterator it2;
                QVector<QPointF> qpoints;
                for (it2 = points.begin(); it2 != points.end(); ++it2)
                {
                    qpoints << AXRPlatformDriverQt::_pointToQtPoint(*it2);
                }
                ret.addPolygon(QPolygonF(qpoints));
                break;
            }
                
            default:
                break;
        }
    }
    return ret;
}

QColor AXRPlatformDriverQt::_colorToQtColor(QSharedPointer<HSSRgb> color)
{
    return QColor(static_cast<int>(color->getRed()), static_cast<int>(color->getGreen()), static_cast<int>(color->getBlue()), static_cast<int>(color->getAlpha()));
}

void AXRPlatformDriverQt::_initializeRenderHints()
{
    if (d->globalAntialiasingEnabled)
    {
        d->painter->setRenderHint(QPainter::Antialiasing);
    }
    else
    {
        d->painter->setRenderHint(QPainter::NonCosmeticDefaultPen);
    }
}

void AXRPlatformDriverQt::_setTextRenderHints()
{
    if (d->globalAntialiasingEnabled)
        d->painter->setRenderHint(QPainter::TextAntialiasing);
}
void AXRPlatformDriverQt::_resetTextRenderHints()
{
    if (d->globalAntialiasingEnabled)
        d->painter->setRenderHint(QPainter::Antialiasing);
}

int AXRPlatformDriverQt::_weightClassFromKeyword(AXRString keyword)
{
    if (keyword == "thin")
        return 100;
    if (keyword == "ultralight")
        return 200;
    if (keyword == "light")
        return 300;
    if (keyword == "regular")
        return 400;
    if (keyword == "medium")
        return 500;
    if (keyword == "semibold")
        return 600;
    if (keyword == "bold")
        return 700;
    if (keyword == "heavy")
        return 800;
    if (keyword == "black")
        return 900;
    
    return 400;
}

// From qfontconfigdatabase.cpp
int AXRPlatformDriverQt::getQtWeight(int fc_weight)
{
    int qtweight = QFont::Black;
    
    if (fc_weight <= (AXRPlatformDriverQt::_weightClassFromKeyword("light") + AXRPlatformDriverQt::_weightClassFromKeyword("medium")) / 2)
        qtweight = QFont::Light;
    else if (fc_weight <= (AXRPlatformDriverQt::_weightClassFromKeyword("medium") + AXRPlatformDriverQt::_weightClassFromKeyword("semibold")) / 2)
        qtweight = QFont::Normal;
    else if (fc_weight <= (AXRPlatformDriverQt::_weightClassFromKeyword("semibold") + AXRPlatformDriverQt::_weightClassFromKeyword("bold")) / 2)
        qtweight = QFont::DemiBold;
    else if (fc_weight <= (AXRPlatformDriverQt::_weightClassFromKeyword("bold") + AXRPlatformDriverQt::_weightClassFromKeyword("black")) / 2)
        qtweight = QFont::Bold;
    
    return qtweight;
}

int AXRPlatformDriverQt::_weightToQtWeight(AXRString keyword)
{
    return AXRPlatformDriverQt::getQtWeight(AXRPlatformDriverQt::_weightClassFromKeyword(keyword));
}

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>

CGImageRef AXRPlatformDriverQt::getFinalFrameAsCGImageRef() const
{
    QImage canvas = getFinalFrame();
    return AXRPlatformDriverQt::qt_mac_toCGImage(canvas);
}

static void qt_mac_deleteImage(void *image, const void *, size_t)
{
    delete static_cast<QImage *>(image);
}

// Creates a CGDataProvider with the data from the given image.
// The data provider retains a copy of the image.
CGDataProviderRef AXRPlatformDriverQt::qt_mac_CGDataProvider(const QImage &image)
{
    return CGDataProviderCreateWithData(new QImage(image), image.bits(),
                                        image.byteCount(), qt_mac_deleteImage);
}

CGImageRef AXRPlatformDriverQt::qt_mac_toCGImage(const QImage &inImage)
{
    if (inImage.isNull())
        return 0;
    
    QImage image = inImage;
    
    uint cgflags = kCGImageAlphaNone;
    switch (image.format()) {
        case QImage::Format_ARGB32:
            cgflags = kCGImageAlphaFirst | kCGBitmapByteOrder32Host;
            break;
        case QImage::Format_RGB32:
            cgflags = kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Host;
            break;
        case QImage::Format_RGB888:
            cgflags = kCGImageAlphaNone | kCGBitmapByteOrder32Big;
            break;
            
        default:
            // Everything not recognized explicitly is converted to ARGB32_Premultiplied.
            image = inImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            // no break;
        case QImage::Format_ARGB32_Premultiplied:
            cgflags = kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host;
            break;
    }
    
    CGDataProviderRef dataProvider = qt_mac_CGDataProvider(image);
    return CGImageCreate(image.width(), image.height(), 8, 32,
                         image.bytesPerLine(),
                         CGColorSpaceCreateDeviceRGB(),
                         cgflags, dataProvider, 0, false, kCGRenderingIntentDefault);
}


#endif // __APPLE__

#endif // platform
