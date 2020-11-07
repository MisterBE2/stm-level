#include "SSD1331.h"

uint16_t bgColor;
uint8_t bufferA[6144];
uint8_t bufferB[6144];

enum db {
	A,
	B
};

enum db currentFrameBuffer;
enum db lastFrameBuffer;

void displayInit();
void resetBuffer(enum db buffer);
void switchBuffers();
uint8_t* getCurrentBuffer();
void displayUpdate();
uint16_t twoDimmensionsToOneIndex(uint8_t posX, uint8_t posY);
