#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QLCDNumber>
#include "map.h"
#ifdef totallevel
#undef totallevel
#endif
#define totallevel 2
namespace Ui {
class MainWindow;
}
enum{LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setResource();
    void keyPressEvent(QKeyEvent *e);
    //void keyReleaseEvent(QKeyEvent *e);
    void moveplayer(int , int);

    ~MainWindow();
private:
    Ui::MainWindow *ui;
    point current_position;

    QLabel ***maplabel;
    QLabel ***mapground;
    QLabel *movenumlabel;
    QLabel *coinnumlabel;
    QLabel *maxstepnumlabel;
    QWidget *mainWidget;

    QPixmap Wall;
    QPixmap Ground;
    QPixmap Crates;
    QPixmap Gamer;
    QPixmap Goal;
    QPixmap GoLeft[3];
    QPixmap GoUp[3];
    QPixmap GoRight[3];
    QPixmap GoDown[3];
    QPixmap Coin;
    QPixmap Star;

    Map *gamemap;
    QTimer *timer;
    QTimer *usetime;
    QTime basetime;
    QPushButton *nextlevel;
    QLCDNumber *lcd;
    int height;
    int width;
    int current_stage;
    int maxstage;
    bool timeactive;
    int timecount;
    int leg;
    int nextmove;
    int movenum;
    int coinnum;
    int maxtime[totallevel];
    int maxstep[totallevel];
private slots:
    void newgame();
    void select();
    void setmenu();
    void restartgame();
    void selectgame(int);
    void walk(int);
    void updatetime();
    //void close();
};

#endif // MAINWINDOW_H
