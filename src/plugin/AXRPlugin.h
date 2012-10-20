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

#ifndef H_AXRPLUGIN
#define H_AXRPLUGIN

#include <QImage>

#include "PluginCore.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginEvents/DrawingEvents.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginWindow.h"

#if FB_WIN
#include "PluginWindowWin.h"
#include "PluginWindowlessWin.h"
#elif FB_MACOSX
#include "PluginWindowMac.h"
#include "PluginWindowMacCG.h"
#include "PluginEvents/MacEventCocoa.h"
#elif FB_X11
#include "PluginWindowX11.h"
#endif

namespace AXR
{
    class AXRDocument;
}

FB_FORWARD_PTR(AXRPlugin)
class AXRPlugin : public FB::PluginCore
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    AXRPlugin();
    virtual ~AXRPlugin();

public:
    void onPluginReady();
    void shutdown();
    virtual FB::JSAPIPtr createJSAPI();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    //virtual bool isWindowless() { return false; }

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE(FB::MouseDownEvent, onMouseDown, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseUpEvent, onMouseUp, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindow)
        EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindow)
#if FB_WIN
        EVENTTYPE_CASE(FB::RefreshEvent, onDrawGDIWindowed, FB::PluginWindowWin)
        EVENTTYPE_CASE(FB::RefreshEvent, onDrawGDIWindowless, FB::PluginWindowlessWin)
#elif FB_MACOSX
        EVENTTYPE_CASE(FB::CoreGraphicsDraw, onDrawCoreGraphics, FB::PluginWindowMacCG)
#elif FB_X11
        EVENTTYPE_CASE(FB::RefreshEvent, onDrawX11, FB::PluginWindowX11)
#endif
    END_PLUGIN_EVENT_MAP()

    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    virtual bool onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *);
    virtual bool onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *);
    virtual bool onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *);
    virtual bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *);
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

    QImage composite(int width, int height);

#if FB_WIN
    virtual bool onDrawGDIWindowed(FB::RefreshEvent *evt, FB::PluginWindowWin *);
    virtual bool onDrawGDIWindowless(FB::RefreshEvent *evt, FB::PluginWindowlessWin *);
#elif FB_MACOSX
    virtual bool onDrawCoreGraphics(FB::CoreGraphicsDraw *evt, FB::PluginWindowMacCG *);
#elif FB_X11
    virtual bool onDrawX11(FB::RefreshEvent *evt, FB::PluginWindowX11 *);
#endif

private:
    AXR::AXRDocument *document;
};

#endif
