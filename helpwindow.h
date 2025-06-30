#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>

class helpWindow : public QWidget
{
    Q_OBJECT

public:
    explicit helpWindow(QWidget *parent = nullptr);
    ~helpWindow();

private:
    QTextEdit *textEdit;
};

#endif // HELPWINDOW_H
