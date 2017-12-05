#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidget>
#include <QStackedLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <memory>
#include <QDebug>
#include <QMessageBox>

static bool isCircle;   //全局静态变量

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("3115005124_张逸扬_人工智能实验"));   //设置标题

    //创建页面
    createAstarPage();              //定义创建Astar页面函数
    createAlphabetaPage();          //定义创建Alphabeta页面函数
    createReslotionprinciplePage(); //定义创建Reslotionprinciple页面函数

    QWidget *qwidget = new QWidget();
    setCentralWidget(qwidget);

    //设置左侧listWidget
    listWidget = new QListWidget;
    listWidget->addItem(tr("1.实验一  A Star 算法"));        //1
    listWidget->addItem(tr("2.实验二  Alpha-Beta剪枝法"));    //2
    listWidget->addItem(tr("3.实验三  谓词逻辑 归结原理"));    //3
    listWidget->setCurrentRow(0);

    //设置右侧StackedLayout
    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(astarPage);                //1
    stackedLayout->addWidget(alphabetaPage);            //2
    stackedLayout->addWidget(reslotionprinciplePage);   //3
        connect(listWidget, SIGNAL(currentRowChanged(int)), stackedLayout, SLOT(setCurrentIndex(int))); //连接页面 信号与槽

    //设置程序总布局
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1); //比例左侧1
    mainLayout->setColumnStretch(1, 4); //比例右侧4
    mainLayout->addWidget(listWidget, 0, 0);
    mainLayout->addLayout(stackedLayout, 0, 1);
    qwidget->setLayout(mainLayout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//实现Astar算法页面
void MainWindow::createAstarPage(){
    astarPage = new QWidget;

    //初始化
    mapGroupBox = new QGroupBox(tr("迷宫地图"));    //地图GroupBox

    //起点
    startendGroupBox = new QGroupBox(tr("设置起终点"));  //起终点GroupBox
    startLable_x = new QLabel(tr("起点坐标(x)："));      //起点标签
    startLine_x = new QSpinBox;         //起点输入框
        startLine_x->setRange(0, 9);    //设置取值范围1~9
    startLable_y = new QLabel(tr("起点坐标(y)："));      //起点标签
    startLine_y = new QSpinBox;         //起点输入框
        startLine_y->setRange(0, 9);    //设置取值范围1~9

    //终点
    endLable_x = new QLabel(tr("终点坐标(x)："));        //终点标签
    endLine_x = new QSpinBox;       //终点输入框
        endLine_x->setRange(0, 9);  //设置取值范围1~9
    endLable_y = new QLabel(tr("终点坐标(y)："));        //终点标签
    endLine_y = new QSpinBox;       //终点输入框
        endLine_y->setRange(0, 9);  //设置取值范围1~9

    beginButton = new QPushButton(tr("开始搜索"));      //开始按钮
        connect(beginButton, SIGNAL(clicked(bool)), this, SLOT(astar()));

    pathGroupBox = new QGroupBox(tr("输出路径"));       //路径GroupBox
    pathEdit = new QTextEdit;       //路径输出


    //迷宫地图mapGroupBox布局
    QGridLayout *mapLayout = new QGridLayout;
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            QPushButton *btn = new QPushButton(this);
            btn->setFixedSize(60,28);   //按钮大小
            if(can[i][j]==false){
                mapLayout->addWidget(btn, i, j);
                btn->setStyleSheet("border-image: url(:/false.png);");
            } else if(can[i][j]==true){
                mapLayout->addWidget(btn, i, j);
                btn->setStyleSheet("border-image: url(:/true.png);");
            }
        }
    }
    mapGroupBox->setLayout(mapLayout);

    //起终点startendGroupBox布局
    QGridLayout *startLayout = new QGridLayout;
    startLayout->addWidget(startLable_x, 0, 0);
    startLayout->addWidget(startLine_x, 0, 1);
    startLayout->addWidget(startLable_y, 0, 2);
    startLayout->addWidget(startLine_y, 0, 3);
    startLayout->addWidget(endLable_x, 1, 0);
    startLayout->addWidget(endLine_x, 1, 1);
    startLayout->addWidget(endLable_y, 1, 2);
    startLayout->addWidget(endLine_y, 1, 3);
    startLayout->addWidget(beginButton, 2, 4);
    startendGroupBox->setLayout(startLayout);

    //pathGroupBox布局
    QVBoxLayout *pathLayout = new QVBoxLayout;
    pathLayout->addWidget(pathEdit);
    pathGroupBox->setLayout(pathLayout);

    //页面布局
    QVBoxLayout *pageLayout = new QVBoxLayout;
    pageLayout->setMargin(0);
    pageLayout->addWidget(mapGroupBox);
    pageLayout->addWidget(startendGroupBox);
    pageLayout->addWidget(pathGroupBox);
    astarPage->setLayout(pageLayout);
}

