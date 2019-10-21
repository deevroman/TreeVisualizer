#include "mainwindow.h"
#include <QPushButton>
#include "Trees.h"
#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QLineEdit>
#include <vector>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "NodeButton.h"
#include <QGraphicsRectItem>
#include <QHBoxLayout>
#include <QGroupBox>
#include <algorithm>
#include <QMovie>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1248, 748);
    auto CentralGridLayot = new QGridLayout();

    TreeViewScene = new QGraphicsScene;
    TreeView = new QGraphicsView(TreeViewScene, this);
    CentralGridLayot->addWidget(TreeView, 0, 1);
    TreeView->move(148, 0);

    this->setMinimumSize(this->size());

    LeftBar = new QWidget(this);
    CentralGridLayot->addWidget(LeftBar, 0, 0);

    LeftBar->setMinimumSize(148, this->height());
    LeftBar->setMaximumWidth(148);
    LeftBar->setStyleSheet("QWidget{background-color:gray}");

    auto GLayout = new QGridLayout();
    GLayout->setContentsMargins(10, 10, 10, 10);
    selectorTree = new QGroupBox("Тип дерева");
    GLayout->addWidget(selectorTree, 0, 0, 1, 2);
    auto selectorLayout = new QGridLayout();
    AVLButton = new QRadioButton(trUtf8("АВЛ"));
    RBButton = new QRadioButton(trUtf8("Красно-черное"));
    SplayButton = new QRadioButton(trUtf8("Splay"));
    AVLButton->setChecked(true);
    connect(AVLButton, &QRadioButton::clicked, this, &MainWindow::paintTree);
    connect(RBButton, &QRadioButton::clicked, this, &MainWindow::paintTree);
    connect(SplayButton, &QRadioButton::clicked, this, &MainWindow::paintTree);
    selectorLayout->addWidget(AVLButton);
    selectorLayout->addWidget(RBButton);
    selectorLayout->addWidget(SplayButton);
    selectorTree->setLayout(selectorLayout);
    selectorLayout->setContentsMargins(0, 0, 0, 0);

    inputKey = new QLineEdit();
    inputKey->setMinimumHeight(35);
    connect(inputKey, SIGNAL(returnPressed()), this, SLOT(clickEnterInsertButton()));
    connect(inputKey, SIGNAL(textChanged(QString)), this, SLOT(checkInputChanged()));
    GLayout->addWidget(inputKey, 1, 0, 1, 2);

    auto inputButtonsLayout = new QHBoxLayout();
    enterInput = new QPushButton("Insert");
    enterInput->resize(40, 30);
    enterInput->setMaximumSize(42, 35);
    inputButtonsLayout->addWidget(enterInput);
    connect(enterInput, SIGNAL(clicked()), this, SLOT(clickEnterInsertButton()));
    enterDelete = new QPushButton("Delete");
    enterDelete->resize(40, 30);
    enterDelete->setMaximumSize(42, 30);
    inputButtonsLayout->addWidget(enterDelete);
    connect(enterDelete, SIGNAL(clicked()), this, SLOT(clickEnterDeleteButton()));
    GLayout->addLayout(inputButtonsLayout, 2, 0, 1, 2);

    historyScroll = new QScrollArea();
    GLayout->addWidget(historyScroll, 3, 0, 1, 2);
    clearHistory = new QPushButton(trUtf8("Очистить историю"));
    GLayout->addWidget(clearHistory, 4, 0, 1, 2);
    connect(clearHistory, SIGNAL(clicked()), this, SLOT(clickClearHistory()));
    history = new QLabel(historyScroll);
    history->setText(trUtf8("История"));
    historyScroll->setWidget(history);
    LeftBar->setLayout(GLayout);
    TreeView->setStyleSheet("QWidget{background-color:green}");
    TreeView->setMinimumSize(this->width() - 148, this->height());
//    TreeView->setMaximumSize(this->width() - 148, this->height());
    TreeView->show();
    auto window = new QWidget;
    CentralGridLayot->setContentsMargins(0, 0, 0, 0);
    CentralGridLayot->setSpacing(0);
    window->setLayout(CentralGridLayot);
    setCentralWidget(window);
    inputKey->setFocus();
}

MainWindow::~MainWindow() = default;

bool MainWindow::checkInput(const QString s){
    QStringList text_spl = s.split(" ",  QString::SkipEmptyParts);
    bool ok = s.size() != 0;
    for(const auto& now : text_spl){
        now.toInt(&ok);
        if(!ok){
            return false;
        }

    }
    return ok;
}

void MainWindow::checkInputChanged(){
    if(checkInput(inputKey->text())){
        inputKey->setStyleSheet("QLineEdit{color : white}");
    } else {
        inputKey->setStyleSheet("QLineEdit{color : red}");
    }
}

