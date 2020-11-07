#define OLED_WIDTH 96
#define OLED_HEIGHT 64

#include "display/display.h"
#include <string.h>

void displayInit() {
	ssd1331_init();

	bgColor = BLACK;
	currentFrameBuffer = A;
	lastFrameBuffer = B;
}

void resetBuffer(enum db buffer) {
	if (buffer == A) {
		memset(bufferA, 0, 6144);
	} else if (buffer == B) {
		memset(bufferB, 0, 6144);
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

uint8_t* getCurrentBuffer() {
	if (currentFrameBuffer == A) {
		return bufferA;
	} else {
		return bufferB;
	}
}

uint16_t twoDimmensionsToOneIndex(uint8_t posX, uint8_t posY) {
	return posY * OLED_WIDTH + posX ;
}

void displayUpdate() {
	uint8_t tmpPixel = 0;
	for(int x = 0; x < OLED_WIDTH; x++) {
		for(int y = 0; y < OLED_HEIGHT; y++) {
			if(currentFrameBuffer == A) {
				ssd1331_draw_point(x, y, RED);
			} else {
				ssd1331_draw_point(x, y, WHITE);
			}
		}
	}

	switchBuffers();
	resetBuffer(currentFrameBuffer);
}
