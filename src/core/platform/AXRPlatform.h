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

#ifndef HSSPLATFORM_H
#define HSSPLATFORM_H

#include "AXRPlatformDriver.h"
#include "HSSTextEnums.h"

namespace AXR
{
    class AXRPlatformDriver;
    class AXRPlatformPrivate;
    class HSSAbstractStroke;
    class HSSFont;
    class HSSPath;
    class HSSRgb;
    class HSSRenderGradient;

    class AXRPlatform
    {
    public:
        AXRPlatform();
        virtual ~AXRPlatform();
        
        void setDriver(AXRPlatformDriver * driver);
        AXRPlatformDriver * getDriver() const;
        void setDocument(AXRDocument * document);
        AXRDocument * getDocument() const;
        AXRString resourcesPath() const;
        AXRString getDirectory(QSharedPointer<AXRBuffer> file) const;
        bool urlIsValid(const AXRString & url) const;
        void readFile(const HSSString & filePath, HSSString * buffer, HSSString * sourceUrl, bool * isValid);
        bool fileExists(const HSSString & filePath);

        void initializeRender();
        void regeneratePainter(HSSUnit width, HSSUnit height);
        void fillPath(QSharedPointer<HSSPath> path, QSharedPointer<HSSRgb> color);
        void fillPathGradient(QSharedPointer<HSSPath> path, QSharedPointer<HSSRenderGradient> gradient);
        void drawStroke(QSharedPointer<HSSPath> path, QSharedPointer<HSSAbstractStroke> stroke);
        void renderText(AXRString text, HSSRect rect, HSSTextAlignType alignment, QSharedPointer<HSSFont> font);
        HSSRect getTextBounds(AXRString text, QSharedPointer<HSSFont> font, HSSUnit allowedWidth, HSSTextAlignType alignment);
        
    private:
        AXRPlatformPrivate * d;
    };
}

#endif /* HSSPLATFORM_H */
