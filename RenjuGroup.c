#include <stdlib.h>

#include "RenjuGroup.h"

Renju* getNewRenju()
{
    Renju* renju = (Renju*)malloc(sizeof(Renju));
    renju->maxPieceCount = 5;
    renju->blackPieceCount = renju->whitePieceCount = 0;
    renju->pureBlackCount = 0;
    renju->isBlackWin = renju->isWhiteWin = false;
    return renju;
}

void increaseRenjuRangeBlackPieceCount(RenjuRange range)
{
    if (!range.isValid) return;
    for (int i = 0; i <= range.offset; ++i) {
        if ((*(range.start+i))->blackPieceCount < (*(range.start+i))->maxPieceCount) {
            ++((*(range.start+i))->pureBlackCount);
            if (++((*(range.start+i))->blackPieceCount) == (*(range.start+i))->maxPieceCount)
            {
                (*(range.start+i))->isBlackWin = true;
                (*(range.start+i))->isWhiteWin = false;
            }
        }
    }
}

void increaseRenjuRangeWhitePieceCount(RenjuRange range)
{
    if (!range.isValid) return;
    for (int i = 0; i <= range.offset; ++i) {
        if ((*(range.start+i))->whitePieceCount < (*(range.start+i))->maxPieceCount) {
            (*(range.start+i))->pureBlackCount = -10;
            if (++((*(range.start+i))->whitePieceCount) == (*(range.start+i))->maxPieceCount)
            {
                (*(range.start+i))->isWhiteWin = true;
                (*(range.start+i))->isBlackWin = false;
            }
        }
    }
}

RenjuGroup* getNewRenjuGroup()
{
    RenjuGroup* renjuGroup = (RenjuGroup*)malloc(sizeof(RenjuGroup));
    return renjuGroup;
}

RenjuGroup* getNewRowRenjuGroup()
{
    RenjuGroup* rowRenjuGroup = getNewRenjuGroup();
    rowRenjuGroup->type = REGR_TYPE_ROW;
    rowRenjuGroup->name = "Row Renju Group";
    allocRenjuGroupNewRenjusWithCount(rowRenjuGroup, 165);
    return rowRenjuGroup;
}

RenjuGroup* getNewColumnRenjuGroup()
{
    RenjuGroup* columnRenjuGroup = getNewRenjuGroup();
    columnRenjuGroup->type = REGR_TYPE_COLUMN;
    columnRenjuGroup->name = "Column Renju Group";
    allocRenjuGroupNewRenjusWithCount(columnRenjuGroup, 165);
    return columnRenjuGroup;
}

RenjuGroup* getNewBDiagonalRenjuGroup()
{
    RenjuGroup* bDiagonalRenjuGroup = getNewRenjuGroup();
    bDiagonalRenjuGroup->type = REGR_TYPE_BDIAGONAL;
    bDiagonalRenjuGroup->name = "B-Diagonal Renju Group";
    allocRenjuGroupNewRenjusWithCount(bDiagonalRenjuGroup, 121);
    return bDiagonalRenjuGroup;
}

RenjuGroup* getNewFDiagonalRenjuGroup()
{
    RenjuGroup* fDiagonalRenjuGroup = getNewRenjuGroup();
    fDiagonalRenjuGroup->type = REGR_TYPE_FDIAGONAL;
    fDiagonalRenjuGroup->name = "F-Diagonal Renju Group";
    allocRenjuGroupNewRenjusWithCount(fDiagonalRenjuGroup, 121);
    return fDiagonalRenjuGroup;
}

void allocRenjuGroupNewRenjusWithCount(RenjuGroup* group, int count)
{
    group->renjuCount = count;
    group->renjus = (Renju**)malloc(sizeof(Renju*)*count);
    for (int i = 0; i < count; ++i) {
        (group->renjus)[i] = getNewRenju();
    }
}

