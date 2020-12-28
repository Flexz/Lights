/*
 * colors.h
 *
 *  Created on: 24 дек. 2020 г.
 *      Author: Flexz
 */

#ifndef COLORS_H_
#define COLORS_H_

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

hsv rgb2hsv(rgb in);
rgb hsv2rgb(hsv in);

#endif /* COLORS_H_ */
