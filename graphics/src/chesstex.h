#pragma once

#include <texture.h>
#include <vectors.h>
#include <vector>

class ChessTex : public Texture {
	std::vector<vec4> pixels;

	float* getPixels() {
		return &pixels[0].x;
	}
public:
	ChessTex(vec4 color1, vec4 color2) {
		int widht = 50, height = 50;
		int alapcol = 1, col = 0;;


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
		return 50;
	}

	int getHeight() {
		return 50;
	}
};