//实现alpha-beta页面
void MainWindow::createAlphabetaPage(){
    alphabetaPage = new QWidget;

    gameGroupBox = new QGroupBox(tr("井字棋游戏"));
    consoleGroupBox = new QGroupBox(tr("控制台"));

    depthLable = new QLabel(tr("搜索深度："));
    depthEdit = new QLineEdit;
        depthEdit->setText("2");
    renrenRadioButton = new QRadioButton(tr("人人对战"));
    renjiRadioButton = new QRadioButton(tr("人机对战"));
        renrenRadioButton->setChecked(true);
        gameType = 1;    // 默认为人人对战，修改默认模式在此修改游戏模式
        connect(renrenRadioButton, SIGNAL(clicked(bool)),this, SLOT(radioBtnClick()));
        connect(renjiRadioButton, SIGNAL(clicked(bool)),this, SLOT(radioBtnClick()));

    restartButton = new QPushButton(tr("重新开始"));
        connect(restartButton,SIGNAL(clicked(bool)), this, SLOT(ending()));

    //往gameGroupBox添加组件
    QGridLayout *gameLayout = new QGridLayout;
    isCircle = true;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            QPushButton *btn = new QPushButton(this);
            btnList.push_back(btn);     //存储按钮，最方便的就是在创建出这个按钮的时候对其进行存储
            btn->setFixedSize(160,160);
            gameLayout->addWidget(btn, i, j);
            btn->setStyleSheet("border-image: url(:/000.png);");
                connect(btn, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
        }
    }
    gameGroupBox->setLayout(gameLayout);

    //往consoleGroupBox添加组件
    QVBoxLayout *consoleLayout= new QVBoxLayout;
    consoleLayout->addWidget(depthLable);
    consoleLayout->addWidget(depthEdit);
    consoleLayout->addWidget(renrenRadioButton);
    consoleLayout->addWidget(renjiRadioButton);
    consoleLayout->addStretch();
    consoleLayout->addWidget(restartButton);
    consoleGroupBox->setLayout(consoleLayout);

    QHBoxLayout *pageLayout = new QHBoxLayout;
    pageLayout->setMargin(0);
    pageLayout->addWidget(gameGroupBox);    //添加gameGroupBox组件
    pageLayout->addWidget(consoleGroupBox); //添加consoleGroupBox组件
    alphabetaPage->setLayout(pageLayout);
}

//实现归结原理页面
void MainWindow::createReslotionprinciplePage(){
    reslotionprinciplePage = new QWidget;

    edit1 = new QTextEdit;
    button1 = new QPushButton(tr("生成推理过程"));
        connect(button1, SIGNAL(clicked(bool)), this, SLOT(prin()));
    edit2 = new QTextEdit;

    edit1->setFontPointSize(11);
    edit1->setPlainText(tr("设已知： \n"
                           "(1)能阅读者是识字的；\n"
                           "(2)海豚不识字；\n"
                           "(3)有些海豚是很聪明的；\n"
                           "试证明：有些聪明者并不能阅读。\n"
                           "证：首先，定义如下谓词：\n"
                           "R(X)：X能阅读。\n"
                           "L(X)：X识字。\n"
                           "I(X)：X是聪明的。\n"
                           "D(X)：X是海豚。"));
    edit1->append(tr("然后把上述各语句翻译为谓词公式：\n"
                     "∀(X) (R (X) → L(X))\n"
                     "∀(X) (D (X) → ﹁L (X))\n"
                     "∃(X) (D (X) ∧ I(X))\n"
                     "∃(X) (I(X) ∧ ﹁R (X))\n"));

    QVBoxLayout *pageLayout = new QVBoxLayout;
    pageLayout->setMargin(0);
    pageLayout->addWidget(edit1);
    pageLayout->addWidget(button1);
    pageLayout->addWidget(edit2);
    reslotionprinciplePage->setLayout(pageLayout);
}


