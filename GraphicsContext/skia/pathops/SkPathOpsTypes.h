/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkPathOpsTypes_DEFINED
#define SkPathOpsTypes_DEFINED

#include <float.h>  // for FLT_EPSILON
#include <math.h>   // for fabs, sqrt

#include "SkFloatingPoint.h"
#include "SkPath.h"
#include "SkPathOps.h"
#include "SkPathOpsDebug.h"
#include "SkScalar.h"

enum SkPathOpsMask {
    kWinding_PathOpsMask = -1,
    kNo_PathOpsMask = 0,
    kEvenOdd_PathOpsMask = 1
};

// Use Almost Equal when comparing coordinates. Use epsilon to compare T values.
bool AlmostEqualUlps(float a, float b);
inline bool AlmostEqualUlps(double a, double b) {
    return AlmostEqualUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

// Use Almost Dequal when comparing should not special case denormalized values.
bool AlmostDequalUlps(float a, float b);
inline bool AlmostDequalUlps(double a, double b) {
    return AlmostDequalUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

bool NotAlmostEqualUlps(float a, float b);
inline bool NotAlmostEqualUlps(double a, double b) {
    return NotAlmostEqualUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

bool NotAlmostDequalUlps(float a, float b);
inline bool NotAlmostDequalUlps(double a, double b) {
    return NotAlmostDequalUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

// Use Almost Bequal when comparing coordinates in conjunction with between.
bool AlmostBequalUlps(float a, float b);
inline bool AlmostBequalUlps(double a, double b) {
    return AlmostBequalUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

bool AlmostPequalUlps(float a, float b);
inline bool AlmostPequalUlps(double a, double b) {
    return AlmostPequalUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

bool RoughlyEqualUlps(float a, float b);
inline bool RoughlyEqualUlps(double a, double b) {
    return RoughlyEqualUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

bool AlmostLessUlps(float a, float b);
inline bool AlmostLessUlps(double a, double b) {
    return AlmostLessUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

bool AlmostLessOrEqualUlps(float a, float b);
inline bool AlmostLessOrEqualUlps(double a, double b) {
    return AlmostLessOrEqualUlps(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

bool AlmostBetweenUlps(float a, float b, float c);
inline bool AlmostBetweenUlps(double a, double b, double c) {
    return AlmostBetweenUlps(SkDoubleToScalar(a), SkDoubleToScalar(b), SkDoubleToScalar(c));
}

int UlpsDistance(float a, float b);
inline int UlpsDistance(double a, double b) {
    return UlpsDistance(SkDoubleToScalar(a), SkDoubleToScalar(b));
}

// FLT_EPSILON == 1.19209290E-07 == 1 / (2 ^ 23)
// DBL_EPSILON == 2.22045e-16
const double FLT_EPSILON_CUBED = FLT_EPSILON * FLT_EPSILON * FLT_EPSILON;
const double FLT_EPSILON_HALF = FLT_EPSILON / 2;
const double FLT_EPSILON_DOUBLE = FLT_EPSILON * 2;
const double FLT_EPSILON_SQUARED = FLT_EPSILON * FLT_EPSILON;
const double FLT_EPSILON_SQRT = sqrt(FLT_EPSILON);
const double FLT_EPSILON_INVERSE = 1 / FLT_EPSILON;
const double DBL_EPSILON_ERR = DBL_EPSILON * 4;  // FIXME: tune -- allow a few bits of error
const double DBL_EPSILON_SUBDIVIDE_ERR = DBL_EPSILON * 16;
const double ROUGH_EPSILON = FLT_EPSILON * 64;
const double MORE_ROUGH_EPSILON = FLT_EPSILON * 256;

inline bool approximately_zero(double x) {
    return fabs(x) < FLT_EPSILON;
}

inline bool precisely_zero(double x) {
    return fabs(x) < DBL_EPSILON_ERR;
}

inline bool precisely_subdivide_zero(double x) {
    return fabs(x) < DBL_EPSILON_SUBDIVIDE_ERR;
}

inline bool approximately_zero(float x) {
    return fabs(x) < FLT_EPSILON;
}

inline bool approximately_zero_cubed(double x) {
    return fabs(x) < FLT_EPSILON_CUBED;
}

inline bool approximately_zero_half(double x) {
    return fabs(x) < FLT_EPSILON_HALF;
}

inline bool approximately_zero_double(double x) {
    return fabs(x) < FLT_EPSILON_DOUBLE;
}

inline bool approximately_zero_squared(double x) {
    return fabs(x) < FLT_EPSILON_SQUARED;
}

inline bool approximately_zero_sqrt(double x) {
    return fabs(x) < FLT_EPSILON_SQRT;
}

inline bool roughly_zero(double x) {
    return fabs(x) < ROUGH_EPSILON;
}

inline bool approximately_zero_inverse(double x) {
    return fabs(x) > FLT_EPSILON_INVERSE;
}

// OPTIMIZATION: if called multiple times with the same denom, we want to pass 1/y instead
inline bool approximately_zero_when_compared_to(double x, double y) {
    return x == 0 || fabs(x / y) < FLT_EPSILON;
}

// Use this for comparing Ts in the range of 0 to 1. For general numbers (larger and smaller) use
// AlmostEqualUlps instead.
inline bool approximately_equal(double x, double y) {
    return approximately_zero(x - y);
}

inline bool precisely_equal(double x, double y) {
    return precisely_zero(x - y);
}

inline bool precisely_subdivide_equal(double x, double y) {
    return precisely_subdivide_zero(x - y);
}

inline bool approximately_equal_half(double x, double y) {
    return approximately_zero_half(x - y);
}

inline bool approximately_equal_double(double x, double y) {
    return approximately_zero_double(x - y);
}

inline bool approximately_equal_squared(double x, double y) {
    return approximately_equal(x, y);
}

inline bool approximately_greater(double x, double y) {
    return x - FLT_EPSILON >= y;
}

inline bool approximately_greater_or_equal(double x, double y) {
    return x + FLT_EPSILON > y;
}

inline bool approximately_lesser(double x, double y) {
    return x + FLT_EPSILON <= y;
}

inline bool approximately_lesser_or_equal(double x, double y) {
    return x - FLT_EPSILON < y;
}

inline bool approximately_greater_than_one(double x) {
    return x > 1 - FLT_EPSILON;
}

inline bool precisely_greater_than_one(double x) {
    return x > 1 - DBL_EPSILON_ERR;
}

inline bool approximately_less_than_zero(double x) {
    return x < FLT_EPSILON;
}

inline bool precisely_less_than_zero(double x) {
    return x < DBL_EPSILON_ERR;
}

inline bool approximately_negative(double x) {
    return x < FLT_EPSILON;
}

inline bool precisely_negative(double x) {
    return x < DBL_EPSILON_ERR;
}

inline bool approximately_one_or_less(double x) {
    return x < 1 + FLT_EPSILON;
}

inline bool approximately_positive(double x) {
    return x > -FLT_EPSILON;
}

inline bool approximately_positive_squared(double x) {
    return x > -(FLT_EPSILON_SQUARED);
}

inline bool approximately_zero_or_more(double x) {
    return x > -FLT_EPSILON;
}

inline bool approximately_between(double a, double b, double c) {
    return a <= c ? approximately_negative(a - b) && approximately_negative(b - c)
            : approximately_negative(b - a) && approximately_negative(c - b);
}

inline bool precisely_between(double a, double b, double c) {
    return a <= c ? precisely_negative(a - b) && precisely_negative(b - c)
            : precisely_negative(b - a) && precisely_negative(c - b);
}

// returns true if (a <= b <= c) || (a >= b >= c)
inline bool between(double a, double b, double c) {
    return (a - b) * (c - b) <= 0;
}

inline bool more_roughly_equal(double x, double y) {
    return fabs(x - y) < MORE_ROUGH_EPSILON;
}

inline bool roughly_equal(double x, double y) {
    return fabs(x - y) < ROUGH_EPSILON;
}

struct SkDPoint;
struct SkDVector;
struct SkDLine;
struct SkDQuad;
struct SkDTriangle;
struct SkDCubic;
struct SkDRect;

inline SkPath::Verb SkPathOpsPointsToVerb(int points) {
    int verb = (1 << points) >> 1;
    return (SkPath::Verb)verb;
}

inline int SkPathOpsVerbToPoints(SkPath::Verb verb) {
    int points = (int) verb - ((int) verb >> 2);
    return points;
}

inline double SkDInterp(double A, double B, double t) {
    return A + (B - A) * t;
}

double SkDCubeRoot(double x);

/* Returns -1 if negative, 0 if zero, 1 if positive
*/
inline int SkDSign(double x) {
    return (x > 0) - (x < 0);
}

/* Returns 0 if negative, 1 if zero, 2 if positive
*/
inline int SKDSide(double x) {
    return (x > 0) + (x >= 0);
}

/* Returns 1 if negative, 2 if zero, 4 if positive
*/
inline int SkDSideBit(double x) {
    return 1 << SKDSide(x);
}

inline double SkPinT(double t) {
    return precisely_less_than_zero(t) ? 0 : precisely_greater_than_one(t) ? 1 : t;
}

#ifdef SK_DEBUG
inline void DebugDumpDouble(double x) {
    if (x == floor(x)) {
        SkDebugf("%.0f", x);
    } else {
        SkDebugf("%1.17g", x);
    }
}

inline void DebugDumpFloat(float x) {
    if (x == floorf(x)) {
        SkDebugf("%.0f", x);
    } else {
        SkDebugf("%1.9gf", x);
    }
}
#endif

#endif
