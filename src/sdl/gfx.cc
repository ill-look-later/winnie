#ifdef WINNIE_SDL
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "gfx.h"
#include "shalloc.h"

static SDL_Surface *fbsurf;

struct Graphics {
	Rect screen_rect;
	Rect clipping_rect;
	int color_depth; // bits per pixel
	Pixmap *pixmap;
};

static Graphics *gfx;

bool init_gfx()
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		fprintf(stderr, "failed to initialize SDL\n");
		return false;
	}

	if(!(gfx = (Graphics*)sh_malloc(sizeof *gfx))) {
		return false;
	}

	Rect scr_rect = {0, 0, 1024, 768};
	gfx->screen_rect = scr_rect;
	gfx->color_depth = 32;

	if(!(fbsurf = SDL_SetVideoMode(gfx->screen_rect.width, gfx->screen_rect.height, gfx->color_depth, 0))) {
		fprintf(stderr, "Failed to set video mode\n");
		return false;
	}
	SDL_ShowCursor(0);

	if(!(gfx->pixmap = (Pixmap*)sh_malloc(sizeof(Pixmap)))) {
		fprintf(stderr, "Failed to allocate pixmap.\n");
		return false;
	}

	gfx->pixmap->width = gfx->screen_rect.width;
	gfx->pixmap->height = gfx->screen_rect.height;

	int fbsize = gfx->pixmap->width * gfx->pixmap->height * gfx->color_depth / 8;
	if(!(gfx->pixmap->pixels = (unsigned char*)sh_malloc(fbsize))) {
		fprintf(stderr, "failed to allocate the pixmap framebuffer.\n");
		return false;
	}

	set_clipping_rect(gfx->screen_rect);

	return true;
}

void destroy_gfx()
{
	sh_free(gfx->pixmap->pixels);
	gfx->pixmap->pixels = 0;
	sh_free(gfx->pixmap);
	sh_free(gfx);
	SDL_Quit();
}

unsigned char *get_framebuffer()
{
	return gfx->pixmap->pixels;
}

Pixmap *get_framebuffer_pixmap()
{
	return gfx->pixmap;
}

Rect get_screen_size()
{
	return gfx->screen_rect;
}

int get_color_depth()
{
	return gfx->color_depth;
}

/*void set_clipping_rect(const Rect &rect)
{
	gfx->clipping_rect = rect_intersection(rect, gfx->screen_rect);

	SDL_Rect sdl_rect;
	sdl_rect.x = gfx->clipping_rect.x;
	sdl_rect.y = gfx->clipping_rect.y;
	sdl_rect.w = gfx->clipping_recvoid fill_rect(const Rect &rect, int r, int g, int b)
{
	Rect drect = rect;
	Rect screen_rect = get_screen_size();

	if(drect.x < clipping_rect.x) {
		drect.width -= clipping_rect.x - drect.x;
		drect.x = clipping_rect.x;
	}

	if(drect.y < clipping_rect.y) {
		drect.height -= clipping_rect.y - drect.y;
		drect.y = clipping_rect.y;
	}

	if(drect.x + drect.width >= clipping_rect.x + clipping_rect.width) {
		drect.width = clipping_rect.width + clipping_rect.x - drect.x;
	}

	if(drect.y + drect.height >= clipping_rect.y + clipping_rect.height) {
		drect.height = clipping_rect.height + clipping_rect.y - drect.y;
	}

	unsigned char *fb = get_framebuffer() + (drect.x + screen_rect.width * drect.y) * 4;
	for(int i=0; i<drect.height; i++) {
		for(int j=0; j<drect.width; j++) {
			fb[j * 4] = b;
			fb[j * 4 + 1] = g;
			fb[j * 4 + 2] = r;
		}
		fb += screen_rect.width * 4;
	}
}
t.width;
	sdl_rect.h = gfx->clipping_rect.height;

	SDL_SetClipRect(fbsurf, &sdl_rect);
}

const Rect &get_clipping_rect()
{
	return gfx->clipping_rect;
}

void clear_screen(int r, int g, int b)
{
	fill_rect(gfx->screen_rect, r, g, b);
}

void fill_rect(const Rect &rect, int r, int g, int b)
{
	uint32_t color = ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
	
	SDL_Rect sdl_rect;
	sdl_rect.x = rect.x;
	sdl_rect.y = rect.y;
	sdl_rect.w = rect.width;
	sdl_rect.h = rect.height;

	SDL_FillRect(fbsurf, &sdl_rect, color);
}*/

void set_clipping_rect(const Rect &rect)
{
	gfx->clipping_rect = rect_intersection(rect, get_screen_size());
}

const Rect &get_clipping_rect()
{
	return gfx->clipping_rect;
}


void set_cursor_visibility(bool visible)
{
}

void gfx_update()
{
	if(SDL_MUSTLOCK(fbsurf)) {
		SDL_LockSurface(fbsurf);
	}
	memcpy(fbsurf->pixels, gfx->pixmap->pixels, gfx->pixmap->width * gfx->pixmap->height * (gfx->color_depth / 8));
	if(SDL_MUSTLOCK(fbsurf)) {
		SDL_UnlockSurface(fbsurf);
	}
	SDL_UpdateRect(fbsurf, 0, 0, 0, 0);
}

void wait_vsync()
{
}

#endif // WINNIE_SDL
