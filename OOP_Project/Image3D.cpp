#include "Image3D.h"

Uint32 Image3D::getColor(int x, int y) {
	if (x >= width)
		x = width - 1;
	if (x < 0)
		x = 0;
	if (y >= height)
		y = height - 1;
	if (y < 0)
		y = 0;
	return pixels[width * y + x];
}