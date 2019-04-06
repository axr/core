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
#ifdef AXR_PLATFORM_MAC

#import <AppKit/AppKit.h>

#include "axr.h"
#include "AXRPlatformDriverMac.h"

using namespace AXR;

namespace AXR
{
    class AXRPlatformDriverMacPrivate
    {
        friend class AXRPlatformDriverMac;
        
        AXRPlatformDriverMacPrivate()
        {
        }
        CGContextRef gc;
    };
}

AXRPlatformDriverMac::AXRPlatformDriverMac()
: d(new AXRPlatformDriverMacPrivate)
{
    
}

AXRPlatformDriverMac::~AXRPlatformDriverMac()
{
    delete d;
}

inline AXRString AXRPlatformDriverMac__nsstringToString(NSString * input)
{
    return HSSString([input UTF8String]);
}

inline NSString * AXRPlatformDriverMac__stringToNSString(const HSSString & input)
{
    return [[[NSString alloc] initWithUTF8String:input.data().c_str()] autorelease];
}

inline NSColor * AXRPlatformDriverMac__colorToNSColor(QSharedPointer<HSSRgb> color)
{
    HSSUnit red = color->getRed() / 255;
    HSSUnit green = color->getGreen() / 255;
    HSSUnit blue = color->getBlue() / 255;
    HSSUnit alpha = color->getAlpha() / 255;
    NSColor * ret = [NSColor colorWithRed:red green:green blue:blue alpha:alpha];
    return ret;
}

inline NSPoint AXRPlatformDriverMac__pointToNSPoint(HSSPoint input)
{
    NSPoint ret;
    ret.x = input.x;
    ret.y = input.y;
    return ret;
}

inline NSRect AXRPlatformDriverMac__rectToNSRect(HSSRect input)
{
    NSRect ret;
    ret.origin.x = input.origin.x;
    ret.origin.y = input.origin.y;
    ret.size.width = input.size.width;
    ret.size.height = input.size.height;
    return ret;
}


inline NSBezierPath * AXRPlatformDriverMac__pathToNSPath(QSharedPointer<HSSPath> path)
{
    NSBezierPath * ret = [NSBezierPath bezierPath];
    const std::vector<QSharedPointer<HSSPathCommand> > & cmds = path->getCommands();
    std::vector<QSharedPointer<HSSPathCommand> >::const_iterator it;
    for (it = cmds.begin(); it != cmds.end(); ++it) {
        const QSharedPointer<HSSPathCommand> & cmd = *it;
        switch (cmd->getCommandType())
        {
            case HSSPathCommandTypeMoveTo:
            {
                QSharedPointer<HSSPathMoveTo> cmdMoveTo = qSharedPointerCast<HSSPathMoveTo>(cmd);
                [ret moveToPoint: NSMakePoint(cmdMoveTo->getX(), cmdMoveTo->getY())];
                break;
            }
                
            case HSSPathCommandTypeLineTo:
            {
                QSharedPointer<HSSPathLineTo> cmdLineTo = qSharedPointerCast<HSSPathLineTo>(cmd);
                [ret lineToPoint:NSMakePoint(cmdLineTo->getX(), cmdLineTo->getY())];
                break;
            }
                
            case HSSPathCommandTypeArcTo:
            {
                QSharedPointer<HSSPathArcTo> cmdArcTo = qSharedPointerCast<HSSPathArcTo>(cmd);
                NSPoint centerPoint = NSMakePoint(cmdArcTo->getX()+(cmdArcTo->getWidth()/2), cmdArcTo->getY()+(cmdArcTo->getHeight()/2));
                HSSUnit startAngle = 360 - cmdArcTo->getAngle();
                if (startAngle <= 360) startAngle += 360;
                if (startAngle >= 360) startAngle -= 360;
                HSSUnit endAngle = startAngle - cmdArcTo->getSweepLength();
                if (endAngle <= 360) endAngle += 360;
                if (endAngle >= 360) endAngle -= 360;
                
                [ret appendBezierPathWithArcWithCenter:centerPoint radius:cmdArcTo->getWidth()/2 startAngle:startAngle endAngle:endAngle clockwise:YES];
                break;
            }
                
            case HSSPathCommandTypeSubtract:
            {
                QSharedPointer<HSSPathSubtract> cmdSubtract = qSharedPointerCast<HSSPathSubtract>(cmd);
                [ret appendBezierPath:AXRPlatformDriverMac__pathToNSPath(cmdSubtract->getOtherPath())];
                break;
            }
                
            case HSSPathCommandTypeCloseSubpath:
            {
                QSharedPointer<HSSPathCloseSubpath> cmdCloseSubpath = qSharedPointerCast<HSSPathCloseSubpath>(cmd);
                [ret closePath];
                break;
            }
            case HSSPathCommandTypeAddEllipse:
            {
                QSharedPointer<HSSPathAddEllipse> cmdAddEllipse = qSharedPointerCast<HSSPathAddEllipse>(cmd);
                [ret appendBezierPathWithOvalInRect:AXRPlatformDriverMac__rectToNSRect(cmdAddEllipse->getRect())];
                break;
            }
            case HSSPathCommandTypeAddPolygon:
            {
                QSharedPointer<HSSPathAddPolygon> cmdAddPolygon = qSharedPointerCast<HSSPathAddPolygon>(cmd);
                const std::vector<HSSPoint> & points = cmdAddPolygon->getPoints();
                std::vector<HSSPoint>::const_iterator it2;
                bool isFirst = true;
                for (it2 = points.begin(); it2 != points.end(); ++it2)
                {
                    if (isFirst)
                    {
                        isFirst = false;
                        [ret moveToPoint:AXRPlatformDriverMac__pointToNSPoint(*it2)];
                    }
                    else
                    {
                        [ret lineToPoint:AXRPlatformDriverMac__pointToNSPoint(*it2)];
                    }
                }
                break;
            }
                
            default:
                break;
        }
    }
    [ret setWindingRule:NSEvenOddWindingRule];
    return ret;
}

