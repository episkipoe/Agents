#ifndef TEXT_H
#define TEXT_H

#include <common/rect.h>
#include <string>

struct Font {
	Font() : size(0.05), outline(false) { }
	Font(float s) : size(s), outline(false) { }

	float size;
	bool outline;
};

void drawText(float x, float y, const std::string & text, const Font & font);
void drawText(float x, float y, float z, const std::string & text, const Font & font);
Rect boundingBox(const std::string & text, const Font & font);
#endif
