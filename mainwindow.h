#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QListWidget;
class QStackedLayout;
class QTextEdit;
class QPushButton;
class QRadioButton;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSpinBox;

#define COM -1
#define MAN 1
#define STEP 9
#define DRAW 0
#define ROW 3
#define COL 3
#define MAX_NUM 1000;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QList<QPushButton *> btnList;

private slots:
    void btnClick();
    void astar();
    void prin();
    void ending();
    void radioBtnClick();

private:
    Ui::MainWindow *ui;

    void createAstarPage();             //定义创建Astar页面函数
    void createAlphabetaPage();         //定义创建Alphabeta页面函数
    void createReslotionprinciplePage();//定义创建Reslotionprinciple页面函数

    QListWidget *listWidget;
    QStackedLayout *stackedLayout;
    QWidget *astarPage;                 //1
    QWidget *alphabetaPage;             //2
    QWidget *reslotionprinciplePage;    //3

//1
    QGroupBox *mapGroupBox;         //地图GroupBox
    QTextEdit *mapEdit;

    QGroupBox *startendGroupBox;    //起终点GroupBox
    QLabel *startLable_x;             //起点标签
    QLabel *startLable_y;             //起点标签
    QSpinBox *startLine_x;           //起点输入框
    QSpinBox *startLine_y;           //起点输入框
    QLabel *endLable_x;               //终点标签
    QLabel *endLable_y;               //终点标签
    QSpinBox *endLine_x;             //终点输入框
    QSpinBox *endLine_y;             //终点输入框
    QPushButton *beginButton;       //开始按钮

    QGroupBox *pathGroupBox;        //路径GroupBox
    QTextEdit *pathEdit;            //路径输出

    bool can[10][10] = {                           //迷宫模型
               {false,false,false,false,false,false,false,false,false,false},
               {false,true ,true ,false,true ,true ,true ,false,true ,false},
               {false,true ,true ,false,true ,true ,true ,false,true ,false},
               {false,true ,true ,true ,true ,false,false,true ,true ,false},
               {false,true ,false,false,false,true ,true ,true ,true ,false},
               {false,true ,true ,true ,false,true ,true ,true ,true ,false},
               {false,true ,false,true ,true ,true ,false,true ,true ,false},
               {false,true ,false,false,false,true ,false,false,true ,false},
               {false,false,true ,true ,true ,true ,true ,true ,true ,false},
               {false,false,false,false,false,false,false,false,false,false}};

//2
    QLabel *depthLable;             //搜索深度标签
    QLineEdit *depthEdit;           //搜索深度值
    QGroupBox *gameGroupBox;        //游戏GroupBox
    QGroupBox *consoleGroupBox;     //控制GroupBox
    QPushButton *restartButton;     //重新开始按钮
    QRadioButton *renrenRadioButton;//人人模式
    QRadioButton *renjiRadioButton; //人机模式

    int gameType;    // 人人对战 = 1， 人机对战 = 0
    int _cirCleMapNum[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
    };
    int _crossMapNum[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
    };

    //棋盘
    int board[3][3] = {
        { 0,0,0 },
        { 0,0,0 },
        { 0,0,0 }
    };
    // 临时棋盘
    int tempBoard[3][3] = {
        { 0,0,0 },
        { 0,0,0 },
        { 0,0,0 }
    };

    struct Move
    {
        int x;
        int y;

    };

    //玩家
    int player = MAN;
    //最好的一步
    Move bestMove;
    //当前深度
    int currentDepth = 9;

    void isWin();
    void clearMapNum();

    int a_isWin();
    int a_evaluteMap();
    void a_makeMove(Move curMove);
    void a_unMakeMove(Move curMove);
    int a_getMoveList(Move moveList[]);
    int a_miniMaxsearch(int depth);
    void a_printBoard();
    void a_com_play();
    void a_man_play(QPushButton *btn);

//3
    QTextEdit *edit1;
    QPushButton *button1;
    QTextEdit *edit2;


};

#endif // MAINWINDOW_H
