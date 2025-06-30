// showlist.h
#ifndef SHOWLIST_H
#define SHOWLIST_H

#include <QScrollArea>
#include <QTableWidget>
#include <vector>
#include "course.h"
#include <QPushButton>

class ShowList : public QScrollArea
{
    Q_OBJECT

public:
    explicit ShowList(QWidget *parent = nullptr);
    ~ShowList();

    void setCourses(const std::vector<Course>& courses);
    void highlightConflictingCourses(const std::vector<Course>& selectedCourses);
    std::vector<Course> courses; // 存储课程信息
signals:
    void buttonClicked(int row); // 按钮点击信号
    void courseSelected(int row); // 课程选择确认信号

private slots:
    void onButtonClicked(int row); // 按钮点击槽函数

private:
    QTableWidget *tableWidget;
    std::vector<QPushButton*> buttons; // 存储按钮的向量
    //std::vector<Course> courses; // 存储课程信息
};

#endif // SHOWLIST_H
