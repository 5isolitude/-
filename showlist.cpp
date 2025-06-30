#include "showlist.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "choosingwindow.h"
#include <QPushButton>
#include <QMessageBox>

std::string formatClassTime_2(const ClassTime& time) {
    std::stringstream ss;
    if (time.startWeek==0&&time.endWeek==0){
        ss<<"无";
        return ss.str();
    }
    ss << time.startWeek << "~" << time.endWeek << "周 星期";

    // 星期几
    for (size_t i = 0; i < time.weekdays.size(); ++i) {
        if (i > 0) ss << " ";
        int num=time.weekdays[i];
        switch (num){
        case 1:ss<<"一";break;
        case 2:ss<<"二";break;
        case 3:ss<<"三";break;
        case 4:ss<<"四";break;
        case 5:ss<<"五";break;
        case 6:ss<<"六";break;
        case 7:ss<<"七";break;
        }
        ss<<" ";
        //ss << "星期" << time.weekdays[i];
    }

    ss << " ";

    // 节次范围
    ss << "第";
    for (size_t i = 0; i < time.classPeriods.size(); ++i) {
        if (i > 0) ss << "~";
        ss << time.classPeriods[i];
    }
    ss << "节课";

    return ss.str();
}

ShowList::ShowList(QWidget *parent) : QScrollArea(parent)
{
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(12); // 增加一列用于放置按钮
    tableWidget->setHorizontalHeaderLabels({"选择", "课程号", "课程名称", "课程类别", "学分", "授课教师", "班号", "开课单位", "年级", "上课时间", "教室", "备注"});

    // 设置列宽调整策略为自动适应内容
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 允许用户手动调整列宽
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(11, QHeaderView::Interactive);

    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    setWidget(tableWidget);
    setWidgetResizable(true);
}

void ShowList::setCourses(const std::vector<Course>& courses)
{
    this->courses = courses;
    tableWidget->setRowCount(courses.size());
    buttons.clear(); // 清空之前的按钮

    for (size_t i = 0; i < courses.size(); ++i) {
        const Course& course = courses[i];

        QPushButton *button = new QPushButton("选择", tableWidget);
        buttons.push_back(button);
        tableWidget->setCellWidget(i, 0, button);
        connect(button, &QPushButton::clicked, [this, i]() {
            emit buttonClicked(i);
            this->onButtonClicked(i); // 调用槽函数
        });

        QString categoryStr = choosingWindow::mapCategoryToString(course.iCourseCategory);
        auto item = new QTableWidgetItem(categoryStr);


        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(course.icourseID)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(course.strCourseName)));
        tableWidget->setItem(i, 3, item); // 假设类别在第3列
        // tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(course.iCourseCategory)));
        tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(course.iCredit)));
        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(course.strTeacher)));
        tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(course.iClassIndex)));
        tableWidget->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(course.strDepartment)));
        tableWidget->setItem(i, 8, new QTableWidgetItem(QString::number(course.iGrade)));
        tableWidget->setItem(i, 9, new QTableWidgetItem(QString::fromStdString(formatClassTime_2(course.Classtime))));
        tableWidget->setItem(i, 10, new QTableWidgetItem(QString::fromStdString(course.strClassroom)));
        tableWidget->setItem(i, 11, new QTableWidgetItem(QString::fromStdString(course.strRemark)));

        // 设置行的背景颜色，一行白色一行灰色
        for (int j = 0; j < 12; ++j) {
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

void ShowList::onButtonClicked(int row)
{
    const Course& course = courses[row];
    QString message = QString("是否确定选择课程 %1 (%2)？").arg(QString::fromStdString(course.strCourseName), QString::fromStdString(course.icourseID));
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认选择", message, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit courseSelected(row);
    }
}

ShowList::~ShowList()
{
    delete tableWidget;
    for (QPushButton *button : buttons) {
        delete button;
    }
}


void ShowList::highlightConflictingCourses(const std::vector<Course>& selectedCourses)
{
    CourseManager manager; // 假设 CourseManager 类中有 isTimeConflict 函数
    for (size_t i = 0; i < courses.size(); ++i) {
        bool hasConflict = false;
        for (const auto& selectedCourse : selectedCourses) {
            if (manager.isTimeConflict(courses[i].Classtime, selectedCourse.Classtime)) {
                hasConflict = true;
                break;
            }
        }
        for (int j = 1; j < tableWidget->columnCount(); ++j) { // 跳过第一列的按钮
            if (tableWidget->item(i, j)) {
                if (hasConflict) {
                    tableWidget->item(i, j)->setForeground(QBrush(Qt::red));
                } else {
                    tableWidget->item(i, j)->setForeground(QBrush(Qt::black));
                }
            }
        }
    }
}
