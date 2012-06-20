#include "SDL/SDL.h"
#include "cairosdl/cairosdl.h"
#include "../core/AXR.h"
#include "../core/os/linux/LinuxAxrWrapper.h"

#define cairo_argb(cr,argb) cairo_set_source_rgba(cr,(((argb)>>16)&0x000000ff)/256.f,(((argb)>>8)&0x000000ff)/256.f,((argb)&0x000000ff)/256.f,(((argb)>>24)&0x000000ff)/256.f)

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "AXR sdl";

cairo_t *cr;
using namespace AXR;

LinuxAxrWrapper *wrapper;

SDL_Surface* screen;

void render ()
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

int main (int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,
		SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
	cr = cairosdl_create (screen);

	SDL_WM_SetCaption(WINDOW_TITLE, 0);

	SDL_EnableKeyRepeat(300, 50);
	SDL_EnableUNICODE(1);

	wrapper = new LinuxAxrWrapper();
	AXRCore::tp & core = AXRCore::getInstance();

	if (argc > 1)
	{
		std::string filepath;
		filepath = argv[1];

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
			}
			else if (event.type == SDL_KEYDOWN)
			{
			}
			else if (event.type == SDL_VIDEORESIZE)
			{
				cairosdl_destroy (cr);
				screen = SDL_SetVideoMode (event.resize.w, event.resize.h, 32,
					SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
				SDL_SetAlpha(screen, SDL_SRCALPHA, CAIROSDL_AMASK);
				cr = cairosdl_create (screen);

				wrapper->setNeedsDisplay(true);
			}
			else
			{
			}
		}

		render();

		SDL_Flip(screen);
		SDL_Delay(1000/30);
	}

	SDL_Quit();

	free(wrapper);
	free(screen);
	free(cr);

	return 0;
}

