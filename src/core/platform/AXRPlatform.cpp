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

#include "axr.h"
#include "AXRPlatform.h"

//which platform to use is defined in this file:
#include "axr_platform_config.h"
#ifdef AXR_PLATFORM_MAC
#include "AXRPlatformDriverMac.h"
#endif
#ifdef AXR_PLATFORM_QT
#include "AXRPlatformDriverQt.h"
#endif

using namespace AXR;

namespace AXR
{
    class AXRPlatformPrivate
    {
        friend class AXRPlatform;
        
        AXRPlatformPrivate()
        : driver(NULL)
        {
        }

        AXRPlatformDriver * driver;
    };
}

AXRPlatform::AXRPlatform()
: d(new AXRPlatformPrivate)
{
#ifdef AXR_PLATFORM_MAC
    d->driver = new AXRPlatformDriverMac();
#endif
#ifdef AXR_PLATFORM_QT
    d->driver = new AXRPlatformDriverQt();
#endif
}

AXRPlatform::~AXRPlatform()
{
    delete d->driver;
    delete d;
}

void AXRPlatform::setDriver(AXRPlatformDriver * driver)
{
    d->driver = driver;
}

AXRPlatformDriver * AXRPlatform::getDriver() const
{
    return d->driver;
}

void AXRPlatform::setDocument(AXRDocument * document)
{
    d->driver->setDocument(document);
}

AXRDocument * AXRPlatform::getDocument() const
{
    return d->driver->getDocument();
}

AXRString AXRPlatform::resourcesPath() const
{
    return d->driver->resourcesPath();
}

AXRString AXRPlatform::getDirectory(QSharedPointer<AXRBuffer> file) const
{
    return d->driver->getDirectory(file);
}

bool AXRPlatform::urlIsValid(const AXRString & url) const
{
    return d->driver->urlIsValid(url);
}

void AXRPlatform::readFile(const HSSString & filePath, HSSString * buffer, HSSString * sourceUrl, bool * isValid)
{
    return d->driver->readFile(filePath, buffer, sourceUrl, isValid);
}

bool AXRPlatform::fileExists(const HSSString & filePath)
{
    return d->driver->fileExists(filePath);
}

void AXRPlatform::initializeRender()
{
    d->driver->initializeRender();
}

void AXRPlatform::regeneratePainter(HSSUnit width, HSSUnit height)
{
    d->driver->regeneratePainter(width, height);
}

void AXRPlatform::fillPath(QSharedPointer<HSSPath> path, QSharedPointer<HSSRgb> color)
{
    d->driver->fillPath(path, color);
}

void AXRPlatform::fillPathGradient(QSharedPointer<HSSPath> path, QSharedPointer<HSSRenderGradient> gradient)
{
    d->driver->fillPathGradient(path, gradient);
}

void AXRPlatform::drawStroke(QSharedPointer<HSSPath> path, QSharedPointer<HSSAbstractStroke> stroke)
{
    d->driver->strokePath(path, stroke);
}

void AXRPlatform::renderText(AXRString text, HSSRect rect, HSSTextAlignType alignment, QSharedPointer<HSSFont> font)
{
    d->driver->renderText(text, rect, alignment, font);
}

HSSRect AXRPlatform::getTextBounds(AXRString text, QSharedPointer<AXR::HSSFont> font, HSSUnit allowedWidth, HSSTextAlignType alignment)
{
    return d->driver->getTextBounds(text, font, allowedWidth, alignment);
}
