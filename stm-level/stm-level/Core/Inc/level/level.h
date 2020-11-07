#include "../display/display.h"

#ifndef LEVEL_DEF
#define LEVEL_DEF
#define LEVEL_COLOR RGB(25, 255, 25)
#define NOLEVEL_COLOR RGB(255, 0, 0)
#define TAILS_AMMOUNT 5
#endif

void levelDrawUi(float radians);
void levelDrawStaticUi();
void levelDrawAngle(float radians, uint16_t color);
float levelRadiansToDegree(float radians);
float levelDegreeToRadians(float degree);