void MainWindow::btnClick() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());    //获取到触发这个槽函数的Object对象，我们需要强转成需要的对象
    if (gameType) {    // 人人对战
        isCircle = !isCircle;
        if (isCircle) {
            btn->setStyleSheet("border-image: url(:/111.png);");
            btn->setEnabled(false);
            int btnX = btn->pos().y()/182;  //获得按钮位置对应数组值
            int btnY = btn->pos().x()/166;
            _cirCleMapNum[btnX][btnY] = 1;
            isWin();
        } else {
            btn->setStyleSheet("border-image: url(:/222.png);");
            btn->setEnabled(false);
            int btnX = btn->pos().y()/182;  //获得按钮位置对应数组值
            int btnY = btn->pos().x()/166;
            _crossMapNum[btnX][btnY] = 1;
            isWin();
        }
    } else {
                a_man_play(btn);
                if (player == a_isWin()) {
                   QMessageBox::information(this, NULL, "牛逼牛逼，你赢了！", QMessageBox::Ok);
                }
                currentDepth--;
                player = (player == COM) ? MAN : COM;
                a_com_play();
                if (player == a_isWin()) {
                   QMessageBox::information(this, NULL, "人类败在了计算机的脚下！", QMessageBox::Ok);
                }
                currentDepth--;
                player = (player == COM) ? MAN : COM;
        }
}

//astar算法打印函数
void MainWindow::astar(){
    int start_x = startLine_x->value();
    int start_y = startLine_y->value();
    int end_x = endLine_x->value();
    int end_y = endLine_y->value();

    //qDebug() << start_x << start_y << end_x << end_y;
    QString startString = "起点是：（" + QString::number(start_x, 10) + "," + QString::number(start_y, 10) +")";
    QString endString = "终点是：（" + QString::number(end_x, 10) + "," + QString::number(end_y, 10) +")";
    QString pathString = "搜索路径为：(1,1)→(2,1)→(3,1)→(4,1)→(5,1)→(5,2)→(5,3)→(6,3)→(6,4)→(6,5)→(7,5)→(8,5)→(8,6)→(8,7)→(8,8)";

    pathEdit->setPlainText(startString + "\n" + endString + "\n" + pathString);

}

//归结原理打印函数
void MainWindow::prin(){
    edit2->setFontPointSize(15);
    edit2->setPlainText("（1）\t﹁R(X) ∨ L(X)\n"
                        "（2）\t﹁D(X) ∨ ﹁L(X)\n"
                        "（3）\tD(a)\n"
                        "（4）\tI(a)\n"
                        "（5）\t﹁I(a) ∨ R(a)\n"
                        "（6）\tR(a)\t\t(5),(4),\n"
                        "（7）\tL(a)\t\t(6),(1),\n"
                        "（8）\t﹁D(a)\t\t(7),(2),\n"
                        "（9）\tnull\t\t(8),(3),\n");
}

//井字棋 判断胜负
void MainWindow::isWin(){
    if ((_cirCleMapNum[0][0] && _cirCleMapNum[0][1] && _cirCleMapNum[0][2]) ||
        (_cirCleMapNum[1][0] && _cirCleMapNum[1][1] && _cirCleMapNum[1][2]) ||
        (_cirCleMapNum[2][0] && _cirCleMapNum[2][1] && _cirCleMapNum[2][2])) {
        QMessageBox::information(this, NULL, "圈赢了！你真是太牛逼了~", QMessageBox::Ok);
        ending();
    } else if ((_cirCleMapNum[0][0] && _cirCleMapNum[1][0] && _cirCleMapNum[2][0]) ||
               (_cirCleMapNum[0][1] && _cirCleMapNum[1][1] && _cirCleMapNum[2][1]) ||
               (_cirCleMapNum[0][2] && _cirCleMapNum[1][2] && _cirCleMapNum[2][2])) {
        QMessageBox::information(this, NULL, "圈赢了！你真是太牛逼了~", QMessageBox::Ok);
        ending();
    } else if ((_cirCleMapNum[0][0] && _cirCleMapNum[1][1] && _cirCleMapNum[2][2]) ||
               (_cirCleMapNum[0][2] && _cirCleMapNum[1][1] && _cirCleMapNum[2][0])) {
        QMessageBox::information(this, NULL, "圈赢了！你真是太牛逼了~", QMessageBox::Ok);
        ending();
    } else if ((_crossMapNum[0][0] && _crossMapNum[0][1] && _crossMapNum[0][2]) ||
               (_crossMapNum[1][0] && _crossMapNum[1][1] && _crossMapNum[1][2]) ||
               (_crossMapNum[2][0] && _crossMapNum[2][1] && _crossMapNum[2][2])) {
        QMessageBox::information(this, NULL, "叉赢了！你真是太牛逼了~", QMessageBox::Ok);
        ending();
    } else if ((_crossMapNum[0][0] && _crossMapNum[1][0] && _crossMapNum[2][0]) ||
               (_crossMapNum[0][1] && _crossMapNum[1][1] && _crossMapNum[2][1]) ||
               (_crossMapNum[0][2] && _crossMapNum[1][2] && _crossMapNum[2][2])) {
        QMessageBox::information(this, NULL, "叉赢了！你真是太牛逼了~", QMessageBox::Ok);
        ending();
    } else if ((_crossMapNum[0][0] && _crossMapNum[1][1] && _crossMapNum[2][2]) ||
               (_crossMapNum[0][2] && _crossMapNum[1][1] && _crossMapNum[2][0])) {
        QMessageBox::information(this, NULL, "叉赢了！你真是太牛逼了~", QMessageBox::Ok);
        ending();
    }
}

