#include "StartWindow.h"
#include "ui_StartWindow.h"
#include "choosingwindow.h"
#include <QPalette>
#include <QBrush>
#include <QPixmap>
#include <QResizeEvent>

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(400,650);

    //设置背景图片
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/back/pkuBack.jpg")));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setWindowTitle("开始窗口");



    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);

    connect(ui->newChoosingButton, &QPushButton::clicked, this, &StartWindow::newChoosing);

    connect(ui->h_choosingButton, &QPushButton::clicked, this, [=](){
        choosingWindow *p = new choosingWindow(nullptr); // 创建一个独立窗口，不设置父窗口
        p->setMainWindow(this); // 传递主窗口指针给choosingWindow
        p->getMode(1);
        p->readFromHistory();
        p->show();
        this->hide();

    });


}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::newChoosing(){
    choosingWindow *p = new choosingWindow(nullptr); // 创建一个独立窗口，不设置父窗口
    p->setMainWindow(this); // 传递主窗口指针给choosingWindow
    p->show();
    this->hide();
}

// 重写 resizeEvent 确保背景图片随窗口大小调整
void StartWindow::resizeEvent(QResizeEvent *event)
{
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/back/pkuBack.jpg").scaled(
                                           this->size(),
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation
                                           )));
    this->setPalette(palette);
    QMainWindow::resizeEvent(event);
}
