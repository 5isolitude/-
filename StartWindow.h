#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include "choosingwindow.h"
#include <QResizeEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
    void resizeEvent(QResizeEvent *event);
public slots:
    void newChoosing();

private:
    Ui::MainWindow *ui;
};
#endif // STARTWINDOW_H
