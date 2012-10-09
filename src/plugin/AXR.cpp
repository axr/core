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

#include "AXR.h"
#include "AXRAPI.h"

#ifdef FB_WIN
#include <windows.h>
#include "PluginWindowWin.h"
#include "PluginWindowlessWin.h"
#endif

#ifdef FB_MACOSX
#include <CoreGraphics/CoreGraphics.h>
#include "PluginWindowMacCG.h"
#endif

/*!
 * @brief Called from PluginFactory::globalPluginInitialize()
 *
 * @see FB::FactoryBase::globalPluginInitialize
 */
void AXR::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
}

/*!
 * @brief Called from PluginFactory::globalPluginDeinitialize()
 *
 * @see FB::FactoryBase::globalPluginDeinitialize
 */
void AXR::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
}

/*!
 * @brief AXR constructor. Note that your API is not available
 *        at this point, nor the window. For best results wait to use
 *        the JSAPI object until the onPluginReady method is called
 */
AXR::AXR()
{
}

/*!
 * @brief AXR destructor.
 */
AXR::~AXR()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

void AXR::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
}

void AXR::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
}

/*!
 * @brief Creates an instance of the JSAPI object that provides your main
 *        Javascript interface.
 *
 * Note that m_host is your BrowserHost and shared_ptr returns a
 * FB::PluginCorePtr, which can be used to provide a
 * boost::weak_ptr<AXR> for your JSAPI class.
 *
 * Be very careful where you hold a shared_ptr to your plugin class from,
 * as it could prevent your plugin class from getting destroyed properly.
 */
FB::JSAPIPtr AXR::createJSAPI()
{
    // m_host is the BrowserHost
    return boost::make_shared<AXRAPI>(FB::ptr_cast<AXR>(shared_from_this()), m_host);
}

bool AXR::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse down at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool AXR::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse up at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool AXR::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse move at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}
bool AXR::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *)
{
    // The window is attached; act appropriately
    return false;
}

bool AXR::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately
    return false;
}

#if FB_WIN
bool AXR::onDrawGDIWindowed(FB::RefreshEvent *evt, FB::PluginWindowWin *win)
{
    FB::Rect pos = win->getWindowPosition();
    pos.right -= pos.left;
    pos.left = 0;
    pos.bottom -= pos.top;
    pos.top = 0;

    PAINTSTRUCT ps;
    HDC context = BeginPaint(win->getHWND(), &ps);

    {
        ::SetTextAlign(context, TA_CENTERITA_BASELINE);
        LPCTSTR text = _T("FireBreath Plugin!");
        ::TextOut(context, pos.left + (pos.right - pos.left) / 2, pos.top + (pos.bottom - pos.top) / 2, text, lstrlen(text));
    }

    EndPaint(windowed->getHWND());

    return true;
}

bool AXR::onDrawGDIWindowless(FB::RefreshEvent *evt, FB::PluginWindowlessWin *win)
{
    FB::Rect pos = win->getWindowPosition();

    HDC context = win->getHDC();

    {
        ::SetTextAlign(context, TA_CENTERITA_BASELINE);
        LPCTSTR text = _T("FireBreath Plugin!");
        ::TextOut(context, pos.left + (pos.right - pos.left) / 2, pos.top + (pos.bottom - pos.top) / 2, text, lstrlen(text));
    }

    return true;
}
#elif FB_MACOSX
bool AXR::onDrawCoreGraphics(FB::CoreGraphicsDraw *evt, FB::PluginWindowMacCG *win)
{
    CGContextRef currentContext = evt->context;

    CGContextSetStrokeColorWithColor(currentContext, CGColorCreateGenericRGB(1, 0, 0, 1));
    CGContextSetLineWidth(currentContext, 5.0f);
    CGContextMoveToPoint(currentContext, 50.0f, 10.0f);
    CGContextAddLineToPoint(currentContext, 100.0f, 200.0f);
    CGContextStrokePath(currentContext);

    return true;
}
#endif
