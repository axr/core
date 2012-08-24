#include "boost/program_options.hpp"
#ifdef _WIN32
#include <SDL/SDL_syswm.h>
#include <windows.h>
#else
#include "SDL/SDL.h"
#endif
#include "cairosdl/cairosdl.h"
#include "../core/AXR.h"
#ifdef _WIN32
#include "../core/os/win/WinAxrWrapper.h"
#else
#include "../core/os/linux/LinuxAxrWrapper.h"
#endif

#define cairo_argb(cr,argb) cairo_set_source_rgba(cr,(((argb)>>16)&0x000000ff)/256.f,(((argb)>>8)&0x000000ff)/256.f,((argb)&0x000000ff)/256.f,(((argb)>>24)&0x000000ff)/256.f)

namespace po = boost::program_options;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "AXR sdl";

cairo_t *cr;
using namespace AXR;

#ifdef _WIN32
#define AXR_WRAPPER_CLASS WinAxrWrapper
#else
#define AXR_WRAPPER_CLASS LinuxAxrWrapper
#endif

AXR_WRAPPER_CLASS *wrapper;

SDL_Surface* screen;

#ifdef _WIN32
HICON mainicon;
#endif

void render()
{
    if (wrapper->needsDisplay)
    {
        SDL_FillRect(screen, NULL, 0x00ffffff);

        AXRRect axrRect;
        axrRect.size.width = screen->w;
        axrRect.size.height = screen->h;
        axrRect.origin.x = 0;
        axrRect.origin.y = 0;

        AXRRect axrBounds;
        axrBounds.size.width = screen->w;
        axrBounds.size.height = screen->h;
        axrBounds.origin.x = 0;
        axrBounds.origin.y = 0;

        AXRCore::tp & core = AXRCore::getInstance();

        core->setCairo(cr);
        core->drawInRectWithBounds(axrRect, axrBounds);
        core->setCairo(NULL);

        wrapper->setNeedsDisplay(false);
    }
}

int main(int argc, char **argv)
{
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "show this message")
        ("file", po::value<std::string>(), "path to file to open")
        ("layout-tests", po::value<std::string>(), "run layout tests")
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
    cr = cairosdl_create(screen);

    SDL_WM_SetCaption(WINDOW_TITLE, 0);

    #ifdef _WIN32
        HINSTANCE handle = ::GetModuleHandle(NULL);
        mainicon = ::LoadIcon(handle, "main");
        SDL_SysWMinfo wminfo;
        SDL_VERSION(&wminfo.version);
        if (SDL_GetWMInfo(&wminfo) == 1) ::SetClassLong(wminfo.window, GCL_HICON, (LONG) mainicon);
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
        wrapper->_layoutTestsFilePath = varmap["layout-tests"].as<std::string>();

        core->registerCustomFunction("AXRLayoutTestsExecute",
            new AXR::HSSValueChangedCallback<AXR::LinuxAxrWrapper>(wrapper, &AXR::AXRWrapper::executeLayoutTests));

        wrapper->loadFileByPath(wrapper->getPathToResources() +
            "/views/layoutTests.hss");
    }
    else if (varmap.count("file") || additionalArgs.empty() == 0)
    {
        std::string filepath;

        if (additionalArgs.empty())
        {
            filepath = varmap["file"].as<std::string>();
        }
        else
        {
            filepath = additionalArgs[0];
        }

        wrapper->loadFileByPath(filepath);
    }
    else
    {
        wrapper->loadFile();
    }

    int done = 0;

    while (!done)
    {
    SDL_Event event;
    while(SDL_PollEvent(&event))
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
                if (wrapper->hasLoadedFile())
                {
                    std::cout << "Reloading file\n";
                    wrapper->reload();
                }
            }
            else if (event.key.keysym.sym == SDLK_o && (event.key.keysym.mod & KMOD_CTRL))
            {
                wrapper->loadFile();
            }
        }
        else if (event.type == SDL_VIDEORESIZE)
        {
            cairosdl_destroy(cr);
            screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
            SDL_SetAlpha(screen, SDL_SRCALPHA, CAIROSDL_AMASK);
            cr = cairosdl_create(screen);
            wrapper->setNeedsDisplay(true);
        }
        else
        {
        }
    }

    render();

    SDL_Flip(screen);
    SDL_Delay(1000 / 30);
    }

    SDL_Quit();

    #ifdef _WIN32
        ::DestroyIcon(mainicon);
    #endif

    free(wrapper);
    free(screen);
    free(cr);

    return 0;
}
