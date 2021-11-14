/*
 * File: Sierpinski.cpp
 * ---------------
 * A program that generates the Sierpinski triangle, a famous fractal that consists of of patterns of triangles
 * This program draws a triangle with the specified corners in the specified window
 */

#include "Sierpinski.h"
#include "error.h"
using namespace std;

// drawTriangle() function draws a triangle at the given coordinates, with the given window.
void drawTriangle(GWindow& window,
                  double x0, double y0,
                  double x1, double y1,
                  double x2, double y2) {
    window.setColor("black");
    window.fillPolygon({ x0, y0, x1, y1, x2, y2 });
}

/* drawSierpinskiTriangle() is a recursive function that draws a Sierpinski triangle of order n and takes as parameters
 * the coordinates x0, y0, x1, y1, x2, y2 for the three corners with which to draw the triangle, and the window to draw it on.
 * If the order is negative, the function returns an error.
 */
void drawSierpinskiTriangle(GWindow& window,
                            double x0, double y0,
                            double x1, double y1,
                            double x2, double y2,
                            int order) {
    if(order<0) error("the order must greater than 0!");
    if(order==0){
        drawTriangle(window,x0,y0,x1,y1,x2,y2);
        return;
    }
    drawSierpinskiTriangle(window,x0,y0,(x0+x1)/2,(y0+y1)/2,(x0+x2)/2,(y0+y2)/2,order-1);
    drawSierpinskiTriangle(window,(x0+x1)/2,(y0+y1)/2,x1,y1,(x1+x2)/2,(y1+y2)/2,order-1);
    drawSierpinskiTriangle(window,(x0+x2)/2,(y0+y2)/2,(x1+x2)/2,(y1+y2)/2,x2,y2,order-1);
    return;
}