inline NSGradient * AXRPlatformDriverMac__lgradientToNSlGradient(QSharedPointer<HSSRenderGradient> gradient)
{
    const std::vector<QSharedPointer<HSSRenderColorStop> > & stops = gradient->getColorStops();
    NSMutableArray * colorArray = [NSMutableArray arrayWithCapacity:stops.size()];
    std::vector<QSharedPointer<HSSRenderColorStop> >::const_iterator it;
    CGFloat locations[stops.size()];
    size_t i = 0;
    for (it = stops.begin(); it != stops.end(); ++it)
    {
        const QSharedPointer<HSSRenderColorStop> & colorStop = *it;
        [colorArray addObject:AXRPlatformDriverMac__colorToNSColor(colorStop->color())];
        locations[i] = colorStop->position();
        ++i;
    }
    
    NSGradient * ret = [[[NSGradient alloc] initWithColors:colorArray atLocations:locations colorSpace:[NSColorSpace deviceRGBColorSpace]] autorelease];
    return ret;
}

//
//inline NSFont * AXRPlatformDriverMac__fontToNSFont(QSharedPointer<HSSFont> theFont)
//{
//    
//}
//
//inline int AXRPlatformDriverMac_weightToNSWeight(AXRString keyword)
//{
//    
//}


AXRString AXRPlatformDriverMac::resourcesPath() const
{
    NSString * resourcesPath = [[NSBundle mainBundle] resourcePath];
    return AXRPlatformDriverMac__nsstringToString(resourcesPath);
}

AXRString AXRPlatformDriverMac::getDirectory(QSharedPointer<AXRBuffer> file) const
{
    NSURL * url = [NSURL URLWithString:AXRPlatformDriverMac__stringToNSString(file->sourceUrl())];
    NSURL * directoryUrl = [url URLByDeletingLastPathComponent];
    return AXRPlatformDriverMac__nsstringToString([directoryUrl absoluteString]);
}

bool AXRPlatformDriverMac::urlIsValid(const HSSString & url) const
{
    NSURL * testurl = [NSURL URLWithString:AXRPlatformDriverMac__stringToNSString(url)];
    //if all this is found we suppose it's good enough
    return testurl && [testurl scheme] && [testurl host];
}

void AXRPlatformDriverMac::readFile(const HSSString & filePath, HSSString * buffer, HSSString * sourceUrl, bool * isValid)
{
    NSURL * url = [NSURL URLWithString:AXRPlatformDriverMac__stringToNSString(filePath)];
    NSError * error;
    NSString * loadedString = [[NSString alloc] initWithContentsOfURL:url encoding:NSUTF8StringEncoding error:&error];
    if (loadedString == nil)
    {
        axr_log(LoggerChannelIO, HSSString::format("AXRBuffer: Could not open file '%s' - %s", filePath.chardata(), [[error localizedFailureReason] UTF8String]));
        *isValid = false;
        return;
    }
    
    *buffer = AXRPlatformDriverMac__nsstringToString(loadedString);
    *sourceUrl = filePath;
    *isValid = true;
    
    [loadedString release];
}

bool AXRPlatformDriverMac::fileExists(const HSSString & filePath)
{
    return [[NSFileManager defaultManager] fileExistsAtPath:AXRPlatformDriverMac__stringToNSString(filePath)];
}

void AXRPlatformDriverMac::initializeRender()
{
    [[NSColor whiteColor] set];
    NSRect bounds;
    bounds.size.width = this->getDocument()->windowWidth();
    bounds.size.height = this->getDocument()->windowHeight();
    bounds.origin.x = 0;
    bounds.origin.y = 0;
    NSRectFill(bounds);
}

void AXRPlatformDriverMac::regeneratePainter(HSSUnit width, HSSUnit height)
{
    
}

void AXRPlatformDriverMac::fillPath(QSharedPointer<HSSPath> path, QSharedPointer<HSSRgb> color)
{
    NSBezierPath * nspath = AXRPlatformDriverMac__pathToNSPath(path);
    NSColor * nscolor = AXRPlatformDriverMac__colorToNSColor(color);
    [nscolor set];
    [nspath fill];
}

void AXRPlatformDriverMac::fillPathGradient(QSharedPointer<HSSPath> path, QSharedPointer<HSSRenderGradient> gradient)
{
    NSBezierPath * nspath = AXRPlatformDriverMac__pathToNSPath(path);
    NSGradient* nsgradient = AXRPlatformDriverMac__lgradientToNSlGradient(gradient);
    
    [NSGraphicsContext saveGraphicsState];
    [nspath addClip];
    [nsgradient drawFromPoint:NSMakePoint(gradient->startX(), gradient->startY()) toPoint:NSMakePoint(gradient->endX(), gradient->endY()) options:NULL];
    [NSGraphicsContext restoreGraphicsState];
}

void AXRPlatformDriverMac::strokePath(QSharedPointer<HSSPath> path, QSharedPointer<HSSAbstractStroke> stroke)
{
    
}

void AXRPlatformDriverMac::renderText(AXRString text, HSSRect rect, HSSTextAlignType alignment, QSharedPointer<HSSFont> font)
{
    
}

HSSRect AXRPlatformDriverMac::getTextBounds(AXRString text, QSharedPointer<AXR::HSSFont> font, HSSUnit allowedWidth, HSSTextAlignType alignment)
{
    HSSRect ret;
    return ret;
}

void AXRPlatformDriverMac::setGraphicsContext(CGContextRef gc)
{
    d->gc = gc;
}





#endif
