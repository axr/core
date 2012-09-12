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

#include <boost/program_options.hpp>
#include <QApplication>
#include <QFileDialog>
#include "AXR.h"

#if defined(_WIN32)
#include <SDL/SDL_syswm.h>
#include <windows.h>
#include "WindowsAXRWrapper.h"
#ifndef AXR_WRAPPER_CLASS
#define AXR_WRAPPER_CLASS WindowsAXRWrapper
#endif
#else
#include <SDL/SDL.h>
#include "GenericAXRWrapper.h"
#ifndef AXR_WRAPPER_CLASS
#define AXR_WRAPPER_CLASS GenericAXRWrapper
#endif
#endif

namespace po = boost::program_options;
using namespace AXR;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "AXR SDL";

AXR_WRAPPER_CLASS *wrapper;
SDL_Surface* screen = NULL;

#ifdef _WIN32
HICON mainicon;
#endif

/* SDL interprets each pixel as a 32-bit number, so our masks must depend
 on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
Uint32 rmask = 0xff000000;
Uint32 gmask = 0x00ff0000;
Uint32 bmask = 0x0000ff00;
Uint32 amask = 0x000000ff;
#else
Uint32 rmask = 0x000000ff;
Uint32 gmask = 0x0000ff00;
Uint32 bmask = 0x00ff0000;
Uint32 amask = 0xff000000;
#endif

void render()
{
    if (wrapper->needsDisplay)
    {
        SDL_FillRect(screen, NULL, SDL_MapRGBA(screen->format, 255, 255, 255, 255));

        AXRCore::tp &core = AXRCore::getInstance();
        HSSRect bounds(0, 0, screen->w, screen->h);
        core->drawInRectWithBounds(bounds, bounds);

        const QImage *qsurf = core->getRender()->surface();

        SDL_Surface *surf = SDL_CreateRGBSurfaceFrom((void*)qsurf->constBits(), qsurf->width(), qsurf->height(), 32, qsurf->width() * 4, rmask, gmask, bmask, amask);
        SDL_BlitSurface(surf, NULL, screen, NULL);
        SDL_FreeSurface(surf);
        SDL_Flip(screen);

        wrapper->setNeedsDisplay(false);
    }
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "show this message")
            ("file", po::value<std::string > (), "path to file to open")
            ("layout-tests", po::value<std::string > (), "run layout tests")
            ;

    po::variables_map varmap;
    std::vector<std::string> additionalArgs;

    // Parse arguments
    po::parsed_options parsed = po::command_line_parser(argc, argv)
            .options(desc).allow_unregistered().run();
    po::store(parsed, varmap);
    additionalArgs = po::collect_unrecognized(parsed.options,
                                              po::include_positional);
    po::notify(varmap);

    // Display help message
    if (varmap.count("help"))
    {
        std::cout << desc << "\n";
        return 0;
    }

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

    wrapper = new AXR_WRAPPER_CLASS();
    AXRCore::tp & core = AXRCore::getInstance();
#ifdef _WIN32
    wrapper->hwnd = wminfo.window;
#endif

    if (varmap.count("layout-tests"))
    {
        wrapper->_layoutTestsFilePath = varmap["layout-tests"].as<std::string > ();

        core->registerCustomFunction("AXRLayoutTestsExecute",
                                     new AXR::HSSValueChangedCallback<AXR_WRAPPER_CLASS > (wrapper, &AXR::AXRWrapper::executeLayoutTests));

        wrapper->loadFileByPath(wrapper->getPathToResources() +
                                "/views/layoutTests.hss");
    }
    else if (varmap.count("file") || additionalArgs.empty() == 0)
    {
        std::string filepath;

        if (additionalArgs.empty())
        {
            filepath = varmap["file"].as<std::string > ();
        }
        else
        {
            filepath = additionalArgs[0];
        }

        wrapper->loadFileByPath(filepath);
    }
    else
    {
        QString filepath = QFileDialog::getOpenFileName(NULL, QObject::tr("Open File"), QString(), QObject::tr("AXR Files (*.xml *.hss)"));
        wrapper->loadFileByPath(filepath.toStdString());
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
                AXRCore::tp & core = AXRCore::getInstance();
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
                AXRCore::tp & core = AXRCore::getInstance();
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
                AXRCore::tp & core = AXRCore::getInstance();
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
                    std::cout << "Reloading file\n";
                    wrapper->reload();
                }
                else if (event.key.keysym.sym == SDLK_o && (event.key.keysym.mod & KMOD_CTRL))
                {
                    wrapper->loadFile();
                }
            }
            else if (event.type == SDL_VIDEORESIZE)
            {
                screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
                wrapper->setNeedsDisplay(true);
            }
            else if (event.active.state & SDL_APPMOUSEFOCUS && event.active.gain == 0)
            {
                AXRCore::tp & core = AXRCore::getInstance();
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

    delete wrapper;

    return 0;
}
