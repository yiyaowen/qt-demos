#ifndef RENJU_GROUP_H
#define RENJU_GROUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "Point/Point.h"
#include "Pair/IntPair.h"

typedef int Side;
#define SIDE_BLACK  (-1)
#define SIDE_EMPTY  0
#define SIDE_WHITE  1

#define isBlackSide(SIDE) ((SIDE) == SIDE_BLACK)

#define maxInt(a, b) ((a) > (b) ? (a) : (b))
#define minInt(a, b) ((a) < (b) ? (a) : (b))

// Renju
typedef struct type_Renju {
    int maxPieceCount;
    int blackPieceCount;
    int whitePieceCount;
    int pureBlackCount;
    bool isBlackWin;
    bool isWhiteWin;
} Renju;

typedef struct type_RenjuRange {
    bool isValid;
    Renju** start;
    int offset;
} RenjuRange;

Renju* getNewRenju();

void increaseRenjuRangeBlackPieceCount(RenjuRange range);
void increaseRenjuRangeWhitePieceCount(RenjuRange range);

// Renju Group
typedef struct type_RenjuGroup {
    int type;
    char* name;
    int renjuCount;
    Renju** renjus;
} RenjuGroup;

#define REGR_TYPE_ROW       1
#define REGR_TYPE_COLUMN    2
#define REGR_TYPE_BDIAGONAL 3
#define REGR_TYPE_FDIAGONAL 4

RenjuGroup* getNewRenjuGroup();
RenjuGroup* getNewRowRenjuGroup();
RenjuGroup* getNewColumnRenjuGroup();
RenjuGroup* getNewBDiagonalRenjuGroup();
RenjuGroup* getNewFDiagonalRenjuGroup();

void allocRenjuGroupNewRenjusWithCount(RenjuGroup* renjuGroup, int count);

IntPair getRenjuIndexRangeInRowRenjuGroupOfPoint(Point point);
IntPair getRenjuIndexRangeInColumnRenjuGroupOfPoint(Point point);
IntPair getRenjuIndexRangeInBDiagonalRenjuGroupOfPoint(Point point);
IntPair getRenjuIndexRangeInFDiagonalRenjuGroupOfPoint(Point point);

RenjuRange getRenjuRangeInRowRenjuGroupOfPoint(RenjuGroup* group, Point point);
RenjuRange getRenjuRangeInColumnRenjuGroupOfPoint(RenjuGroup* group, Point point);
RenjuRange getRenjuRangeInBDiagonalRenjuGroupOfPoint(RenjuGroup* group, Point point);
RenjuRange getRenjuRangeInFDiagonalRenjuGroupOfPoint(RenjuGroup* group, Point point);

bool isRowRenjuIndexRangeValid(int beginIndex, int endIndex);
#define isColumnRenjuIndexRangeValid isRowRenjuIndexRangeValid
bool isBDiagonalRenjuIndexRangeValid(int beginIndex, int endIndex);
#define isFDiagonalRenjuIndexRangeValid isBDiagonalRenjuIndexRangeValid

// Renju Group List
typedef struct type_RenjuGroupList {
    int renjuGroupCount;
    RenjuGroup** renjuGroups;
} RenjuGroupList;

RenjuGroupList* getNewRenjuGroupList();

#define getRowRenjuGroupInRenjuGroupList(LIST)          ((LIST)->renjuGroups[0])
#define getColumnRenjuGroupInRenjuGroupList(LIST)       ((LIST)->renjuGroups[1])
#define getBDiagonalRenjuGroupInRenjuGroupList(LIST)    ((LIST)->renjuGroups[2])
#define getFDiagonalRenjuGroupInRenjuGroupList(LIST)    ((LIST)->renjuGroups[3])

void addBlackPieceNewPointToRenjuGroupList(Point point, RenjuGroupList* list);
void addWhitePieceNewPointToRenjuGroupList(Point point, RenjuGroupList* list);

void addNewMoveToRenjuGroupList(Side moveSide, Point movePoint, RenjuGroupList* list);

Side getWinnerInRenjuGroupList(RenjuGroupList* list);

// Release Memory
void releaseRenju(Renju** renju);
void releaseRenjuGroup(RenjuGroup** group);
void releaseRenjuGroupList(RenjuGroupList** list);

#ifdef __cplusplus
}
#endif

#endif // RENJU_GROUP_H
