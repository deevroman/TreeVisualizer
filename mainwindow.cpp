#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPushButton>
#include "Trees.h"
#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QBoxLayout>
#include <QSizePolicy>
#include <QLayout>
#include <QLineEdit>
#include <vector>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TreeViewScroll = new QScrollArea(this);
    TreeViewScroll->move(148, 0);

    TreeViewScene = new QGraphicsScene;
    TreeView = new QGraphicsView(TreeViewScene);

    QGraphicsRectItem* item1 = new QGraphicsRectItem(0,0,50,50);
    item1->setBrush(QBrush(Qt::red));
    TreeViewScene->addItem(item1);

    TreeViewScroll->setMinimumSize(this->width() - 148, this->height() - 48);
    TreeViewScroll->setWidgetResizable(true);
    TreeViewScroll->setWidget(TreeView);
    TreeView->show();

    this->setMinimumSize(this->size());

    LeftBar = new QWidget(this);

    LeftBar->setMinimumSize(148, this->height());
    LeftBar->setStyleSheet("QWidget{background-color:gray}");

    auto GLayout = new QGridLayout();
    input = new QLineEdit();
    input->setMinimumSize(30, 30);
    GLayout->addWidget(input, 0, 0);
    enter = new QPushButton("ok");
    enter->resize(30, 30);
    enter->setMaximumSize(30, 30);
    GLayout->addWidget(enter, 0, 1);
    connect(enter, SIGNAL(clicked()), this, SLOT(clickEnterButton()));

    historyScroll = new QScrollArea();
    GLayout->addWidget(historyScroll, 1, 0, 1, 2);

    history = new QLabel(historyScroll);
    history->setText(trUtf8("История"));
    historyScroll->setWidget(history);
    LeftBar->setLayout(GLayout);

    input->setFocus();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickEnterButton(){
    QString text = input->text();
    QStringList text_spl = text.split(" ");
    int i = 0;
    bool ok = true;
    for(auto now : text_spl){
        if(i == 0){
            if(now != "insert"){
                ok = false;
                break;
            }
        } else{
            for(auto now2 : now){
                if(!now2.isDigit()){
                    ok = false;
                    break;
                }
            }
        }
        i++;
    }
    if(!ok)
        return; // todo
    //todo для всех элементов
    history->setText(history->text() + "\n" + text_spl.front());
    text_spl.pop_front();
    for(auto now : text_spl){
        //todo
        AVL.insert(now.toInt());
        history->setText(history->text() + " " + now);
    }
    history->adjustSize();
    paintTree();
}

void MainWindow::paintTree(){
    auto stat = AVL.getHeightStat();
    int height = SIZE_BORDER * 2 + SIZE_NODE * 2 * stat.size();
    int width = SIZE_BORDER * 2 + 2 * SIZE_NODE * (1<<stat.size());

    history->adjustSize();

    return;
    list_buttons.clear();
    paintAVLTree(AVL.root, SIZE_BORDER, width / 2 - SIZE_NODE / 2, stat.size() - 1);

    TreeViewScroll->setWidget(TreeView);
    TreeView->setMinimumSize(width, height);
    TreeView->setStyleSheet("QWidget{background-color:green}");
}


void MainWindow::paintAVLTree(AVLTree<int>::node* now, int x, int y, int level){
    if(now == nullptr){
        return;
    }
    list_buttons.push_back(new QPushButton(QString::number(now->key), TreeView));
    list_buttons.back()->resize(SIZE_NODE, SIZE_NODE);
    list_buttons.back()->move(y, x);

    if(now->left != nullptr){
        paintAVLTree(now->left, x + SIZE_NODE*2, y - (1<<level)*SIZE_NODE, level - 1);
    }
    if(now->right != nullptr){
        paintAVLTree(now->right, x + SIZE_NODE*2, y + (1<<level)*SIZE_NODE, level - 1);
    }
}

