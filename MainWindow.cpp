#include <QPainter>
#include <QPoint>
#include <QImage>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <vector>
using std::vector;

#include "MainWindow.h"
extern "C"
{
    #include "Core.h"
    #include "CoreDefinitions.h"
    void initializeGobangCore();
    void releaseGobangCore();
    void createNewCoreGameWithTag(CoreGameTag tag, CoreSide favors);
    void addNewMovePieceToCoreGameWithTag(CoreSide side, CorePoint point, CoreGameTag tag);
    void removeCoreGameWithTag(CoreGameTag tag);
    CorePoint  getCorePointFromCoreAlgorithm(const CorePrefabConfig* config, CoreGameTag tag);
}

CorePoint convertDotToCorePoint(QPoint dot)
{
    CorePoint point = { dot.x(), dot.y() };
    return point;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    initializeGobangCore();
    initializeBoardArray();
    initializeLayout();
    bindSignalsAndSlots();

    this->setWindowTitle("Gobang Analyzer");

    this->setMouseTracking(true);
    this->centralWidget()->setMouseTracking(true);
    lblBoard->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeLayout()
{
    QWidget* centralWidget = new QWidget;
    this->setCentralWidget(centralWidget);
    
    lblBoard = new QLabel;
    lblBoard->setFixedSize(boardSize, boardSize);
    lblBoard->installEventFilter(this);

    btnSwitchToPvCWithPlayerBlack = new QPushButton("PvC Player Black");
    btnSwitchToPvCWithPlayerWhite = new QPushButton("PvC player White");
    btnSwitchToCvC = new QPushButton("Auto CvC");

    cbCvCIntervalTime = new QComboBox;
    cbCvCIntervalTime->insertItem(0, "No delay");
    for (int i = 1; i <= 5; ++i) {
        cbCvCIntervalTime->insertItem(i, QString("%1 s").arg(i));
    }

    QVBoxLayout* verticalLayout = new QVBoxLayout;
    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    verticalLayout->addWidget(lblBoard);
    horizontalLayout->addWidget(btnSwitchToPvCWithPlayerBlack);
    horizontalLayout->addWidget(btnSwitchToPvCWithPlayerWhite);
    horizontalLayout->addWidget(btnSwitchToCvC);
    horizontalLayout->addWidget(cbCvCIntervalTime);
    QWidget* horizontalLayoutWidget = new QWidget;
    horizontalLayoutWidget->setLayout(horizontalLayout);
    verticalLayout->addWidget(horizontalLayoutWidget);
    centralWidget->setLayout(verticalLayout);
}

void MainWindow::initializeBoardArray()
{
    boardArray = new BoardArray;
}

void MainWindow::bindSignalsAndSlots()
{
    connect(btnSwitchToPvCWithPlayerBlack, SIGNAL(clicked()), this, SLOT(slot_switchToPvCWithPlayerBlack()));
    connect(btnSwitchToPvCWithPlayerWhite, SIGNAL(clicked()), this, SLOT(slot_switchToPvCWithPlayerWhite()));
    connect(btnSwitchToCvC, SIGNAL(clicked()), this, SLOT(slot_switchToCvC()));
    connect(cbCvCIntervalTime, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_changeCvCIntervalTime(int)));

    connect(gameCvCTimer, SIGNAL(timeout()), this, SLOT(slot_movePieceByMachine()));
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == lblBoard && event->type() == QEvent::Paint) {

        QPainter painter(lblBoard);

        QImage newBoardImage(lblBoard->size(), QImage::Format_ARGB32);
        newBoardImage.fill(Qt::transparent);
        QPainter imagePainter(&newBoardImage);

        if (isWindowFirstShow) {
            imagePainter.drawImage(QPoint(0, 0), QImage(":/board.png"));
            oldBoardImage = newBoardImage;
            painter.drawImage(QPoint(0, 0), newBoardImage);
            isWindowFirstShow = false;
        }
        else {
            if (needRepaintPeices) {
                imagePainter.drawImage(QPoint(0, 0), QImage(":/board.png"));

                vector<QPoint> blackPositions = boardArray->getBlackPositions();
                vector<QPoint> whitePositions = boardArray->getWhitePositions();
                for (auto dot : blackPositions) {
                    QPoint pos = convertDotToPosition(dot);
                    imagePainter.drawImage(pos-QPoint(pieceSize/2,pieceSize/2), QImage(":/black.png"));
                }
                for (auto dot : whitePositions) {
                    QPoint pos = convertDotToPosition(dot);
                    imagePainter.drawImage(pos-QPoint(pieceSize/2,pieceSize/2), QImage(":/white.png"));
                }

                oldBoardImage = newBoardImage;
            }
            else {
                imagePainter.drawImage(QPoint(0, 0), oldBoardImage);
            }

            QPoint currentSelectedBlackPosition = convertDotToPosition(currentSelectedBlackDot);
            imagePainter.drawImage(currentSelectedBlackPosition-QPoint(selectFrameSize/2,selectFrameSize/2),
                QImage(":/select_red.png"));
            QPoint currentSelectedWhitePosition = convertDotToPosition(currentSelectedWhiteDot);
            imagePainter.drawImage(currentSelectedWhitePosition-QPoint(selectFrameSize/2,selectFrameSize/2),
                QImage(":/select_orange.png"));

            painter.drawImage(QPoint(0, 0), newBoardImage);

            needRepaintPeices = false;
        }

        imagePainter.end();
        painter.end();
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (isPvC) {
        QPoint mousePositionInBoard = lblBoard->mapFromParent(event->pos());
        QPoint selectedDot = convertPositionToDot(mousePositionInBoard);
        if (isDotInBoardArray(selectedDot)) {
            playerRound == GameRound::BLACK ? currentSelectedBlackDot = selectedDot :
                    currentSelectedWhiteDot = selectedDot;
        }
        lblBoard->repaint();
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (!isGameOver && isPvC && isPlayerRound() && (event->button() & Qt::LeftButton))
    { 
        bool isPressValid = false;
        if (playerRound == GameRound::BLACK) {
            if (isDotEmptyInBoardArray(currentSelectedBlackDot)) {
                addCurrentSelectedBlackDotToBoardArray();
                addNewMovePieceToCoreGameWithTag(CORE_SIDE_BLACK, convertDotToCorePoint(currentSelectedBlackDot), gameTag);
                isPressValid = true;
            }
        }
        else {
            if (isDotEmptyInBoardArray(currentSelectedWhiteDot)) {
                addCurrentSelectedWhiteDotToBoardArray();
                addNewMovePieceToCoreGameWithTag(CORE_SIDE_WHITE, convertDotToCorePoint(currentSelectedWhiteDot), gameTagEx);
                isPressValid = true;
            }
        }
        if (isPressValid) {
            needRepaintPeices = true;
            lblBoard->repaint();
            handleGameWinner();
            goToNextGameRound();
            slot_movePieceByMachine();
        }
    }
}

QPoint MainWindow::convertPositionToDot(QPoint position)
{
    QPoint dot;
    position -= QPoint(boardLeftOffset-crossDotOffset/2, boardTopOffset-crossDotOffset/2);
    dot.setY(position.x() >= 0 ? position.x() / crossDotOffset : -1);
    dot.setX(position.y() >= 0 ? position.y() / crossDotOffset : -1);
    return dot; 
}

QPoint MainWindow::convertDotToPosition(QPoint dot)
{
    QPoint position;
    position.setY(dot.x() * crossDotOffset + boardTopOffset);
    position.setX(dot.y() * crossDotOffset + boardLeftOffset);
    return position;
}

void MainWindow::slot_switchToPvCWithPlayerBlack()
{
    releaseGobangCore();
    initializeGobangCore();
    createNewCoreGameWithTag(gameTagEx, CORE_SIDE_WHITE);
    this->setWindowTitle("Gobang Analyzer - Player vs. Machine (Player Black)");
    gameCvCTimer->stop();
    cbCvCIntervalTime->setEnabled(false);

    isPvC = true;
    isMachineFirstStep = false;
    isCvC = false;
    isGameOver = false;
    playerRound = GameRound::BLACK;
    machineRound = GameRound::WHITE;
    gameRound = GameRound::BLACK;
    initializeBoardArray();

    needRepaintPeices = true;
    lblBoard->repaint(); 
}

void MainWindow::slot_switchToPvCWithPlayerWhite()
{
    releaseGobangCore();
    initializeGobangCore();
    createNewCoreGameWithTag(gameTag, CORE_SIDE_BLACK);
    this->setWindowTitle("Gobang Analyzer - Player vs. Machine (Player White)");
    gameCvCTimer->stop();
    cbCvCIntervalTime->setEnabled(false);

    isPvC = true;
    isMachineFirstStep = true;
    isCvC = false;
    isGameOver = false;
    playerRound = GameRound::WHITE;
    machineRound = GameRound::BLACK;
    gameRound = GameRound::BLACK;
    initializeBoardArray();

    needRepaintPeices = true;
    lblBoard->repaint();

    slot_movePieceByMachine();
}

void MainWindow::slot_switchToCvC()
{
    releaseGobangCore();
    initializeGobangCore();
    createNewCoreGameWithTag(gameTagEx, CORE_SIDE_BLACK);
    createNewCoreGameWithTag(gameTag, CORE_SIDE_WHITE);
    this->setWindowTitle("Gobang Analyzer - Machine vs. Machine");
    gameCvCTimer->stop();
    cbCvCIntervalTime->setEnabled(true);
    
    isCvC = true;
    isMachineFirstStep = true;
    isPvC = false;
    isGameOver = false;
    machineRound = GameRound::BLACK;
    gameRound = GameRound::BLACK;
    initializeBoardArray();

    needRepaintPeices = true;
    lblBoard->repaint(); 

    gameCvCTimer->setInterval(1000*gameCvCIntervalTimeOfSeconds);
    gameCvCTimer->start();
}

void MainWindow::slot_changeCvCIntervalTime(int seconds)
{
    gameCvCIntervalTimeOfSeconds = seconds;
    gameCvCTimer->setInterval(1000*seconds);
}

QPoint MainWindow::getMovePositionOfMachine()
{
    if (isMachineFirstStep) {
        isMachineFirstStep = false;
        return QPoint(7, 7);
    }
    CorePrefabConfig prefabConfig = {
        machineRound == GameRound::BLACK ? CORE_SIDE_BLACK : CORE_SIDE_WHITE,
        CORE_LEVEL_MIDDLE
    };

    qDebug() << "Before GetCoreAnalysisResult";
    boardArray->printBoardArray();

    CorePoint point = getCorePointFromCoreAlgorithm(&prefabConfig,
        machineRound == GameRound::BLACK ? gameTagEx : gameTag);
    return QPoint(point.i, point.j);
}

void MainWindow::slot_movePieceByMachine()
{
    if (isGameOver) return;
    movePieceByMachineMutex.lock();
    QPoint movePosition = getMovePositionOfMachine();
    if (movePosition.x() == -1 && movePosition.y() == -1) {
        gameCvCTimer->stop();
        movePieceByMachineMutex.unlock();
        return;
    }

    machineRound == GameRound::BLACK ? currentSelectedBlackDot = movePosition :
        currentSelectedWhiteDot = movePosition;

    if (!boardArray->isEmptyAtPosition(movePosition)) {
        qDebug() << "Error! movePosition not empty!";
    }

    machineRound == GameRound::BLACK ? addCurrentSelectedBlackDotToBoardArray() :
        addCurrentSelectedWhiteDotToBoardArray();
    if (isCvC) {
        addNewMovePieceToCoreGameWithTag(
            machineRound == GameRound::BLACK ? CORE_SIDE_BLACK : CORE_SIDE_WHITE,
            convertDotToCorePoint(machineRound == GameRound::BLACK ?
                currentSelectedBlackDot : currentSelectedWhiteDot),
            gameTagEx
        );
        addNewMovePieceToCoreGameWithTag(
            machineRound == GameRound::BLACK ? CORE_SIDE_BLACK : CORE_SIDE_WHITE,
            convertDotToCorePoint(machineRound == GameRound::BLACK ?
                currentSelectedBlackDot : currentSelectedWhiteDot),
            gameTag
        );
    }
    else if (machineRound == GameRound::BLACK) {
        addNewMovePieceToCoreGameWithTag(
            CORE_SIDE_BLACK,
            convertDotToCorePoint(currentSelectedBlackDot),
            gameTagEx
        );
    }
    else if (machineRound == GameRound::WHITE) {
        addNewMovePieceToCoreGameWithTag(
            CORE_SIDE_WHITE,
            convertDotToCorePoint(currentSelectedWhiteDot),
            gameTag
        );
    }

    qDebug() << "After GetCoreAnalysisResult";
    boardArray->printBoardArray();

    needRepaintPeices = true;
    lblBoard->repaint();

    handleGameWinner();
    goToNextGameRound();
    movePieceByMachineMutex.unlock();
}

void MainWindow::addCurrentSelectedBlackDotToBoardArray()
{
    if (isDotEmptyInBoardArray(currentSelectedBlackDot)) {
        boardArray->setBlackAtPosition(currentSelectedBlackDot);
    }
}

void MainWindow::addCurrentSelectedWhiteDotToBoardArray()
{
    if (isDotEmptyInBoardArray(currentSelectedWhiteDot)) {
        boardArray->setWhiteAtPosition(currentSelectedWhiteDot);
    }
}

bool MainWindow::isDotInBoardArray(QPoint dot)
{
    return 0 <= dot.x() && dot.x() < boardArray->colSize() &&
        0 <= dot.y() && dot.y() < boardArray->rowSize();
}

bool MainWindow::isDotEmptyInBoardArray(QPoint dot)
{
    return boardArray->isEmptyAtPosition(dot);
}

void MainWindow::handleGameWinner()
{
    Side winner = boardArray->getWinnerFromBoardArray();
    if (winner == SIDE_BLACK) {
        this->setWindowTitle("Black Win");
        isGameOver = true;
        if (gameCvCTimer->isActive())
            gameCvCTimer->stop();
    }
    else if (winner == SIDE_WHITE) {
        this->setWindowTitle("White Win");
        isGameOver = true;
        if (gameCvCTimer->isActive())
            gameCvCTimer->stop();
    }
}
