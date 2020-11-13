#include "level/level.h"
#include <string.h>

#define LINE_LENGTH 60
#define CENTER_LINE_LENGTH 5

void levelDrawUi(float radians) {
	uint16_t tmpColor = 0;
	float originalRadians = radians;
	int sx, sy, ex, ey, vectorX, vectorY;

	float angle = levelRadiansToDegree(radians);

	if (abs(angle) < 2) {
		tmpColor = LEVEL_COLOR;
	} else {
		tmpColor = NOLEVEL_COLOR;
	}

	if (angle > 25) {
		radians = levelDegreeToRadians(25);
	} else if (angle < -25) {
		radians = levelDegreeToRadians(-25);
	}

	vectorX = LINE_LENGTH * cos(radians);
	vectorY = LINE_LENGTH * sin(radians);

	sx = -vectorX;
	sy = -vectorY;
	ex = vectorX;
	ey = vectorY;

	displayTranslateOrigin(48, 40);
	displayDrawLine(sx, sy, ex, ey, tmpColor);

	vectorX = 15 * cos(radians);
	vectorY = 15 * sin(radians);

	sx = -vectorX;
	sy = -vectorY;
	ex = vectorX;
	ey = vectorY;

	displayDrawLine(sx, sy, ex, ey, BLACK);
	displayDrawCircle(0, 0, 15, tmpColor);
	levelDrawAngle(originalRadians, WHITE);

	displayTranslateOrigin(0, 0);
}

void levelDrawAngle(float radians, uint16_t color) {
	int angle = levelRadiansToDegree(radians);
	int digits = 0;
	int offset = 0;

	if (abs(angle) < 9) {
		digits = 1;
		offset = 2;
	} else if (abs(angle) < 99) {
		digits = 2;
		offset = 5;
	} else if (abs(angle) < 999) {
		digits = 3;
		offset = 12;
	}

	if (angle < 0) {
		digits++;
		offset += 3;
	}

	char angleString[digits];
	sprintf(angleString, "%d", angle);

	displayDrawString(48-offset, 34, angleString, 12, color);
}

float levelRadiansToDegree(float radians) {
	return radians * (360 / 6.28f);
}

float levelDegreeToRadians(float degree) {
	return degree * (6.28f / 360);
}

void levelDrawStaticUi() {
	displayDrawString(0, 0, "digital level v1", 12, WHITE);
	displayDrawRect(0, 14, 95, 63, WHITE);
}
