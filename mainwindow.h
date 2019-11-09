#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVector>
#include <QGroupBox>
#include "Trees.h"
#include <QGraphicsScene>
#include <QRadioButton>
#include "NodeButton.h"
#include <QGraphicsView>
#include <QSlider>
#include <QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene* TreeViewScene;
    QGraphicsView* TreeView;
    QScrollArea* historyScroll;
    QPushButton* enterInput;
    QPushButton* clearHistory;
    QPushButton* clearTrees;
    QLineEdit* inputKey;
    QPushButton* enterDelete;
    QWidget* LeftBar;
    QGroupBox* selectorTree;
    QRadioButton* AVLButton;
    QRadioButton* RBButton;
    QRadioButton* SplayButton;
    QSlider* zoomer;
    long double lastScale = 1;
    QLabel* history;
    QVector<NodeButton*> list_buttons;
    QVector<QGraphicsLineItem*> list_lines;
    AVLTree<int> AVL;
    SplayTree<int> Splay;
    RedBlackTree<int> RBTree;
    const int CONST_SIZE_NODE = 30;
    int SIZE_NODE = 30;
    const int SIZE_BORDER = 30;
private:
    template <typename T>
    void PaintCurTree(T now, int x, int y);
    void paintTree();
private slots:
    void clickEnterInsertButton();
    void clickEnterDeleteButton();
    void clickDeleteNode(int nodeKey);
    void clickClearHistory();

    QColor getNodeColor(AVLTree<int>::node *cur);
    QColor getNodeColor(SplayTree<int>::node *cur);
    QColor getNodeColor(RedBlackTree<int>::node *cur);

    bool checkInput(const QString s);

    void checkInputChanged();

    bool is_leaf(RedBlackTree<int>::node *cur);

    bool is_leaf(SplayTree<int>::node *cur);

    bool is_leaf(AVLTree<int>::node *cur);

    void Zoomer(int value);

    void clickClearTrees();
};
#endif // MAINWINDOW_H
