/*
gcc xftbug.c -lX11 -lXft -I /usr/include/freetype2 -lfontconfig
*/
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

int main(void) {
	Display *dpy = XOpenDisplay(NULL);
	if(dpy == NULL)
		return 1;
	int screen = DefaultScreen(dpy);

	char *fontname = "Noto Color Emoji";
	XftFont *xfont = XftFontOpenName(dpy, screen, fontname);
	if(xfont == NULL) {
		printf("Can't load font pattern");
		return 1;
	}

	XGlyphInfo ext;
	XftTextExtentsUtf8(dpy, xfont, "\xf0\x9f\x93\x93", 4, &ext);
	fprintf(stderr, "OK1\n");
	XSync (dpy, False);
	fprintf(stderr, "OK2\n");

	XCloseDisplay(dpy);
}
