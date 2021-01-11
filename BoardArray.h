#ifndef BOARD_ARRAY_H
#define BOARD_ARRAY_H

#include "RenjuGroup.h"

#include <QPoint>
#include <QMutex>

#include <vector>
using std::vector;

class BoardArray
{
public:
    BoardArray();
    ~BoardArray();

public:
    int rowSize() { return rowSize_; }
    int colSize() { return colSize_; }

    vector<QPoint> getBlackPositions();
    vector<QPoint> getWhitePositions();
    vector<QPoint> getEmptyPositions();

    bool setBlackAtPosition(QPoint position);
    bool setWhiteAtPosition(QPoint position);
    bool setEmptyAtPosition(QPoint position);

    bool isBlackAtPosition(QPoint position);
    bool isWhiteAtPosition(QPoint position);
    bool isEmptyAtPosition(QPoint position);

    void assignToNativeArray(int nativeArray[15][15]);

    void printBoardArray();

    Side getWinnerFromBoardArray();

private:
    enum class ItemStatus : int {
        EMPTY = 0,
        BLACK = -1,
        WHITE = 1
    };

private:
    bool isPositionValid(QPoint position);
    vector<QPoint> getStatusPositions(ItemStatus status);
    void setStatusAtPositionWithoutCheck(ItemStatus status, QPoint position);
    bool isStatusAtPositionWithoutCheck(ItemStatus status, QPoint position);

private:
    static const int rowSize_ = 15;
    static const int colSize_ = 15;
    vector<ItemStatus> boardArray;
    RenjuGroupList* list;

    QMutex setPositionMutex;
};

#endif // BOARD_ARRAY_H
