#pragma once

#include <texture.h>
#include <vectors.h>
#include <vector>

class ChessTex : public Texture {
	std::vector<vec4> pixels;
	int widht, height;
	const float* getPixels() {
		return &pixels[0].x;
	}
public:
	ChessTex(vec4 color1, vec4 color2, int awidht, int aheight) {
		int alapcol = 1, col = 0;;
		widht = awidht;
		height = aheight;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < widht; j++)
			{
				col = j % 2;
				if (i % 2 == 0)
					col = col == 1 ? 0 : 1;

				if (col == 0)
					pixels.push_back(color1);
				else
					pixels.push_back(color2);
			}
			
		}

		
	}

	int getWidth() {
		return widht;
	}

	int getHeight() {
		return height;
	}
};