IntPair getRenjuIndexRangeInRowRenjuGroupOfPoint(Point point)
{
    return makeIntPair(point.i * 11 + maxInt(point.j - 4, 0), point.i * 11 + minInt(point.j, 10));
}

IntPair getRenjuIndexRangeInColumnRenjuGroupOfPoint(Point point)
{
    Point symmetryPoint = { point.j, point.i };
    return getRenjuIndexRangeInRowRenjuGroupOfPoint(symmetryPoint);
}

IntPair getRenjuIndexRangeInBDiagonalRenjuGroupOfPoint(Point point)
{
    static bool isSymmetricPoint = false;
    if (point.i + point.j > 14) {
        Point symmetryPoint = { 14 - point.i, 14 - point.j };
        isSymmetricPoint = true;
        return getRenjuIndexRangeInBDiagonalRenjuGroupOfPoint(symmetryPoint);
    }

    int diagonalOffset = point.i + point.j - 4;
    int startOffset = diagonalOffset * (diagonalOffset + 1) / 2;
    int beginIndex = startOffset + maxInt(point.j - 4, 0);
    int endIndex = startOffset + minInt(point.j, point.i + point.j - 4);
    if (isSymmetricPoint) {
        int tmpIndex = beginIndex;
        beginIndex = 120 - endIndex;
        endIndex = 120 - tmpIndex;
    }

    isSymmetricPoint = false;
    return makeIntPair(beginIndex, endIndex);
}

IntPair getRenjuIndexRangeInFDiagonalRenjuGroupOfPoint(Point point)
{
    Point symmetryPoint = { point.i, 14 - point.j };
    return getRenjuIndexRangeInBDiagonalRenjuGroupOfPoint(symmetryPoint);
}

RenjuRange getRenjuRangeInRowRenjuGroupOfPoint(RenjuGroup* group, Point point)
{
    IntPair indexRange = getRenjuIndexRangeInRowRenjuGroupOfPoint(point);
    int beginIndex = indexRange.first;
    int endIndex = indexRange.second;

    RenjuRange range;
    if (!(range.isValid = isRowRenjuIndexRangeValid(beginIndex, endIndex))) return range;
    range.start = group->renjus + beginIndex;
    range.offset = endIndex - beginIndex;
    return range;
}

RenjuRange getRenjuRangeInColumnRenjuGroupOfPoint(RenjuGroup* group, Point point)
{ 
    Point symmetryPoint = { point.j, point.i };
    return getRenjuRangeInRowRenjuGroupOfPoint(group, symmetryPoint);
}

RenjuRange getRenjuRangeInBDiagonalRenjuGroupOfPoint(RenjuGroup* group, Point point)
{
    IntPair indexRange = getRenjuIndexRangeInBDiagonalRenjuGroupOfPoint(point);
    int beginIndex = indexRange.first;
    int endIndex = indexRange.second;

    RenjuRange range;
    if (!(range.isValid = isBDiagonalRenjuIndexRangeValid(beginIndex, endIndex))) return range;
    range.start = group->renjus + beginIndex;
    range.offset = endIndex - beginIndex;
    return range;
}

RenjuRange getRenjuRangeInFDiagonalRenjuGroupOfPoint(RenjuGroup* group, Point point)
{
    Point symmetryPoint = { point.i, 14 - point.j };
    return getRenjuRangeInBDiagonalRenjuGroupOfPoint(group, symmetryPoint);
}

bool isRowRenjuIndexRangeValid(int beginIndex, int endIndex)
{
    return beginIndex <= endIndex && beginIndex >= 0 && endIndex < 165;
}

bool isBDiagonalRenjuIndexRangeValid(int beginIndex, int endIndex)
{
    return beginIndex <= endIndex && beginIndex >= 0 && endIndex < 121;
}

