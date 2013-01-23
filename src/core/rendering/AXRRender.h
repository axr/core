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

#ifndef AXRRENDER_H
#define AXRRENDER_H

#include <QImage>
#include "AXRGlobal.h"

namespace AXR
{
    class AXRController;
    class AXRDocument;

    /**
     *  @brief This is the main control hub for rendering. Calls for drawing and handling
     *  errors should go through this class.
     */
    class AXR_API AXRRender
    {
    public:
        /**
         *  Creates a new instance of the render object.
         *  @param  controller  A regular pointer to the controller object that is associated with this renderer.
         */
        AXRRender(AXRController * controller);
        /**
         *  Destructs the render object.
         */
        virtual ~AXRRender();
        /**
         *  The main drawing function. Call this to redraw an area of the window.
         *  @param  rect    The rectangular section of the screen that should be redrawn
         *  @param  bounds  The rectangle describing the bounds of the window.
         *
         *  @warning The root surface should be created in platform specific subclass before calling
         *  this as base class' method
         */
        virtual void drawInRectWithBounds(HSSRect rect, HSSRect bounds);
        /**
         *  Reset the renderer to initial values, for example when reloading a file.
         */
        void reset();
        const QImage surface() const;
        /**
         *  @return The width of the window.
         */
        HSSUnit getWindowWidth();
        /**
         *  @return The height of the window.
         */
        HSSUnit getWindowHeight();

        /**
         *  @todo create getter/setter and make private
         */
        HSSUnit windowWidth;

        /**
         *  @todo create getter/setter and make private
         */
        HSSUnit windowHeight;

        bool globalAntialiasingEnabled() const;
        void setGlobalAntialiasingEnabled(bool enable);

        void regenerateRootSurface();

    protected:
        bool _globalAntialiasingEnabled;
        bool _needsSurfaces;
        AXRController *controller;
        QImage *rootSurface;
        QImage rootSurfaceFinal;
    };
}


#endif //AXRRENDER_H
