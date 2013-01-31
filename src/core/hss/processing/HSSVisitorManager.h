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

#ifndef HSSVISITORMANAGER_H
#define HSSVISITORMANAGER_H

#include "AXRGlobal.h"
#include "HSSAbstractVisitor.h"

namespace AXR
{
    class AXRController;
    class AXRDocument;

    /**
     *  @brief This is the main control hub for rendering. Calls for drawing and handling
     *  errors should go through this class.
     */
    class AXR_API HSSVisitorManager
    {
    public:
        /**
         *  Creates a new instance of the render object.
         *  @param  controller  A regular pointer to the controller object that is associated with this renderer.
         */
        HSSVisitorManager(AXRController * controller);

        /**
         *  Destructs the render object.
         */
        virtual ~HSSVisitorManager();

        void addVisitor(HSSAbstractVisitor *visitor);

        void runVisitors();
        void runVisitors(HSSAbstractVisitor::VisitorFilterFlags filterFlags);
        void runVisitors(QSharedPointer<HSSDisplayObject> root, HSSAbstractVisitor::VisitorFilterFlags filterFlags, bool traverse);

        /**
         *  The main drawing function. Call this to redraw an area of the window.
         *  @param  rect    The rectangular section of the screen that should be redrawn
         *  @param  bounds  The rectangle describing the bounds of the window.
         *
         *  @warning The root surface should be created in platform specific subclass before calling
         *  this as base class' method
         */
        /**
         *  Reset the renderer to initial values, for example when reloading a file.
         */
        void reset();

    protected:
        void resetVisitors();

    private:
        class Private;
        Private *d;
    };
}


#endif //HSSVISITORMANAGER_H
