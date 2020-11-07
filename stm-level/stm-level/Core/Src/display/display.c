#define OLED_WIDTH 96
#define OLED_HEIGTH 64

#include "display/display.h"
#include <string.h>
#include "math.h"

void displayInit() {
	ssd1331_init();

	bgColor = BLACK;
	currentFrameBuffer = A;
	lastFrameBuffer = B;
	centerX = centerY = 0;
}

void resetBuffer(enum db buffer) {
	if (buffer == A) {
		memset(bufferA, 0, 6144 * sizeof(uint16_t));
	} else if (buffer == B) {
		memset(bufferB, 0, 6144 * sizeof(uint16_t));
	}
}

void switchBuffers() {
	if (currentFrameBuffer == A) {
		currentFrameBuffer = B;
		lastFrameBuffer = A;
	} else {
		currentFrameBuffer = A;
		lastFrameBuffer = B;
	}
}

uint16_t* getCurrentBuffer() {
	if (currentFrameBuffer == A) {
		return bufferA;
	} else {
		return bufferB;
	}
}

uint16_t* getSecondaryBuffer() {
	if (currentFrameBuffer == A) {
		return bufferB;
	} else {
		return bufferA;
	}
}

int twoDimmensionsToOneIndex(int posX, int posY) {
	int index = (posY * OLED_WIDTH) + posX;
	if (index > (OLED_HEIGTH * OLED_WIDTH)) {
		return 0;
	}
	return index;
}

void displayUpdate() {
	uint16_t *cb = getCurrentBuffer();
	uint16_t *ob = getSecondaryBuffer();

	int index = 0;

	for (int x = 0; x < OLED_WIDTH; x++) {
		for (int y = 0; y < OLED_HEIGTH; y++) {
			index = twoDimmensionsToOneIndex(x, y);
			if (cb[index] != ob[index]) {
				ssd1331_draw_point(x, y, cb[index]);
			}
		}
	}

	switchBuffers();
	resetBuffer(currentFrameBuffer);
	displayTranslateOrigin(0, 0);
}

void displayDrawLine(int posXStart, int posYStart, int posXEnd, int posYEnd,
		uint16_t color) {

	posXStart += centerX;
	posYStart += centerY;
	posXEnd += centerX;
	posYEnd += centerY;

	int xMin, xMax, yMin, yMax = 0;

	if (posXStart >= posXEnd) {
		xMin = posXEnd;
		xMax = posXStart;
	} else {
		xMax = posXEnd;
		xMin = posXStart;
	}

	if (posYStart >= posYEnd) {
		yMin = posYEnd;
		yMax = posYStart;
	} else {
		yMax = posYEnd;
		yMin = posYStart;
	}

	int dx = posXEnd - posXStart;
	int dy = posYEnd - posYStart;

	if (dx == 0 && dy == 0) {
		return;
	}

	if (dx == 0) {

		if (posXStart < 0 && posXStart > OLED_WIDTH) {
			return;
		}

		for (int y = 0; y < OLED_HEIGTH; y++) {
			if (y >= yMin && y <= yMax) {
				displayDrawPoint(posXStart, y, color);
			}
		}

		return;
	} else if (dy == 0) {

		if (posYStart < 0 && posYStart > OLED_HEIGTH) {
			return;
		}

		for (int x = 0; x < OLED_WIDTH; x++) {
			if (x >= xMin && x <= xMax) {
				displayDrawPoint(x, posYStart, color);
			}
		}

		return;
	}

	float slope = 0;
	slope = (dy + 0.0f) / dx;

	float offset = posYStart - (slope * posXStart);

	if (abs(dx) >= abs(dy)) {
		int y = 0;
		for (int x = 0; x < OLED_WIDTH; x++) {

			if (x >= xMin && x <= xMax) {
				y = round(slope * x + offset);

				if (y >= 0 && y < OLED_HEIGTH && y >= yMin && y <= yMax) {
					displayDrawPoint(x, y, color);
				}
			}
		}
	} else {
		int x = 0;
		for (int y = 0; y < OLED_HEIGTH; y++) {

			if (y >= yMin && y <= yMax) {
				if (slope != 0) {
					x = round((y - offset) / slope);
				} else {
					if (posXStart >= posXEnd) {
						x = posXStart;
					} else {
						x = posXEnd;
					}
				}

				if (x >= 0 && x < OLED_WIDTH && x >= xMin && x <= xMax) {
					displayDrawPoint(x, y, color);
				}
			}
		}
	}
}

