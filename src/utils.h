#pragma once

#ifndef UTILS_H
#define UTILS_H

#define ARR_LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))
#define ARR2D(ARR, ROWS, I, J) ((I) * (ROWS) + J)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#endif
