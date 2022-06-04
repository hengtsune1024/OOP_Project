#include "Line.h"

Line::Line() :curve(0), x(0), y(0), z(0), type(0), clip(0), slope(0), sprite(NULL), actsprite(NULL)
{}

Line::~Line() {
    //no need to delete the image, since they all point to the image object in Map class
}

void Line::project(int camX, int camY, int camZ, double camDegree, double camDepth)
{
    double _x = x, _y = y - camY, _z = z - camZ;
    double co = cos(camDegree), si = sin(camDegree);
    _x = co * (x - camX) - si * (z - camZ);
    //_z = si * (x - camX) + co * (z - camZ);

    if (_z < 0) {
        W = 0;
        return;
    }
    scale = camDepth / _z;

    X = (1 + scale * _x) * WIDTH / 2;
    Y = (1 - scale * _y) * HEIGHT / 2;
    W = scale * ROAD_WIDTH * WIDTH / 2;
}

void Line::drawSprite(SDL_Renderer* renderer) {
    if (sprite == NULL)
        return;
    int w = sprite->getWidth(), h = sprite->getHeight();
    double destX = X - scale * spriteX * WIDTH / 4;
    double destY = Y + 4;
    double destW = w * W / 150.0;
    double destH = h * W / 150.0;

    destX -= destW * spriteX;
    destY += -destH;

    double clipH = destY + destH - clip - 1;
    if (clipH < 0)
        clipH = 0;

    if (clipH >= destH)
        return;

    SDL_Rect src = { 0,0,w,h - h * clipH / destH };
    SDL_Rect dst = { destX,destY,destW,destH - clipH };
    sprite->draw(renderer, &src, &dst);

    destX = X + scale * spriteX * WIDTH / 4 - 30;
    destX += destW * spriteX;
    dst.x = destX;
    sprite->draw(renderer, &src, &dst, true);

}

void Line::drawActSprite(SDL_Renderer* renderer, int state) {
    if (actsprite == NULL)
        return;
    int w = actsprite[state].getWidth(), h = actsprite[state].getHeight();
    double destX = X;
    double destY = Y + 4;
    double destW = w * W / 380.0;
    double destH = h * W / 380.0;

    //destX -= destW * spriteX;
    destY += -destH;

    double clipH = destY + destH - clip - 2;
    if (clipH < 0)
        clipH = 0;

    if (clipH >= destH)
        return;

    SDL_Rect src = { 0,0,w,h - h * clipH / destH };
    SDL_Rect dst = { destX - destW / 2,destY,destW,destH - clipH };
    actsprite[state].draw(renderer, &src, &dst);

   
}

//if (W > WIDTH)
//    W = WIDTH;
/*
scale = CAMERA_DEPTH / (z - camZ);
X = (1 + scale * (x - camX)) * WIDTH / 2;
Y = (1 - scale * (y - camY)) * HEIGHT / 2;
W = scale * ROAD_WIDTH * WIDTH / 2;*/

//destX = X + scale * spriteX * WIDTH / 2;
   //destX += destW * spriteX;
   //dst.x = destX;
   //actsprite[state].draw(renderer, &src, &dst);