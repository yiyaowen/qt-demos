#ifndef POINT_H
#define POINT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct type_Point {
    int i;
    int j;
} Point;

Point makePoint(int i, int j);

#define isPointEqual(POINT_A, POINT_B) ((POINT_A).i == (POINT_B).i && (POINT_A).j == (POINT_B).j)

#define isPointValueNegative(POINT) ((POINT).i < 0 || (POINT).j < 0)

#ifdef __cplusplus
}
#endif

#endif // POINT_H
