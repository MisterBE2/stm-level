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
	displayTranslateOrigin(0,0);
}

void displayDrawLine(int posXStart, int posYStart, int posXEnd, int posYEnd,
		uint16_t color) {
	int xMin, xMax, yMin, yMax = 0;

	posXStart += centerX;
	posYStart += centerY;
	posXEnd += centerX;
	posYEnd += centerY;

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

	uint16_t *cb = getCurrentBuffer();

	if (dx == 0) {

		if (posXStart < 0 && posXStart > OLED_WIDTH) {
			return;
		}

		for (int y = 0; y < OLED_HEIGTH; y++) {
			if (y >= yMin && y <= yMax) {
				cb[twoDimmensionsToOneIndex(posXStart, y)] = color;
			}
		}

		return;
	} else if (dy == 0) {

		if (posYStart < 0 && posYStart > OLED_HEIGTH) {
			return;
		}

		for (int x = 0; x < OLED_WIDTH; x++) {
			if (x >= xMin && x <= xMax) {
				cb[twoDimmensionsToOneIndex(x, posYStart)] = color;
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
				y = slope * x + offset;

				if (y >= 0 && y < OLED_HEIGTH && y >= yMin && y <= yMax) {
					cb[twoDimmensionsToOneIndex(x, y)] = color;
				}
			}
		}
	} else {
		int x = 0;
		for (int y = 0; y < OLED_HEIGTH; y++) {

			if (y >= yMin && y <= yMax) {
				if (slope != 0) {
					x = (y - offset) / slope;
				} else {
					if (posXStart >= posXEnd) {
						x = posXStart;
					} else {
						x = posXEnd;
					}
				}

				if (x >= 0 && x < OLED_WIDTH && x >= xMin && x <= xMax) {
					cb[twoDimmensionsToOneIndex(x, y)] = color;
				}
			}
		}
	}
}

void displayTranslateOrigin(int x, int y) {
	centerX = x;
	centerY = y;
}
