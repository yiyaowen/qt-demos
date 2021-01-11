#include "BoardArray.h"

#include <cstdlib>
#include <QtDebug>

BoardArray::BoardArray()
{
    boardArray.assign(rowSize_*colSize_, ItemStatus::EMPTY);
    list = getNewRenjuGroupList();
}

BoardArray::~BoardArray()
{
    releaseRenjuGroupList(&list);
}

bool BoardArray::isPositionValid(QPoint position)
{
    return position.x() >= 0 && position.x() < rowSize_ &&
        position.y() >= 0 && position.y() < colSize_ &&
        boardArray[position.x()*colSize_+position.y()] == ItemStatus::EMPTY;
}

void BoardArray::setStatusAtPositionWithoutCheck(ItemStatus status, QPoint position)
{
    setPositionMutex.lock();
    boardArray[position.x()*colSize_+position.y()] = status;
    Side side = status == ItemStatus::BLACK ? SIDE_BLACK : SIDE_WHITE;
    Point point = makePoint(position.x(), position.y());
    addNewMoveToRenjuGroupList(side, point, list);
    setPositionMutex.unlock();
    qDebug() << "setStatusAtPositionWithoutCheck";
    printBoardArray();
}

vector<QPoint> BoardArray::getStatusPositions(ItemStatus status)
{
    vector<QPoint> statusPositions;
    for (int i = 0; i < static_cast<int>(boardArray.size()); ++i) {
        if (boardArray[i] == status) {
            statusPositions.push_back(QPoint(i/colSize_, i%colSize_));
        }
    }
    return statusPositions;
}

vector<QPoint> BoardArray::getBlackPositions()
{
    return getStatusPositions(ItemStatus::BLACK);
}

vector<QPoint> BoardArray::getWhitePositions()
{
    return getStatusPositions(ItemStatus::WHITE);
}

vector<QPoint> BoardArray::getEmptyPositions()
{
    return getStatusPositions(ItemStatus::EMPTY);
}

bool BoardArray::setBlackAtPosition(QPoint position)
{
    if (isPositionValid(position)) {
        setStatusAtPositionWithoutCheck(ItemStatus::BLACK, position);
        return true;
    }
    return false;
}

bool BoardArray::setWhiteAtPosition(QPoint position)
{
    if (isPositionValid(position)) {
        setStatusAtPositionWithoutCheck(ItemStatus::WHITE, position);
        return true;
    }
    return false;
}

bool BoardArray::setEmptyAtPosition(QPoint position)
{
    if (isPositionValid(position)) {
        setStatusAtPositionWithoutCheck(ItemStatus::EMPTY, position);
        return true;
    }
    return false;
}

bool BoardArray::isStatusAtPositionWithoutCheck(ItemStatus status, QPoint position)
{
    return boardArray[position.x()*colSize_+position.y()] == status;
}

bool BoardArray::isBlackAtPosition(QPoint position)
{
    return isPositionValid(position) &&
        isStatusAtPositionWithoutCheck(ItemStatus::BLACK, position);
}

bool BoardArray::isWhiteAtPosition(QPoint position)
{
    return isPositionValid(position) &&
        isStatusAtPositionWithoutCheck(ItemStatus::WHITE, position);
}

bool BoardArray::isEmptyAtPosition(QPoint position)
{
    return isPositionValid(position) && 
        isStatusAtPositionWithoutCheck(ItemStatus::EMPTY, position);
}

void BoardArray::assignToNativeArray(int nativeArray[15][15])
{
    for (int i = 0; i < static_cast<int>(boardArray.size()); ++i) {
        nativeArray[i/colSize_][i%colSize_] = static_cast<int>(boardArray[i]);
    }
}

void BoardArray::printBoardArray()
{
    for (int i = 0; i < rowSize_; ++i) {
        QString tmpString;
        for (int j = 0; j < colSize_; ++j) {
            tmpString.append(QString(j%colSize_ == colSize_-1 ? "%1" : "%1, ").arg(static_cast<int>(boardArray[i*colSize_+j])));
        }
        qDebug() << tmpString;
    }
    qDebug() << "";
}

Side BoardArray::getWinnerFromBoardArray()
{
    return getWinnerInRenjuGroupList(list);
}
