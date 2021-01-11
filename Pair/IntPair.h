#ifndef INT_PAIR_H
#define INT_PAIR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct type_IntPair {
    int first;
    int second;
} IntPair;

IntPair makeIntPair(int first, int second);

#ifdef __cplusplus
}
#endif

#endif // INT_PAIR_H
