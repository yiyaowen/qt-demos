#ifndef DEMO_WINDOW
#define DEMO_WINDOW

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>

#include "BoardArray.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent=nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    enum class GameRound : int {
        BLACK,
        WHITE
    };
    GameRound gameRound = GameRound::BLACK;
    GameRound playerRound;
    GameRound machineRound;

    inline bool isPlayerRound() { return isPvC && playerRound == gameRound; }
    inline void reverseRound(GameRound* round)
    {
        *round == GameRound::BLACK ? *round = GameRound::WHITE : *round = GameRound::BLACK;
    }
    inline void goToNextGameRound()
    {
        if (isGameOver) return;
        if (isCvC) {
            reverseRound(&machineRound);
        }
        reverseRound(&gameRound);
    }

private:
    void initializeLayout();
    void initializeBoardArray();
    void bindSignalsAndSlots();

    bool isWindowFirstShow = true;

private:
    QLabel* lblBoard;    
    QPushButton* btnSwitchToPvCWithPlayerBlack;
    QPushButton* btnSwitchToPvCWithPlayerWhite;
    QPushButton* btnSwitchToCvC;

    QComboBox* cbCvCIntervalTime;
    QTimer* gameCvCTimer = new QTimer;
    int gameCvCIntervalTimeOfSeconds = 0;
    QMutex movePieceByMachineMutex;

    QImage oldBoardImage;
    bool needRepaintPeices = true;

private:
    BoardArray* boardArray;

    bool isPvC = false;
    bool isCvC = false;
    bool isMachineFirstStep = true;
    bool isGameOver = false;
    int gameTag = 0;
    int gameTagEx = 1;

    QPoint currentSelectedBlackDot = QPoint(0, 0);
    QPoint currentSelectedWhiteDot = QPoint(0, 0);

    QPoint convertPositionToDot(QPoint position);
    QPoint convertDotToPosition(QPoint dot);
    bool isDotInBoardArray(QPoint dot);

    bool isDotEmptyInBoardArray(QPoint dot);
    void addCurrentSelectedBlackDotToBoardArray();
    void addCurrentSelectedWhiteDotToBoardArray();

    QPoint getMovePositionOfMachine();

    void handleGameWinner();

private slots:
    void slot_switchToPvCWithPlayerBlack();
    void slot_switchToPvCWithPlayerWhite();
    void slot_switchToCvC();
    void slot_changeCvCIntervalTime(int seconds);
    void slot_movePieceByMachine();
};

constexpr int rowCount = 15, colCount = 15;
constexpr int boardSize = 512;
constexpr int pieceSize = 25;
constexpr int selectFrameSize = 31;
constexpr int boardLeftOffset = 47;
constexpr int boardTopOffset = 32;
constexpr int crossDotOffset = 31;

#endif // DEMO_WINDOW
