#include "window.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

Xwindow::Xwindow(int width, int height) : width{width}, height{height} {
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        cerr << "Cannot open display" << endl;
        exit(1);
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapRaised(d, w);

    pix = XCreatePixmap(d, w, width, height, DefaultDepth(d, DefaultScreen(d)));
    gc = XCreateGC(d, pix, 0, (XGCValues*)0);

    XFlush(d);
    XFlush(d);

    // Set up colours.
    XColor xcolour;
    Colormap cmap;

    unsigned int hexColours[10] = {
        0xFFFFFF,  // White
        0x000000,  // Black
        0xFF6B6B,  // lRed
        0x8B0000,  // dRed
        0x90EE90,  // lGreen
        0x006400,  // dGreen
        0xADD8E6,  // lBlue
        0x00008B,  // dBlue
        0xD8BFD8,  // lPurple
        0x4B0082   // dPurple
    };

    cmap = DefaultColormap(d, DefaultScreen(d));
    for (int i = 0; i < 10; ++i) {
        xcolour = makeColour(d, cmap, hexColours[i]);
        colours[i] = xcolour.pixel;
    }

    XSetForeground(d, gc, colours[Black]);

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
    hints.height = hints.base_height = hints.min_height = hints.max_height =
        height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints(d, w, &hints);

    // NOTE: This is removed to improve performance
    // XSynchronize(d, True);

    usleep(1000);
}

XColor Xwindow::makeColour(Display* d, Colormap cmap, unsigned int hex) {
    XColor colour;
    colour.red = ((hex >> 16) & 0xFF) * 257;
    colour.green = ((hex >> 8) & 0xFF) * 257;
    colour.blue = (hex & 0xFF) * 257;
    colour.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(d, cmap, &colour);
    return colour;
}

Xwindow::~Xwindow() {
    XFreeGC(d, gc);
    XCloseDisplay(d);
}

int Xwindow::getWidth() const { return width; }
int Xwindow::getHeight() const { return height; }

void Xwindow::display() {
    XCopyArea(d, pix, w, gc, 0, 0, width, height, 0, 0);
    XFlush(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
    XSetForeground(d, gc, colours[colour]);
    XFillRectangle(d, pix, gc, x, y, width, height);
    XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, string msg) {
    XDrawString(d, pix, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
}

void Xwindow::drawLine(int x1, int y1, int x2, int y2, int thickness, int colour) {
    XSetForeground(d, gc, colours[colour]);
    XSetLineAttributes(d, gc, thickness, LineSolid, CapRound, JoinRound);
    XDrawLine(d, pix, gc, x1, y1, x2, y2);
    XSetLineAttributes(d, gc, 1, LineSolid, CapRound, JoinRound);
    XSetForeground(d, gc, colours[Black]);
}
