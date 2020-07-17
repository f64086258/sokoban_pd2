#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QLayout>
#include <QString>
#include <QtGlobal>
#include <QMessageBox>
#include <QtMath>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    usetime = new QTimer;
    current_stage = 1;
    maxstage = 1;
    basetime = basetime.currentTime();
    nextmove = 0;
    movenum = 0;
    coinnum = 0;
    for(int i = 0; i < totallevel; i++)
    {
        maxtime[i] = pow(2, 31) -1;
        maxstep[i] = pow(2, 31) -1;
    }
    setmenu();
    setResource();
}


MainWindow::~MainWindow()
{
    delete maxstepnumlabel;
    delete movenumlabel;
    delete usetime;
    delete lcd;
    delete nextlevel;
    delete timer;
    for(int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) {
            delete maplabel[i][j];
            delete mapground[i][j];
        }
        delete [] maplabel[i];
        delete [] mapground[i];
    }
    delete [] maplabel;
    delete [] mapground;
    delete ui;
    delete mainWidget;
    delete gamemap;
}


void MainWindow::setmenu()
{
    int l = maxstage;
    current_position = {0,0};
    timeactive = false;
    timecount = 0;
    leg = 0;
    usetime->stop();
    timer = nullptr;
    usetime = nullptr;
    lcd = nullptr;
    nextlevel = nullptr;
    movenumlabel = nullptr;
    maxstepnumlabel = nullptr;
    gamemap = nullptr;
    height = 5;
    width = 5;
    maplabel = new QLabel**[height];
    for(int i = 0; i < height; i++)
    {
        maplabel[i] = new QLabel*[width];
        for(int j = 0; j < width; j++)
            maplabel[i][j] = nullptr;
    }
    mapground = new QLabel**[height];
    for(int i = 0; i < height; i++)
    {
        mapground[i] = new QLabel*[width];
        for(int j = 0; j < width; j++)
            mapground[i][j] = nullptr;
    }
    mainWidget = new QWidget;
    QPushButton *NewGame = new QPushButton;
    QPushButton *Choose = new QPushButton;
    QPushButton *Quit = new QPushButton;
    QVBoxLayout *menulayout = new QVBoxLayout;
    QHBoxLayout *menu = new QHBoxLayout;
    menulayout->setMargin(0);
    menulayout->setSpacing(0);
    NewGame->setFocusPolicy(Qt::NoFocus);
    Choose->setFocusPolicy(Qt::NoFocus);
    Quit->setFocusPolicy(Qt::NoFocus);
    NewGame->setText("Level " + QString::number(maxstage));
    Choose->setText("Choose");
    Quit->setText("Quit");
    connect(NewGame, &QPushButton::clicked, this, [this, l]{selectgame(l);});
    connect(Choose, SIGNAL(clicked()), this, SLOT(select()));
    connect(Quit, SIGNAL(clicked()), this, SLOT(close()));
    menulayout->addStretch();
    menulayout->addWidget(NewGame);
    menulayout->addWidget(Choose);
    menulayout->addWidget(Quit);
    menulayout->addStretch();
    menu->addStretch();
    menu->addLayout(menulayout);
    menu->addStretch();
    mainWidget->setLayout(menu);
    setCentralWidget(mainWidget);
}
void MainWindow::restartgame()
{
    gamemap = new Map(current_stage);
    lcd->display("00:00:00");
    basetime = basetime.currentTime();
    usetime->start(1000);
    movenum = 0;
    coinnum = 0;
    movenumlabel->setNum(movenum);
    coinnumlabel->setNum(coinnum);
    for(int i = 0; i < gamemap->height; i++)
    {
        for(int j = 0; j < gamemap->width; j++)
        {
            gamemap->mapdata[i][j] = gamemap->mapbackup[i][j];
            if(gamemap->mapbackup[i][j] == WALL)
            {
                  maplabel[i][j]->setPixmap(Wall);
            }
            else if(gamemap->mapbackup[i][j] == PLAYER)
            {
                maplabel[i][j]->setPixmap(Gamer);
                current_position = {j, i};
            }
            else if(gamemap->mapbackup[i][j] == CRATES)
            {
                maplabel[i][j]->setPixmap(Crates);
            }
            else if(gamemap->mapbackup[i][j] == GOAL)
            {
                maplabel[i][j]->setPixmap(Goal);
            }
            else if(gamemap->mapbackup[i][j] == COIN)
            {
                maplabel[i][j]->setPixmap(Coin);
            }
            else if(gamemap->mapbackup[i][j] == STAR)
            {
                maplabel[i][j]->setPixmap(Star);
            }
            else {
                maplabel[i][j]->setPixmap(Ground);
            }
        }
    }
    mainWidget->update();
}
void MainWindow::setResource()
{
    Wall.load(":/res/Blocks/block_08.png");
    Ground.load(":/res/Ground/ground_06.png");
    Crates.load(":/res/Crates/crate_02.png");
    Gamer.load(":/res/Player/player_05.png");
    Goal.load(":/res/Environment/environment_02.png");
    GoLeft[0].load(":/res/Player/player_22.png");
    GoLeft[1].load(":/res/Player/player_21.png");
    GoLeft[2].load(":/res/Player/player_20.png");
    GoUp[0].load(":/res/Player/player_09.png");
    GoUp[1].load(":/res/Player/player_10.png");
    GoUp[2].load(":/res/Player/player_08.png");
    GoRight[0].load(":/res/Player/player_18.png");
    GoRight[1].load(":/res/Player/player_19.png");
    GoRight[2].load(":/res/Player/player_17.png");
    GoDown[0].load(":/res/Player/player_06.png");
    GoDown[1].load(":/res/Player/player_07.png");
    GoDown[2].load(":/res/Player/player_05.png");
    Coin.load(":/res/Environment/environment_12.png");
    Star.load(":/res/Environment/environment_10.png");
}
void MainWindow::newgame()
{
    selectgame(1);
}
void MainWindow::select()
{
    mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    QPushButton **levelbutton = new QPushButton*[totallevel];
    for(int i = 0; i < totallevel; i++)
    {
        levelbutton[i] = new QPushButton;
    }
    QHBoxLayout *levellayout = new QHBoxLayout;
    levellayout->addStretch();
    for(int i = 0; i < totallevel; i++)
    {
        levelbutton[i]->setText(QString::number(i+1));
        levellayout->addWidget(levelbutton[i]);
        connect(levelbutton[i], &QPushButton::clicked, this, [this, i]{selectgame(i+1);});
        if(maxstage < i + 1)
        {
            levelbutton[i]->setEnabled(false);
        }
        else {
            levelbutton[i]->setEnabled(true);
        }
    }
    levellayout->addStretch();
    mainWidget->setLayout(levellayout);
}
void MainWindow::selectgame(int l)
{
    movenum = 0;
    current_stage = l;
    if(maxstage<current_stage)
        maxstage = current_stage;
    mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    gamemap = new Map(l);
    height = gamemap->height;
    width = gamemap->width;
    QGridLayout *MAP = new QGridLayout;
    MAP->setMargin(0);
    MAP->setSpacing(0);
    MAP->setSizeConstraint(QLayout::SetFixedSize);
    maplabel = new QLabel **[height];
    for(int i = 0; i < height; i++)
    {
        maplabel[i] = new QLabel *[width];
        for(int j = 0; j < width; j++)
        {
            maplabel[i][j] = new QLabel;
        }
    }
    mapground = new QLabel **[height];
    for(int i = 0; i < height; i++)
    {
        mapground[i] = new QLabel *[width];
        for(int j = 0; j < width; j++)
        {
            mapground[i][j] = new QLabel;
        }
    }
    for(int i = 0; i < height; i++)
    {
        //MAP->setRowStretch(i, 0);
        for(int j = 0; j < width; j++)
        {
            mapground[i][j]->setPixmap(Ground);
            if(gamemap->mapdata[i][j] == WALL)
            {
                  maplabel[i][j]->setPixmap(Wall);
            }
            else if(gamemap->mapdata[i][j] == PLAYER)
            {
                maplabel[i][j]->setPixmap(Gamer);
                current_position = {j, i};
            }
            else if(gamemap->mapdata[i][j] == CRATES)
            {
                maplabel[i][j]->setPixmap(Crates);
            }
            else if(gamemap->mapdata[i][j] == GOAL)
            {
                maplabel[i][j]->setPixmap(Goal);
            }
            else if (gamemap->mapdata[i][j] == COIN) {
                maplabel[i][j]->setPixmap(Coin);
            }
            else if (gamemap->mapdata[i][j] == STAR) {
                maplabel[i][j]->setPixmap(Star);
            }
            else {
                maplabel[i][j]->setPixmap(Ground);
            }
            mapground[i][j]->setFixedSize(40,40);
            mapground[i][j]->setScaledContents(true);
            maplabel[i][j]->setFixedSize(40,40);
            maplabel[i][j]->setScaledContents(true);
            MAP->addWidget(mapground[i][j], i, j);
            MAP->addWidget(maplabel[i][j], i, j);
        }
    }
    QVBoxLayout *leftlayout = new QVBoxLayout;
    leftlayout->addStretch();
    leftlayout->addLayout(MAP);
    leftlayout->addStretch();
    QVBoxLayout *rightlayout = new QVBoxLayout;
    QPushButton *restart =new QPushButton;
    restart->setText("Restart");
    restart->setFocusPolicy(Qt::NoFocus);
    connect(restart, SIGNAL(clicked()), this, SLOT(restartgame()));
    nextlevel =new QPushButton;
    nextlevel->setText("Next Level");
    nextlevel->setFocusPolicy(Qt::NoFocus);
    connect(nextlevel, &QPushButton::clicked, this, [this, l]{selectgame(l+1);});
    nextlevel->setEnabled(false);
    QPushButton *backtomenu = new QPushButton;
    backtomenu->setText("Menu");
    backtomenu->setFocusPolicy(Qt::NoFocus);
    connect(backtomenu, SIGNAL(clicked()), this, SLOT(setmenu()));
    lcd = new QLCDNumber;
    lcd->display("00:00:00");
    usetime = new QTimer;
    connect(usetime, SIGNAL(timeout()), this, SLOT(updatetime()));
    basetime = basetime.currentTime();
    usetime->start(1000);
    movenumlabel = new QLabel;
    movenumlabel->setNum(movenum);
    QLabel *movelabel = new QLabel;
    movelabel->setText(" step: ");
    coinnumlabel = new QLabel;
    coinnumlabel->setNum(coinnum);
    QLabel *coinlabel = new QLabel;
    coinlabel->setText(" coin: ");
    QLabel *maxsteplabel = new QLabel;
    maxsteplabel->setText(" Least Step: ");
    maxstepnumlabel = new QLabel;
    if(maxstep[current_stage-1] < (pow(2, 31) - 1))
        maxstepnumlabel->setNum(maxstep[current_stage-1]);
    else
        maxstepnumlabel->setText("No Data");
    QHBoxLayout *steplayout = new QHBoxLayout;
    steplayout->addWidget(movelabel);
    steplayout->addWidget(movenumlabel);
    QHBoxLayout *maxsteplayout = new QHBoxLayout;
    maxsteplayout->addWidget(maxsteplabel);
    maxsteplayout->addWidget(maxstepnumlabel);
    QHBoxLayout *coinlayout = new QHBoxLayout;
    steplayout->addWidget(coinlabel);
    steplayout->addWidget(coinnumlabel);
    rightlayout->addStretch();
    rightlayout->addStretch();
    rightlayout->addWidget(lcd);
    rightlayout->addStretch();
    rightlayout->addLayout(steplayout);
    rightlayout->addLayout(maxsteplayout);
    rightlayout->addStretch();
    rightlayout->addStretch();
    rightlayout->addStretch();
    rightlayout->addWidget(restart);
    if(current_stage < totallevel)
        rightlayout->addWidget(nextlevel);
    rightlayout->addWidget(backtomenu);
    rightlayout->addStretch();
    rightlayout->addStretch();
    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout->addStretch();
    mainlayout->addLayout(leftlayout);
    mainlayout->addLayout(rightlayout);
    mainlayout->addStretch();
    mainWidget->setLayout(mainlayout);
}
void MainWindow::updatetime()
{
    QTime current = QTime::currentTime();
    int t = basetime.msecsTo(current);
    QTime showTime(0,0,0);
    showTime=showTime.addMSecs(t);
    QString timestr = showTime.toString("hh:mm:ss");
    lcd->display(timestr);
}
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    int movedirectx;
    int movedirecty;
    if(!timeactive && gamemap->cratenum != 0)
    {
        nextmove = 0;
    switch(e->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        movedirectx = -1;
        movedirecty = 0;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        movedirectx = 1;
        movedirecty = 0;
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        movedirectx = 0;
        movedirecty = -1;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        movedirectx = 0;
        movedirecty = 1;
        break;
    default:
        movedirectx = 0;
        movedirecty = 0;
        break;
    }

    moveplayer(movedirectx, movedirecty);
    }
    else if(gamemap->cratenum != 0){
        switch(e->key())
        {
        case Qt::Key_A:
        case Qt::Key_Left:
            nextmove = LEFT;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            nextmove = RIGHT;
            break;
        case Qt::Key_W:
        case Qt::Key_Up:
            nextmove = UP;
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            nextmove = DOWN;
            break;
        default:
            nextmove = 0;
            break;
        }
    }
}
void MainWindow::moveplayer(int directx, int directy)
{
    timer = new QTimer;
    int direct = 0;
    if(directx == -1 && directy == 0){
        direct = LEFT;
        maplabel[current_position.y][current_position.x]->setPixmap(GoLeft[2]);
    }
    else if(directx == 1 && directy == 0){
        direct = RIGHT;
        maplabel[current_position.y][current_position.x]->setPixmap(GoRight[2]);
    }
    else if(directx == 0 && directy == -1){
        direct = UP;
        maplabel[current_position.y][current_position.x]->setPixmap(GoUp[2]);
    }
    else if(directx == 0 && directy == 1){
        direct = DOWN;
        maplabel[current_position.y][current_position.x]->setPixmap(GoDown[2]);
    }
    else {
        direct = 0;
    }
    connect(timer, &QTimer::timeout, this, [this, direct]{walk(direct);});
    //switch (direct)
    //{
    //case LEFT:
        //maplabel[current_position.y][current_position.x]->setPixmap(GoLeft[2]);
        if(gamemap->mapdata[current_position.y+directy][current_position.x+directx] == GROUND || gamemap->mapdata[current_position.y+directy][current_position.x+directx] == GOAL)
        {

            //maplabel[current_position.y][current_position.x-1]->setPixmap(PLeft[2]);
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y+directy][current_position.x+directx] == CRATES && (gamemap->mapdata[current_position.y+directy*2][current_position.x+directx*2] == GROUND || gamemap->mapdata[current_position.y+directy*2][current_position.x+directx*2] == GOAL)) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y+directy][current_position.x+directx] == COIN) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            coinnum++;
            coinnumlabel->setNum(coinnum);
        }
        else if (gamemap->mapdata[current_position.y+directy][current_position.x+directx] == STAR) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            gamemap->cratenum = 0;
        }
    /*    break;
    case RIGHT:
        maplabel[current_position.y][current_position.x]->setPixmap(GoRight[2]);
        if(gamemap->mapdata[current_position.y][current_position.x+1] == GROUND || gamemap->mapdata[current_position.y][current_position.x+1] == GOAL)
        {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y][current_position.x+1] == CRATES && (gamemap->mapdata[current_position.y][current_position.x+2] == GROUND || gamemap->mapdata[current_position.y][current_position.x+2] == GOAL)) {

            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y][current_position.x+1] == COIN) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            coinnum++;
            coinnumlabel->setNum(coinnum);
        }
        else if (gamemap->mapdata[current_position.y][current_position.x+1] == STAR) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            gamemap->cratenum = 0;
        }
        break;
    case UP:
        maplabel[current_position.y][current_position.x]->setPixmap(GoUp[2]);
        if(gamemap->mapdata[current_position.y-1][current_position.x] == GROUND || gamemap->mapdata[current_position.y-1][current_position.x] == GOAL)
        {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y-1][current_position.x] == CRATES && (gamemap->mapdata[current_position.y-2][current_position.x] == GROUND || gamemap->mapdata[current_position.y-2][current_position.x] == GOAL)) {

            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y-1][current_position.x] == COIN) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            coinnum++;
            coinnumlabel->setNum(coinnum);
        }
        else if (gamemap->mapdata[current_position.y-1][current_position.x] == STAR) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            gamemap->cratenum = 0;
        }
        break;
    case DOWN:
        maplabel[current_position.y][current_position.x]->setPixmap(GoDown[2]);
        if(gamemap->mapdata[current_position.y+1][current_position.x] == GROUND || gamemap->mapdata[current_position.y+1][current_position.x] == GOAL)
        {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
        }
        else if (gamemap->mapdata[current_position.y+1][current_position.x] == CRATES && (gamemap->mapdata[current_position.y+2][current_position.x] == GROUND || gamemap->mapdata[current_position.y+2][current_position.x] == GOAL)) {

            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            //current_position = {current_position.x, current_position.y+1};
        }
        else if (gamemap->mapdata[current_position.y+1][current_position.x] == COIN) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            coinnum++;
            coinnumlabel->setNum(coinnum);
        }
        else if (gamemap->mapdata[current_position.y+1][current_position.x] == STAR) {
            timeactive = true;
            timer->start(10);
            timecount = 0;
            movenum++;
            movenumlabel->setNum(movenum);
            gamemap->cratenum = 0;
        }
        break;
    default:
        break;
    }*/

}
void MainWindow::walk(int dir)
{
    int nextmovex,nextmovey;
    switch(nextmove){
    case LEFT:
        nextmovex = -1;
        nextmovey = 0;
        break;
    case RIGHT:
        nextmovex = 1;
        nextmovey = 0;
        break;
    case UP:
        nextmovex = 0;
        nextmovey = -1;
        break;
    case DOWN:
        nextmovex = 0;
        nextmovey = 1;
        break;
    default:
        nextmovex = 0;
        nextmovey = 0;
        break;
    }
    switch (dir) {
    case LEFT:
        if(timecount == 0)
        {
            maplabel[current_position.y][current_position.x]->setPixmap(GoLeft[timecount + leg]);
            timer->start(60);
            timecount++;
        }
        else if(timecount == 1)
        {


            if (gamemap->mapdata[current_position.y][current_position.x-1] == CRATES)
            {
                if(gamemap->mapdata[current_position.y][current_position.x-2] == GOAL)
                    gamemap->cratenum--;
                if(gamemap->mapbackup[current_position.y][current_position.x-1] == GOAL)
                    gamemap->cratenum++;
                gamemap->mapdata[current_position.y][current_position.x-2] = CRATES;
                maplabel[current_position.y][current_position.x-2]->setPixmap(Crates);
            }
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x - 1, current_position.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(GoLeft[2]);
            if(leg == 0)
                leg = 1;
            else {
                leg = 0;
            }
            if(nextmove != dir)
            {
                timeactive = false;
                timer->stop();
                moveplayer(nextmovex, nextmovey);
            }
            else {
                timer->stop();
                timeactive = false;

                //nextmove = 0;

            }
            if(gamemap->cratenum == 0)
            {
                nextlevel->setEnabled(true);
                usetime->stop();
                if(current_stage < totallevel)
                    QMessageBox::information(this, tr(nullptr), tr("WIN!!"));
                else {
                    QMessageBox::information(this, tr(nullptr), tr("Congratulation!! You already pass all stages"));
                }
                if(movenum < maxstep[current_stage - 1])
                {
                    maxstep[current_stage - 1] = movenum;
                    maxstepnumlabel->setNum(maxstep[current_stage - 1]);
                }
            }
            else {
                if(nextlevel->isEnabled())
                    nextlevel->setEnabled(false);
            }
        }
        break;
    case RIGHT:

        if(timecount == 0)
        {
            maplabel[current_position.y][current_position.x]->setPixmap(GoRight[timecount + leg]);
            timer->start(60);
            timecount++;
        }
        else if(timecount == 1)
        {

            if (gamemap->mapdata[current_position.y][current_position.x+1] == CRATES)
            {
                if(gamemap->mapdata[current_position.y][current_position.x+2] == GOAL)
                    gamemap->cratenum--;
                if(gamemap->mapbackup[current_position.y][current_position.x+1] == GOAL)
                    gamemap->cratenum++;
                gamemap->mapdata[current_position.y][current_position.x+2] = CRATES;
                maplabel[current_position.y][current_position.x+2]->setPixmap(Crates);
            }
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x + 1, current_position.y};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(GoRight[2]);
            if(leg == 0)
                leg = 1;
            else {
                leg = 0;
            }
            if(nextmove != dir)
            {
                timeactive = false;
                timer->stop();
                moveplayer(nextmovex, nextmovey);
            }
            else {
                timer->stop();
                timeactive = false;

                //nextmove = 0;

            }
            if(gamemap->cratenum == 0)
            {
                nextlevel->setEnabled(true);
                usetime->stop();
                if(current_stage < totallevel)
                    QMessageBox::information(this, tr(nullptr), tr("WIN!!"));
                else {
                    QMessageBox::information(this, tr(nullptr), tr("Congratulation!! You already pass all stages"));
                }
                if(movenum < maxstep[current_stage - 1])
                {
                    maxstep[current_stage - 1] = movenum;
                    maxstepnumlabel->setNum(maxstep[current_stage - 1]);
                }
            }
            else {
                if(nextlevel->isEnabled())
                    nextlevel->setEnabled(false);
            }
        }
        break;
    case UP:

        if(timecount == 0)
        {
            maplabel[current_position.y][current_position.x]->setPixmap(GoUp[timecount + leg]);
            timer->start(60);
            timecount++;
        }
        else if(timecount == 1)
        {

            if (gamemap->mapdata[current_position.y-1][current_position.x] == CRATES)
            {
                if(gamemap->mapdata[current_position.y-2][current_position.x] == GOAL)
                    gamemap->cratenum--;
                if(gamemap->mapbackup[current_position.y-1][current_position.x] == GOAL)
                    gamemap->cratenum++;
                gamemap->mapdata[current_position.y - 2][current_position.x] = CRATES;
                maplabel[current_position.y - 2][current_position.x]->setPixmap(Crates);
            }
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x, current_position.y - 1};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(GoUp[2]);
            if(leg == 0)
                leg = 1;
            else {
                leg = 0;
            }
            if(nextmove != dir)
            {
                timeactive = false;
                timer->stop();
                moveplayer(nextmovex, nextmovey);
            }
            else {
                timer->stop();
                timeactive = false;

                //nextmove = 0;

            }
            if(gamemap->cratenum == 0)
            {
                nextlevel->setEnabled(true);
                usetime->stop();
                if(current_stage < totallevel)
                    QMessageBox::information(this, tr(nullptr), tr("WIN!!"));
                else {
                    QMessageBox::information(this, tr(nullptr), tr("Congratulation!! You already pass all stages"));
                }
                if(movenum < maxstep[current_stage - 1])
                {
                    maxstep[current_stage - 1] = movenum;
                    maxstepnumlabel->setNum(maxstep[current_stage - 1]);
                }
            }
            else {
                if(nextlevel->isEnabled())
                    nextlevel->setEnabled(false);
            }
        }
        break;
    case DOWN:

        if(timecount == 0)
        {
            maplabel[current_position.y][current_position.x]->setPixmap(GoDown[timecount + leg]);
            timer->start(60);
            timecount++;
        }
        else if(timecount == 1)
        {

            if (gamemap->mapdata[current_position.y+1][current_position.x] == CRATES)
            {
                if(gamemap->mapdata[current_position.y+2][current_position.x] == GOAL)
                    gamemap->cratenum--;
                if(gamemap->mapbackup[current_position.y+1][current_position.x] == GOAL)
                    gamemap->cratenum++;
                gamemap->mapdata[current_position.y+2][current_position.x] = CRATES;
                maplabel[current_position.y+2][current_position.x]->setPixmap(Crates);
            }
            if(gamemap->mapbackup[current_position.y][current_position.x] == GOAL)
            {
                gamemap->mapdata[current_position.y][current_position.x] = GOAL;
                maplabel[current_position.y][current_position.x]->setPixmap(Goal);
            }
            else {
                gamemap->mapdata[current_position.y][current_position.x] = GROUND;
                maplabel[current_position.y][current_position.x]->setPixmap(Ground);
            }
            current_position = {current_position.x, current_position.y + 1};
            gamemap->mapdata[current_position.y][current_position.x] = PLAYER;
            maplabel[current_position.y][current_position.x]->setPixmap(GoDown[2]);
            if(leg == 0)
                leg = 1;
            else {
                leg = 0;
            }
            if(nextmove != dir)
            {
                timeactive = false;
                timer->stop();
                moveplayer(nextmovex, nextmovey);
            }
            else {
                timer->stop();
                timeactive = false;

                //nextmove = 0;

            }
            if(gamemap->cratenum == 0)
            {
                nextlevel->setEnabled(true);
                usetime->stop();
                if(current_stage < totallevel)
                    QMessageBox::information(this, tr(nullptr), tr("WIN!!"));
                else {
                    QMessageBox::information(this, tr(nullptr), tr("Congratulation!! You already pass all stages"));
                }
                if(movenum < maxstep[current_stage - 1])
                {
                    maxstep[current_stage - 1] = movenum;
                    maxstepnumlabel->setNum(maxstep[current_stage - 1]);
                }
            }
            else {
                if(nextlevel->isEnabled())
                    nextlevel->setEnabled(false);
            }
        }
        break;
    default:
        break;
    }
    mainWidget->update();

}
