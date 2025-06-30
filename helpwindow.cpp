#include "HelpWindow.h"
#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>

helpWindow::helpWindow(QWidget *parent) : QWidget(parent)
{
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    this->resize(800,300);
    this->setWindowTitle("帮助&功能文档");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);

    QFile file(":/txt/help.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        textEdit->setPlainText(content);
        file.close();
    }
}

helpWindow::~helpWindow()
{
    delete textEdit;
}
