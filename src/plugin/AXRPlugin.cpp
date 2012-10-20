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

#include <QApplication>

#include "AXRAPI.h"
#include "AXRPlugin.h"

#include "AXRDocument.h"
#include "AXRRender.h"

#include "DOM/Window.h"

#if FB_WIN
#include <windows.h>
#elif FB_MACOSX
#include <ApplicationServices/ApplicationServices.h>
#endif

using namespace AXR;

static class QCoreApplication *app = NULL;

/*!
 * @brief Called from PluginFactory::globalPluginInitialize()
 *
 * @see FB::FactoryBase::globalPluginInitialize
 */
void AXRPlugin::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
    int argc = 0;
    char **argv = NULL;
    app = new QApplication(argc, argv);
}

/*!
 * @brief Called from PluginFactory::globalPluginDeinitialize()
 *
 * @see FB::FactoryBase::globalPluginDeinitialize
 */
void AXRPlugin::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
    delete app;
}

/*!
 * @brief AXRPlugin constructor. Note that your API is not available
 *        at this point, nor the window. For best results wait to use
 *        the JSAPI object until the onPluginReady method is called
 */
AXRPlugin::AXRPlugin()
{
    document = new AXRDocument();
}

/*!
 * @brief AXRPlugin destructor.
 */
AXRPlugin::~AXRPlugin()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
    delete document;
}

void AXRPlugin::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.

    document->loadFileByPath(QUrl(fromStdString(m_host->getDOMWindow()->getLocation())));
}

void AXRPlugin::shutdown()
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
FB::JSAPIPtr AXRPlugin::createJSAPI()
{
    // m_host is the BrowserHost
    return boost::make_shared<AXRAPI>(FB::ptr_cast<AXRPlugin>(shared_from_this()), m_host);
}

bool AXRPlugin::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse down at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool AXRPlugin::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse up at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool AXRPlugin::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse move at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}
bool AXRPlugin::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *)
{
    // The window is attached; act appropriately
    return false;
}

bool AXRPlugin::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately
    return false;
}

QImage AXRPlugin::composite(int width, int height)
{
    AXRRender::p renderer = document->getRender();
    if (renderer && document->getController()->getRoot())
    {
        QRect paintRect = QRect(0, 0, width, height);

        document->drawInRectWithBounds(paintRect, paintRect);
        return renderer->surface();
    }

    return QImage();
}

#if FB_WIN
bool AXRPlugin::onDrawGDIWindowed(FB::RefreshEvent *evt, FB::PluginWindowWin *win)
{
    FB::Rect pos = win->getWindowPosition();
    pos.right -= pos.left;
    pos.left = 0;
    pos.bottom -= pos.top;
    pos.top = 0;

    PAINTSTRUCT ps;
    HDC context = BeginPaint(win->getHWND(), &ps);

    {
        LPCTSTR text = _T("FireBreath Plugin!");
        ::TextOut(context, pos.left + (pos.right - pos.left) / 2, pos.top + (pos.bottom - pos.top) / 2, text, lstrlen(text));
    }

    EndPaint(win->getHWND(), &ps);

    return true;
}

bool AXRPlugin::onDrawGDIWindowless(FB::RefreshEvent *evt, FB::PluginWindowlessWin *win)
{
    FB::Rect pos = win->getWindowPosition();

    HDC context = win->getHDC();

    {
        LPCTSTR text = _T("FireBreath Plugin!");
        ::TextOut(context, pos.left + (pos.right - pos.left) / 2, pos.top + (pos.bottom - pos.top) / 2, text, lstrlen(text));
    }

    return true;
}
#elif FB_MACOSX
bool AXRPlugin::onDrawCoreGraphics(FB::CoreGraphicsDraw *evt, FB::PluginWindowMacCG *win)
{
    CGContextRef currentContext = evt->context;

    // Flip the coordinate system since Mac is upside down
    CGContextTranslateCTM(currentContext, 0, win->getWindowHeight());
    CGContextScaleCTM(currentContext, 1, -1);

    CGRect paintRect = CGRectMake(0, 0, win->getWindowWidth(), win->getWindowHeight());
    QPixmap pix = QPixmap::fromImage(composite(win->getWindowWidth(), win->getWindowHeight()));
    CGImageRef image = pix.toMacCGImageRef();
    CGContextDrawImage(currentContext, paintRect, image);
    CGImageRelease(image);

    return true;
}
#elif FB_X11
bool AXRPlugin::onDrawX11(FB::RefreshEvent *evt, FB::PluginWindowX11 *)
{
    // TODO: Implement drawing for X11 platforms
}
#endif