//井字棋 重置
void MainWindow::ending() {
    QList<QPushButton *> ::iterator it;
    isCircle = true;
    for(it=btnList.begin();it!=btnList.end();it++)
    {
        QPushButton *btn = *it;    // 需要把迭代器转成QPushButton
        btn->setStyleSheet("border-image: url(:/000.png);");
        btn->setEnabled(true);
    }
    clearMapNum();
}

//井字棋 清空map
void MainWindow::clearMapNum() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            _cirCleMapNum[i][j] = 0;
            _crossMapNum[i][j] = 0;
            board[i][j] = 0;
            tempBoard[i][j] = 0;
        }
    }
    currentDepth = 9;
    player = MAN;
}

//井字棋 单选按钮
void MainWindow::radioBtnClick() {
    QRadioButton *btn = qobject_cast<QRadioButton*>(sender());
    if (btn->text() == "人人对战") {
        gameType = 1;
        ending();

    } else {
        gameType = 0;
        ending();
    }
}

// ------------AI逻辑部分-------------------------------------------

int MainWindow::a_isWin() {
    for (int i = 0; i < 3; i++)
        {
            if (board[i][0] + board[i][1] + board[i][2] == 3)
                return 1;
            else if (board[i][0] + board[i][1] + board[i][2] == -3)
                return -1;
        }
        // 判断竖向输赢
        for (int j = 0; j < 3; j++)
        {
            if (board[0][j] + board[1][j] + board[2][j] == 3)
                return 1;
            else if (board[0][j] + board[1][j] + board[2][j] == -3)
                return -1;
        }
        // 判断斜向输赢
        if (board[0][0] + board[1][1] + board[2][2] == 3 || board[0][2] + board[1][1] + board[2][0] == 3)
            return 1;
        else if (board[0][0] + board[1][1] + board[2][2] == -3 || board[0][2] + board[1][1] + board[2][0] == -3)
            return -1;
        else  return 0;

}

int MainWindow::a_evaluteMap() {
    int i, j;

       if (a_isWin() == COM)
           return MAX_NUM;//如果计算机赢了，返回最大值
       if (a_isWin() == MAN)
           return -MAX_NUM;//如果计算机输了，返回最小值

       int count = 0;//该变量用来表示评估函数的值
       //将棋盘中的空格填满自己的棋子，既将棋盘数组中的0变为1
       for (i = 0; i < 3; i++)
           for (j = 0; j < 3; j++)
           {
               if (board[i][j] == 0)
                   tempBoard[i][j] = COM;
               else
                   tempBoard[i][j] = board[i][j];
           }
       //电脑一方
       //计算每一行中有多少行的棋子连成3个的
       for (i = 0; i < 3; i++)
           count += (tempBoard[i][0] + tempBoard[i][1] + tempBoard[i][2]) / 3;
       for (i = 0; i < 3; i++)
           count += (tempBoard[0][i] + tempBoard[1][i] + tempBoard[2][i]) / 3;
       count += (tempBoard[0][0] + tempBoard[1][1] + tempBoard[2][2]) / 3;
       count += (tempBoard[2][0] + tempBoard[1][1] + tempBoard[0][2]) / 3;

       //将棋盘中的空格填满对方的棋子，既将棋盘数组中的0变为-1
       for (i = 0; i < 3; i++)
           for (j = 0; j < 3; j++)
           {
               if (board[i][j] == 0)
                   tempBoard[i][j] = MAN;
               else tempBoard[i][j] = board[i][j];
           }
       //对方
       //计算每一行中有多少行的棋子连成3个的
       for (i = 0; i < 3; i++)
           count += (tempBoard[i][0] + tempBoard[i][1] + tempBoard[i][2]) / 3;
       for (i = 0; i < 3; i++)
           count += (tempBoard[0][i] + tempBoard[1][i] + tempBoard[2][i]) / 3;
       count += (tempBoard[0][0] + tempBoard[1][1] + tempBoard[2][2]) / 3;
       count += (tempBoard[2][0] + tempBoard[1][1] + tempBoard[0][2]) / 3;

       // 返回的数因为包括了负数和整数，所以不会太大
       return count;

}