void MainWindow::clickEnterInsertButton(){
    QString text = inputKey->text();

    QStringList text_spl = text.split(" ",  QString::SkipEmptyParts);
    if(!checkInput(text)) {
        inputKey->setStyleSheet("QLineEdit{color : red}");
        return;
    } else {
        inputKey->setText("");
        inputKey->setStyleSheet("QLineEdit{color : white}");
    }
    history->setText(history->text() + "\ninsert");
    for(const auto& now : text_spl){
        AVL.insert(now.toInt());
        Splay.insert(now.toInt());
        RBTree.insert(now.toInt());
        history->setText(history->text() + " " + now);
    }
    history->adjustSize();
    paintTree();
}

void MainWindow::clickEnterDeleteButton() {
    QString text = inputKey->text();
    QStringList text_spl = text.split(" ",  QString::SkipEmptyParts);
    if(!checkInput(text)) {
        inputKey->setStyleSheet("QLineEdit{color : red}");
        return;
    } else {
        inputKey->setText("");
        inputKey->setStyleSheet("QLineEdit{color : white}");
    }
    history->setText(history->text() + "\ndelete");
    for(const auto& now : text_spl){
        AVL.remove(now.toInt());
        Splay.remove(now.toInt());
        RBTree.remove(now.toInt());
        history->setText(history->text() + " " + now);
    }
    history->adjustSize();
    paintTree();
}

void MainWindow::clickDeleteNode(int nodeKey) {
    history->setText(history->text() + trUtf8("\ndelete ") + QString::number(nodeKey));
    history->adjustSize();
    AVL.remove(nodeKey);
    Splay.remove(nodeKey);
    RBTree.remove(nodeKey);
    paintTree();
}

void MainWindow::paintTree(){
    std::vector<int> stat;
    for (const auto &item : list_buttons) {
        disconnect(item, SIGNAL(clickNode(int)), this, SLOT(clickDeleteNode(int)));
    }
    TreeViewScene->clear();
    list_buttons.clear();
    if(AVLButton->isChecked()){
        stat = getHeightStat(AVL.root);
        paintAVLTree(AVL.root, 0, 0, std::max(0, (int)stat.size()-2));
    } else if(SplayButton->isChecked()){
        stat = getHeightStat(Splay.root);
        paintAVLTree(Splay.root, 0, 0, std::max(0, (int)stat.size()-2));
    } else if(RBButton->isChecked() && !is_leaf(RBTree.root)){
        stat = getHeightStat(RBTree.root);
        paintAVLTree(RBTree.root, 0, 0, std::max(0, (int)stat.size()-2));
    }
}

template<typename T>
void MainWindow::paintAVLTree(T now, int x, int y, int level) {
    if(now == nullptr){
        return;
    }
    auto tmp_but = new NodeButton(QRectF(y, x, SIZE_NODE, SIZE_NODE));
    tmp_but->setBrush(QBrush(getNodeColor(now)));
    TreeViewScene->addItem(tmp_but);
    connect(tmp_but, SIGNAL(clickNode(int)), this, SLOT(clickDeleteNode(int)));
    auto tmp_text = TreeViewScene->addText(QString::number(now->key));
    tmp_text->setPos(y, x);

    list_buttons.push_back(tmp_but);
    tmp_but->nodePointer = now->key;

    QPoint left = QPoint(x + SIZE_NODE*2, y - SIZE_NODE);
    QPoint right = QPoint(x + SIZE_NODE*2, y + SIZE_NODE);
    if(now->left != nullptr && !is_leaf(now->left)){
        int mx = getSize<T>(now->left->right) + 1;
        int to_x = x + SIZE_NODE*2, to_y = y - (mx)*SIZE_NODE;
        list_lines.push_back(TreeViewScene->addLine(y + SIZE_NODE/2, x + SIZE_NODE, to_y + SIZE_NODE/2, to_x));
        paintAVLTree(now->left, to_x, to_y, level-1);
    }
    if(now->right != nullptr && !is_leaf(now->right)){
        int mx = getSize<T>(now->right->left) + 1;
        int to_x = x + SIZE_NODE*2, to_y = y + (mx)*SIZE_NODE;
        list_lines.push_back(TreeViewScene->addLine(y + SIZE_NODE/2, x + SIZE_NODE, to_y + SIZE_NODE/2, to_x));
        paintAVLTree(now->right, to_x, to_y, level-1);
    }
}

QColor MainWindow::getNodeColor(AVLTree<int>::node* cur){
    return Qt::black;
}

QColor MainWindow::getNodeColor(SplayTree<int>::node* cur){
    return Qt::black;
}

QColor MainWindow::getNodeColor(RedBlackTree<int>::node* cur){
    if (cur->color) {
        return Qt::red;
    } else {
        return Qt::black;
    }
}

bool MainWindow::is_leaf(RedBlackTree<int>::node* cur){
    return (cur->left == nullptr && cur->right == nullptr);
}
bool MainWindow::is_leaf(AVLTree<int>::node* cur){
    return false;
}
bool MainWindow::is_leaf(SplayTree<int>::node* cur){
    return false;
}

void MainWindow::clickClearHistory(){
    history->setText(trUtf8("История"));
    history->adjustSize();
}
