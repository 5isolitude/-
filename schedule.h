#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>
#include "coursemanager.h"
#include <QTableWidget>
#include <vector>

namespace Ui {
class schedule;
}

class schedule : public QWidget
{
    Q_OBJECT

public:
    explicit schedule(QWidget *parent = nullptr);
    ~schedule();
    void setSelectedCourses(const std::vector<Course>& courses); // 添加此函数
    void showCourseSchedule();

private:
    Ui::schedule *ui;
    std::vector<Course> selectedCourses; // 存储已选课程
    QTableWidget* tableWidget;
};

#endif // SCHEDULE_H
