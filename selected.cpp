#include "selected.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "choosingwindow.h"
#include <QPushButton>
#include <QMessageBox>

Selected::Selected(QWidget *parent) : QScrollArea(parent)
{
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3); // 增加一列用于放置删除按钮
    tableWidget->setHorizontalHeaderLabels({"删除", "课程名称", "授课教师"});

    // 设置列宽调整策略为自动适应内容
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    setWidget(tableWidget);
    setWidgetResizable(true);
}

void Selected::setSelectedCourses(const std::vector<Course>& courses)
{
    selectedCourses.clear();

    for (const auto& course : courses) {
        if (course.chosen) {
            selectedCourses.push_back(course);
        }
    }

    tableWidget->setRowCount(selectedCourses.size());
    deleteButtons.clear(); // 清空之前的按钮

    tableWidget->setColumnCount(4); // 从3列改为4列
    tableWidget->setHorizontalHeaderLabels({"删除", "课程名称", "授课教师", "课程类别"});

    for (size_t i = 0; i < selectedCourses.size(); ++i) {
        const Course& course = selectedCourses[i];

        QPushButton *button = new QPushButton("删除", tableWidget);
        deleteButtons.push_back(button);
        tableWidget->setCellWidget(i, 0, button);
        connect(button, &QPushButton::clicked, [this, i]() {
            onDeleteButtonClicked(i);
        });

        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(course.strCourseName)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(course.strTeacher)));
        QString categoryStr = choosingWindow::mapCategoryToString(selectedCourses[i].iCourseCategory);
        tableWidget->setItem(i, 3, new QTableWidgetItem(categoryStr));

        // 设置行的背景颜色，一行白色一行灰色
        for (int j = 0; j < 3; ++j) {
            if (i % 2 == 0) {
                if (tableWidget->item(i, j)) {
                    tableWidget->item(i, j)->setBackground(QBrush(Qt::white));
                }
            } else {
                if (tableWidget->item(i, j)) {
                    tableWidget->item(i, j)->setBackground(QBrush(Qt::lightGray));
                }
            }
        }
    }

    // 最后再次调整列宽，确保所有内容都能完整显示
    tableWidget->resizeColumnsToContents();
}

// 新增：实现获取已选课程列表的方法
const std::vector<Course>& Selected::getSelectedCourses() const
{
    return selectedCourses;
}

void Selected::onDeleteButtonClicked(int row)
{
    emit courseDeleted(row); // 发出课程被删除的信号
}

Selected::~Selected()
{
    delete tableWidget;
    for (QPushButton *button : deleteButtons) {
        delete button;
    }
}
