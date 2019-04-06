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

#ifndef HSSPLATFORMDRIVER_H
#define HSSPLATFORMDRIVER_H

#include "HSSTextEnums.h"
#ifdef __APPLE__
typedef struct CGImage *CGImageRef;
#import <CoreGraphics/CoreGraphics.h>
#endif

namespace AXR
{
    class AXRPlatformDriverPrivate;
    class HSSAbstractStroke;
    class HSSFont;
    class HSSPath;
    class HSSRgb;
    class HSSRenderGradient;

    class AXRPlatformDriver
    {
    public:
        AXRPlatformDriver();
        virtual ~AXRPlatformDriver();

        void setDocument(AXRDocument * document);
        AXRDocument * getDocument() const;
        virtual AXRString resourcesPath() const = 0;
        virtual AXRString getDirectory(QSharedPointer<AXRBuffer> file) const = 0;
        virtual bool urlIsValid(const AXRString & url) const = 0;
        virtual void readFile(const HSSString & filePath, HSSString * buffer, HSSString * sourceUrl, bool * isValid) = 0;
        virtual bool fileExists(const HSSString & filePath) = 0;
        
        virtual void initializeRender() = 0;
        virtual void regeneratePainter(HSSUnit width, HSSUnit height) = 0;
        virtual void fillPath(QSharedPointer<HSSPath> path, QSharedPointer<HSSRgb> color) = 0;
        virtual void fillPathGradient(QSharedPointer<HSSPath> path, QSharedPointer<HSSRenderGradient> gradient) = 0;
        virtual void strokePath(QSharedPointer<HSSPath> path, QSharedPointer<HSSAbstractStroke> stroke) = 0;
        virtual void renderText(AXRString text, HSSRect rect, HSSTextAlignType alignment, QSharedPointer<HSSFont> font) = 0;
        virtual HSSRect getTextBounds(AXRString text, QSharedPointer<HSSFont> font, HSSUnit allowedWidth, HSSTextAlignType alignment) = 0;
        
    private:
        AXRPlatformDriverPrivate * d;
    };
}

#endif /* HSSPLATFORMDRIVER_H */
