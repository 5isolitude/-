#include <QApplication>
#include "StartWindow.h"
#include "CourseRepository.h"
#include "Course.h"
#include "CourseManager.h"
#include "filter.h"
#include <regex>
#include <vector>
#include <iomanip>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


void teammateView();


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    StartWindow *p=new StartWindow();
    p->show();



    return app.exec();
}
