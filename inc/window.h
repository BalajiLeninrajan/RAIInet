#ifndef WINDOW_H
#define WINDOW_H
#include <X11/Xlib.h>

#include <string>

class Xwindow {
    Display* d;
    Window w;
    int s, width, height;
    GC gc;
    unsigned long colours[10];
    Pixmap pix;
    XColor makeColour(Display* d, Colormap cmap, unsigned int hex);

   public:
    Xwindow(int width = 500,
            int height = 500);  // Constructor; displays the window.
    ~Xwindow();                 // Destructor; destroys the window.

    enum {
        White = 0,
        Black,
        lRed,
        dRed,
        lGreen,
        dGreen,
        lBlue,
        dBlue,
        lPurple,
        dPurple
    };  // Available colours.

    int getWidth() const;
    int getHeight() const;

    // Draws a rectangle TO BUFFER
    void fillRectangle(int x, int y, int width, int height, int colour = Black);

    // Draws a string TO BUFFER
    void drawString(int x, int y, std::string msg);

    // Draws a line TO BUFFER
    void drawLine(int x1, int y1, int x2, int y2, int thickness = 1, int colour = Black);

    void display();
};
#endif