void displayTranslateOrigin(int x, int y) {
	centerX = x;
	centerY = y;
}

void displayDrawPoint(int posX, int posY, uint16_t hwColor) {
	if (posX >= OLED_WIDTH || posY >= OLED_HEIGTH) {
		return;
	}

	uint16_t *cb = getCurrentBuffer();
	cb[twoDimmensionsToOneIndex(posX, posY)] = hwColor;
}

void displayDrawChar(int posXStart, int posYStart, int chChr, int chSize,
		uint16_t hwColor) {

	uint8_t i, j, chTemp;
	uint8_t posYStart0 = posYStart;

	if (posXStart >= OLED_WIDTH || posYStart >= OLED_HEIGTH) {
		return;
	}

	for (i = 0; i < chSize; i++) {
		if (FONT_1206 == chSize) {
			chTemp = c_chFont1206[chChr - 0x20][i];
		} else if (FONT_1608 == chSize) {
			chTemp = c_chFont1608[chChr - 0x20][i];
		}

		for (j = 0; j < 8; j++) {
			if (chTemp & 0x80) {
				displayDrawPoint(posXStart, posYStart, hwColor);
			}
			chTemp <<= 1;
			posYStart++;
			if ((posYStart - posYStart0) == chSize) {
				posYStart = posYStart0;
				posXStart++;
				break;
			}
		}
	}
}

void displayDrawString(int posXStart, int posYStart, const char *pchString,
		int chSize, uint16_t hwColor) {

	if (posXStart >= OLED_WIDTH || posYStart >= OLED_HEIGTH) {
		return;
	}

	while (*pchString != '\0') {
		if (posXStart > (OLED_WIDTH - chSize / 2)) {
			posXStart = 0;
			posYStart += chSize;
			if (posYStart > (OLED_HEIGTH - chSize)) {
				posYStart = posXStart = 0;
			}
		}

		displayDrawChar(posXStart, posYStart, *pchString, chSize, hwColor);
		posXStart += chSize / 2;
		pchString++;
	}
}

void displayDrawCircle(int posXStart, int posYStart, int r, uint16_t hwColor) {
	posXStart += centerX;
	posYStart += centerY;

	int x = -r, y = 0, err = 2 - 2 * r, e2;

	    do {
	    	displayDrawPoint(posXStart - x, posYStart + y, hwColor);
	    	displayDrawPoint(posXStart + x, posYStart + y, hwColor);
	    	displayDrawPoint(posXStart + x, posYStart - y, hwColor);
	    	displayDrawPoint(posXStart - x, posYStart - y, hwColor);
	        e2 = err;
	        if (e2 <= y) {
	            err += ++ y * 2 + 1;
	            if(-x == y && e2 <= x) e2 = 0;
	        }
	        if(e2 > x) err += ++ x * 2 + 1;
	    } while(x <= 0);
}

void displayDrawRect(int posXStart, int posYStart, int posXEnd, int posYEnd,
		uint16_t hwColor) {
	posXStart += centerX;
	posYStart += centerY;
	posXEnd += centerX;
	posYEnd += centerY;

	displayDrawLine(posXStart, posYStart, posXStart, posXEnd, hwColor);
	displayDrawLine(posXEnd, posYStart, posXEnd, posXEnd, hwColor);
	displayDrawLine(posXStart, posYStart, posXEnd, posYStart, hwColor);
	displayDrawLine(posXStart, posYEnd, posXEnd, posYEnd, hwColor);
}