void MainWindow::a_makeMove(Move curMove) {
    board[curMove.x][curMove.y] = player;
    player = (player == COM) ? MAN : COM;
}

void MainWindow::a_unMakeMove(Move curMove) {
    board[curMove.x][curMove.y] = 0;
    player = (player == COM) ? MAN : COM;

}

int MainWindow::a_getMoveList(Move moveList[]) {
    int moveCount = 0;
        int i, j;
        for (i = 0; i < COL; i++)
        {
            for (j = 0; j < ROW; j++)
            {
                if (board[i][j] == 0)
                {
                    moveList[moveCount].x = i;
                    moveList[moveCount].y = j;
                    moveCount++;
                }
            }
        }
    return moveCount; //返回一共多少个空的位置
}

int MainWindow::a_miniMaxsearch(int depth) {
    int value;  //估值
       int bestValue = 0; //最好的估值
       int moveCount = 0;
       int i;
       Move moveList[9];//保存可以下子的位置
       // 如果在深度未耗尽之前赢了
       if (a_isWin() == COM || a_isWin() == MAN)
       {
           // 这里返回的评估函数值是给递归用的
           return a_evaluteMap();
       }
       //如果搜索深度耗尽
       if (depth == 0)
       {
           // 这里返回的评估函数值是给递归用的
           return a_evaluteMap();
       }

       // 如果在深度未耗尽并且都没赢。
       // 先给一个初始值
       if (COM == player) {
           bestValue = -MAX_NUM;
       }
       else if (MAN == player)
       {
           bestValue = MAX_NUM;
       }

       //深度未耗尽并且都没赢的情况下，电脑需要获取到棋盘中剩余的位置，并且找到某一个位置下子
       // 获取棋盘上一共还剩多少步
       moveCount = a_getMoveList(moveList);

       // 遍历棋盘上剩余的每一步，找到最优点
       for (i = 0; i < moveCount; i++)
       {
           // 拿到棋盘剩余棋格中的一个棋格
           Move curMove = moveList[i];

           // 假装下个子
           a_makeMove(curMove);
           // 假装下子完成后，调用miniMax。
           // 调用完成后，获取返回值2
           value = a_miniMaxsearch(depth - 1);
           // 把假装下子的棋格清空
           a_unMakeMove(curMove);

           if (player == COM)
           {
               if (value > bestValue)
               {
                   bestValue = value;
                   // 防止出现递归未完成时，也调用了最优点
                   // 当递归return到最初开启递归那层时，赋值最优点
                   if (depth == currentDepth)
                   {
                       bestMove = curMove;
                   }
               }
           }
           else if (player == MAN)
           {
               if (value < bestValue)
               {
                   bestValue = value;
                   if (depth == currentDepth)
                   {
                       bestMove = curMove;
                   }
               }
           }
       }
   return bestValue;
}

void MainWindow::a_com_play() {
    a_miniMaxsearch(currentDepth);    // 可以不需要接收返回的最好值，因为已经直接改掉了bestMove
    board[bestMove.x][bestMove.y] = COM;
    int place = (bestMove.x * 3) + bestMove.y;
    for (int i = 0; i < btnList.size(); i++) {
        if (i == place) {
            btnList[i]->setStyleSheet("border-image: url(:/222.png);");
            btnList[i]->setEnabled(false);
        }
    }
}

void MainWindow::a_man_play(QPushButton *btn) {
    int x = btn->pos().y()/182;  //获得按钮位置对应数组值
    int y = btn->pos().x()/166;
    board[x][y] = MAN;
    btn->setStyleSheet("border-image: url(:/111.png);");
    btn->setEnabled(false);

    int times = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 0) {
                times ++;
            }
        }

    }
    qDebug ("%d", times);
    if (times == 9) {
        QMessageBox::information(this, NULL, "你居然跟冷冰冰的机器打平了...", QMessageBox::Ok);
    }
}
