#include "SSD1331.h"

#ifndef DISPLAY
#define DISPLAY
uint16_t bgColor;
uint16_t bufferA[6144];
uint16_t bufferB[6144];

uint8_t centerX, centerY;

enum db {
	A,
	B
};

enum db currentFrameBuffer;
enum db lastFrameBuffer;
#endif

void displayInit();
void resetBuffer(enum db buffer);
void switchBuffers();
uint16_t* getCurrentBuffer();
uint16_t* getSecondaryBuffer();
void displayUpdate();
int twoDimmensionsToOneIndex(int posX, int posY);

void displayDrawLine(int posXStart, int posYStart, int posXEnd, int posYEnd, uint16_t color);
void displayTranslateOrigin(int x, int y);
void displayDrawPoint(int posX, int posY, uint16_t hwColor);
void displayDrawChar(int posXStart, int posYStart, int chChr, int chSize, uint16_t hwColor);
void displayDrawString(int posXStart, int posYStart, const char *pchString, int chSize, uint16_t hwColor);
void displayDrawCircle(int posXStart, int posYStart, int r, uint16_t hwColor);
void displayDrawRect(int posXStart, int posYStart, int posXEnd, int posYEnd, uint16_t hwColor);
