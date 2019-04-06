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

#ifndef HSSPLATFORMDRIVERMAC_H
#define HSSPLATFORMDRIVERMAC_H

#import "AXRPlatformDriver.h"

namespace AXR
{
    class AXRPlatformDriverMacPrivate;
    class AXRPlatformDriverMac : public AXRPlatformDriver
    {
    public:
        AXRPlatformDriverMac();
        virtual ~AXRPlatformDriverMac();
        
        virtual AXRString resourcesPath() const;
        virtual AXRString getDirectory(QSharedPointer<AXRBuffer> file) const;
        bool urlIsValid(const HSSString & url) const;
        void readFile(const HSSString & filePath, HSSString * buffer, HSSString * sourceUrl, bool * isValid);
        virtual bool fileExists(const HSSString & filePath);
        virtual void initializeRender();
        virtual void regeneratePainter(HSSUnit width, HSSUnit height);
        virtual void fillPath(QSharedPointer<HSSPath> path, QSharedPointer<HSSRgb> color);
        virtual void fillPathGradient(QSharedPointer<HSSPath> path, QSharedPointer<HSSRenderGradient> gradient);
        virtual void strokePath(QSharedPointer<HSSPath> path, QSharedPointer<HSSAbstractStroke> stroke);
        virtual void renderText(AXRString text, HSSRect rect, HSSTextAlignType alignment, QSharedPointer<HSSFont> font);
        virtual HSSRect getTextBounds(AXRString text, QSharedPointer<HSSFont> font, HSSUnit allowedWidth, HSSTextAlignType alignment);
        
        void setGraphicsContext(CGContextRef gc);
        
    private:
        AXRPlatformDriverMacPrivate * d;
    };
}

#endif /* HSSPLATFORMDRIVERMAC_H */