RenjuGroupList* getNewRenjuGroupList()
{
    RenjuGroupList* renjuGroupList = (RenjuGroupList*)malloc(sizeof(RenjuGroupList));
    renjuGroupList->renjuGroupCount = 4;
    renjuGroupList->renjuGroups = (RenjuGroup**)malloc(sizeof(RenjuGroup*)*4);
    renjuGroupList->renjuGroups[0] = getNewRowRenjuGroup();
    renjuGroupList->renjuGroups[1] = getNewColumnRenjuGroup();
    renjuGroupList->renjuGroups[2] = getNewBDiagonalRenjuGroup();
    renjuGroupList->renjuGroups[3] = getNewFDiagonalRenjuGroup();
    return renjuGroupList;
}

void addBlackPieceNewPointToRenjuGroupList(Point point, RenjuGroupList* list)
{
    increaseRenjuRangeBlackPieceCount(
        getRenjuRangeInRowRenjuGroupOfPoint(
            getRowRenjuGroupInRenjuGroupList(list), point
        )
    );
    increaseRenjuRangeBlackPieceCount(
        getRenjuRangeInColumnRenjuGroupOfPoint(
            getColumnRenjuGroupInRenjuGroupList(list), point
        )
    );
    increaseRenjuRangeBlackPieceCount(
        getRenjuRangeInBDiagonalRenjuGroupOfPoint(
            getBDiagonalRenjuGroupInRenjuGroupList(list), point
        )
    );
    increaseRenjuRangeBlackPieceCount(
        getRenjuRangeInFDiagonalRenjuGroupOfPoint(
            getFDiagonalRenjuGroupInRenjuGroupList(list), point
        )
    );
}

void addWhitePieceNewPointToRenjuGroupList(Point point, RenjuGroupList* list)
{
    increaseRenjuRangeWhitePieceCount(
        getRenjuRangeInRowRenjuGroupOfPoint(
            getRowRenjuGroupInRenjuGroupList(list), point
        )
    );
    increaseRenjuRangeWhitePieceCount(
        getRenjuRangeInColumnRenjuGroupOfPoint(
            getColumnRenjuGroupInRenjuGroupList(list), point
        )
    );
    increaseRenjuRangeWhitePieceCount(
        getRenjuRangeInBDiagonalRenjuGroupOfPoint(
            getBDiagonalRenjuGroupInRenjuGroupList(list), point
        )
    );
    increaseRenjuRangeWhitePieceCount(
        getRenjuRangeInFDiagonalRenjuGroupOfPoint(
            getFDiagonalRenjuGroupInRenjuGroupList(list), point
        )
    );
}

void addNewMoveToRenjuGroupList(Side moveSide, Point movePoint, RenjuGroupList* list)
{
    if (isBlackSide(moveSide))
        addBlackPieceNewPointToRenjuGroupList(movePoint, list);
    else
        addWhitePieceNewPointToRenjuGroupList(movePoint, list);
}

Side getWinnerInRenjuGroupList(RenjuGroupList* list)
{
    for (int i = 0; i < list->renjuGroupCount; ++i) {
        RenjuGroup* group = (list->renjuGroups)[i];
        for (int j = 0; j < group->renjuCount; ++j) {
            Renju* renju = (group->renjus)[j];
            if (renju->isBlackWin) return SIDE_BLACK;
            if (renju->isWhiteWin) return SIDE_WHITE;
        }
    }
    return SIDE_EMPTY;
}

void releaseRenju(Renju** renju)
{
    free(*renju);
    *renju = NULL;
}

void releaseRenjuGroup(RenjuGroup** group)
{
    for (int i = 0; i < (*group)->renjuCount; ++i) {
        releaseRenju(&(((*group)->renjus)[i]));
    }
    free(*group);
    *group = NULL;
}

void releaseRenjuGroupList(RenjuGroupList** list)
{
    for (int i = 0; i < (*list)->renjuGroupCount; ++i) {
        releaseRenjuGroup(&(((*list)->renjuGroups)[i]));
    }
    free(*list);
    *list = NULL;
}
