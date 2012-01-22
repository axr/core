#include "cairosdl/cairosdl.h"

#ifdef __WIN32__
	#include <SDL/SDL_syswm.h>
	#include <windows.h>
	HICON mainicon;
#endif

#include "../core/AXR.h"
#include "../core/os/win/WinAxrWrapper.h"

#define cairo_argb(cr,argb) cairo_set_source_rgba(cr,(((argb)>>16)&0x000000ff)/256.f,(((argb)>>8)&0x000000ff)/256.f,((argb)&0x000000ff)/256.f,(((argb)>>24)&0x000000ff)/256.f)

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "AXR sdl";

cairo_t *cr;
using namespace AXR;

WinAxrWrapper *wrapper;

SDL_Surface* screen;

void render(){
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

int main(int argc, char **argv) {


	SDL_Init( SDL_INIT_VIDEO );
	screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE );
	cr = cairosdl_create (screen);
	cairo_translate(cr,0.5,0.5);

	SDL_WM_SetCaption( WINDOW_TITLE, 0 );

	#ifdef __WIN32__
		HINSTANCE handle = ::GetModuleHandle(NULL);
		mainicon = ::LoadIcon(handle, "main");
		SDL_SysWMinfo wminfo;
		SDL_VERSION(&wminfo.version);
		if (SDL_GetWMInfo(&wminfo) == 1) ::SetClassLong(wminfo.window, GCL_HICON, (LONG) mainicon);
	#endif


	SDL_EnableKeyRepeat(300, 50);    
	SDL_EnableUNICODE(1);



	
	wrapper = new WinAxrWrapper();
	AXRCore::p core = wrapper->getCore();
	wrapper->hwnd = wminfo.window;

	wrapper->loadFile();


	SDL_Event event;
	while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_QUIT) {
				break;
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			} else if (event.type == SDL_MOUSEBUTTONUP) {
			} else if (event.type == SDL_MOUSEMOTION) {
			} else if (event.type == SDL_KEYUP) {
			} else if (event.type == SDL_KEYDOWN) {
				Beep(400,400);
			} else if(event.type == SDL_VIDEORESIZE) {
				cairosdl_destroy (cr);
				screen = SDL_SetVideoMode (event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
				SDL_SetAlpha(screen,SDL_SRCALPHA,CAIROSDL_AMASK);
				cr = cairosdl_create (screen);
				cairo_translate(cr,0.5,0.5);
			} else {
				render();
			}

			SDL_Flip(screen);
			//SDL_Delay(1000/30);
	}
	SDL_Quit();
#ifdef __WIN32__
	::DestroyIcon(mainicon);
#endif
	return 0;
}
