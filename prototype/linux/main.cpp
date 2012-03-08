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
	SDL_FillRect(screen, NULL, 0x00000000);

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

	AXRCore::p core = wrapper->getCore();

	core->setCairo(cr);
	core->drawInRectWithBounds(axrRect, axrBounds);
	core->setCairo(NULL);
}

int main (int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,
		SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
	cr = cairosdl_create (screen);
	cairo_translate(cr,0.5,0.5);

	SDL_WM_SetCaption(WINDOW_TITLE, 0);

	SDL_EnableKeyRepeat(300, 50);
	SDL_EnableUNICODE(1);

	wrapper = new LinuxAxrWrapper();
	AXRCore::p core = wrapper->getCore();

	wrapper->loadFile();

	SDL_Event event;
	while (SDL_WaitEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			break;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
		}
		else if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_F5)
			{
				wrapper->reload();
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
			SDL_SetAlpha(screen,SDL_SRCALPHA, CAIROSDL_AMASK);
			cr = cairosdl_create (screen);
			cairo_translate(cr, 0.5, 0.5);
		}
		else
		{
		}

		render();

		SDL_Flip(screen);
		//SDL_Delay(1000/30);
	}

	SDL_Quit();

	return 0;
}

