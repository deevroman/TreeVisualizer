#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVector>
#include "Trees.h"
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene* TreeViewScene;
    QGraphicsView* TreeView;
    QScrollArea* TreeViewScroll;
    QScrollArea* historyScroll;
    QPushButton* enter;
    QLineEdit* input;
    QWidget* LeftBar;
    QLabel* history;
    QVector<QPushButton*> list_buttons;
    AVLTree<int> AVL;
    int SIZE_NODE = 25;
    int SIZE_BORDER = 25;
private:
    Ui::MainWindow *ui;
    void paintTree();
    void paintAVLTree(AVLTree<int>::node*, int, int, int);
private slots:
    void clickEnterButton();
};
#endif // MAINWINDOW_H
