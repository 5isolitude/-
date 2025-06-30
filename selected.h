#ifndef SELECTED_H
#define SELECTED_H

#include <QScrollArea>
#include <QTableWidget>
#include <vector>
#include "course.h"
#include<QPushButton>

class Selected : public QScrollArea
{
    Q_OBJECT

public:
    explicit Selected(QWidget *parent = nullptr);
    ~Selected();
    std::vector<Course> selectedCourses;

    void setSelectedCourses(const std::vector<Course>& courses);

    // 新增：获取已选课程列表
    const std::vector<Course>& getSelectedCourses() const;

signals:
    void courseDeleted(int index); // 新增信号，用于通知外部课程被删除

private slots:
    void onDeleteButtonClicked(int row); // 新增槽函数，处理删除按钮点击事件

private:
    QTableWidget *tableWidget;
    std::vector<QPushButton*> deleteButtons; // 新增，存储删除按钮的向量
};

#endif // SELECTED_H
