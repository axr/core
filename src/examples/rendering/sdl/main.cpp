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

#include <SDL.h>
#include <QApplication>
#include <QFileDialog>
#include <QUrl>
#include "AXRDebugging.h"
#include "AXRInitializer.h"

#if defined(_WIN32)
#include <SDL_syswm.h>
#include <windows.h>
#endif

using namespace AXR;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "AXR SDL Demo";

AXRCore *core;
SDL_Surface* screen = NULL;

#ifdef _WIN32
HICON mainicon;
#endif

/*!
 * Converts a QImage to an SDL_Surface.
 * The source image is converted to ARGB32 format if it is not already.
 * The caller is responsible for deallocating the returned pointer.
 */
SDL_Surface* QImage_toSDLSurface(const QImage &sourceImage)
{
    // Ensure that the source image is in the correct pixel format
    QImage image = sourceImage;
    if (image.format() != QImage::Format_ARGB32)
        image = image.convertToFormat(QImage::Format_ARGB32);

    // QImage stores each pixel in ARGB format
    // Mask appropriately for the endianness
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 amask = 0x000000ff;
    Uint32 rmask = 0x0000ff00;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0xff000000;
#else
    Uint32 amask = 0xff000000;
    Uint32 rmask = 0x00ff0000;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x000000ff;
#endif

    return SDL_CreateRGBSurfaceFrom((void*)image.constBits(), image.width(), image.height(), image.depth(), image.bytesPerLine(), rmask, gmask, bmask, amask);
}

void render()
{
    if (core->needsDisplay())
    {
        HSSRect bounds(0, 0, screen->w, screen->h);
        core->drawInRectWithBounds(bounds, bounds);

        SDL_FillRect(screen, NULL, SDL_MapRGBA(screen->format, 255, 255, 255, 255));
        SDL_Surface *surf = QImage_toSDLSurface(core->getRender()->surface());
        SDL_BlitSurface(surf, NULL, screen, NULL);
        SDL_FreeSurface(surf);
        SDL_Flip(screen);

        core->setNeedsDisplay(false);
    }
}

void loadFile()
{
    QString filepath = QFileDialog::getOpenFileName(NULL, QObject::tr("Open File"), QString(), QObject::tr("AXR Files (*.xml *.hss)"));
    core->loadFileByPath(QUrl::fromLocalFile(filepath));
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    // Get command-line arguments
    QStringList args = a.arguments();
    args.removeFirst();

    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);

    SDL_WM_SetCaption(WINDOW_TITLE, 0);

#ifdef _WIN32
    HINSTANCE handle = ::GetModuleHandle(NULL);
    mainicon = ::LoadIcon(handle, "main");
    SDL_SysWMinfo wminfo;
    SDL_VERSION(&wminfo.version);
    if (SDL_GetWMInfo(&wminfo) == 1)
        ::SetClassLong(wminfo.window, GCL_HICON, (LONG) mainicon);
#endif

    SDL_EnableKeyRepeat(300, 50);
    SDL_EnableUNICODE(1);

    core = AXRCore::getInstance();

    if (!args.empty())
    {
        QFileInfo fi(args.last());
        if (fi.exists())
        {
            core->loadFileByPath(QUrl::fromLocalFile(AXR::fromQString(args.first())));
        }
        else
        {
            loadFile();
        }
    }
    else
    {
        loadFile();
    }

    int done = 0;

    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = 1;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                HSSContainer::p root = core->getController()->getRoot();

                if (root)
                {
                    HSSPoint thePoint;
                    thePoint.x = event.button.x;
                    thePoint.y = event.button.y;
                    root->handleEvent(HSSEventTypeMouseDown, &thePoint);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                HSSContainer::p root = core->getController()->getRoot();

                if (root)
                {
                    HSSPoint thePoint;
                    thePoint.x = event.button.x;
                    thePoint.y = event.button.y;
                    root->handleEvent(HSSEventTypeMouseUp, &thePoint);
                    root->handleEvent(HSSEventTypeClick, &thePoint);
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                HSSContainer::p root = core->getController()->getRoot();

                if (root)
                {
                    HSSPoint thePoint;
                    thePoint.x = event.motion.x;
                    thePoint.y = event.motion.y;
                    root->handleEvent(HSSEventTypeMouseMove, &thePoint);
                }
            }
            else if (event.type == SDL_KEYDOWN)
            {
            }
            else if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_F5)
                {
                    std_log("Reloading file");
                    core->reload();
                }
                else if (event.key.keysym.sym == SDLK_o && (event.key.keysym.mod & KMOD_CTRL))
                {
                    loadFile();
                }
            }
            else if (event.type == SDL_VIDEORESIZE)
            {
                screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
                core->setNeedsDisplay(true);
            }
            else if (event.active.state & SDL_APPMOUSEFOCUS && event.active.gain == 0)
            {
                HSSContainer::p root = core->getController()->getRoot();

                if (root)
                {
                    root->handleEvent(HSSEventTypeExitedWindow, NULL);
                }
            }
        }

        render();

        SDL_Delay(1000 / 30);
    }

    SDL_Quit();

#ifdef _WIN32
    ::DestroyIcon(mainicon);
#endif

    // TODO: don't delete until thread pointer is eliminated
    //delete core;

    return 0;
}
