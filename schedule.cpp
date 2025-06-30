#include "schedule.h"
#include "ui_schedule.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QFont>
#include <QColor>

schedule::schedule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::schedule)
{
    ui->setupUi(this);
    this->resize(1200, 800);
    this->setWindowTitle("课程表");

    // 创建表格控件
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(7); // 周一至周日
    tableWidget->setRowCount(13);   // 12节课 + 时间行

    // 设置表头
    QStringList days = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};
    tableWidget->setHorizontalHeaderLabels(days);

    // 设置行表头（节次和时间）
    QStringList periods;
    periods << "节次" << "1\n08:00\n08:50" << "2\n09:00\n09:50"
            << "3\n10:00\n10:50" << "4\n11:00\n11:50" << "5\n13:00\n13:50"
            << "6\n14:00\n14:50" << "7\n15:10\n16:00" << "8\n16:10\n17:00"
            << "9\n17:10\n18:00" << "10\n18:40\n19:30" << "11\n19:40\n20:30"
            << "12\n20:40\n21:30";
    tableWidget->setVerticalHeaderLabels(periods);

    // 设置表格样式
    tableWidget->setStyleSheet(
        "QTableWidget {"
        "   gridline-color: #d0d0d0;"
        "   background-color: white;"
        "}"
        "QHeaderView::section {"
        "   background-color: #f0f8ff;"
        "   padding: 4px;"
        "   border: 1px solid #d0d0d0;"
        "   font-weight: bold;"
        "}"
        );

    // 设置表格属性
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);

    // 设置行高和列宽
    tableWidget->verticalHeader()->setDefaultSectionSize(80);
    tableWidget->horizontalHeader()->setDefaultSectionSize(150);

    // 设置布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->addWidget(tableWidget);
    setLayout(layout);
}

schedule::~schedule()
{
    delete ui;
}

void schedule::setSelectedCourses(const std::vector<Course>& courses)
{
    //this->selectedCourses = courses; // 直接设置已选课程
    selectedCourses.clear();

    for (const auto& course : courses) {
        if (course.chosen) {
            selectedCourses.push_back(course);
        }
    }

}

void schedule::showCourseSchedule()
{
    // 清空表格内容（保留表头）
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            if (tableWidget->item(row, col)) {
                delete tableWidget->item(row, col);
            }
            tableWidget->setItem(row, col, nullptr);
        }
    }

    // 填充课程数据
    for (const Course& course : selectedCourses) {
        QString courseText = QString::fromStdString(course.strCourseName + "\n" + course.strClassroom);

        for (int day : course.Classtime.weekdays) {
            if (day < 1 || day > 7) continue; // 确保星期值有效

            for (int period : course.Classtime.classPeriods) {
                if (period < 1 || period > 12) continue; // 确保节次值有效

                // 课程在表格中的位置（行=节次，列=星期）
                int row = period; // 第0行是标题行，所以节次1对应第1行
                int col = day - 1; // 周一=0, 周二=1, ..., 周日=6

                if (row < tableWidget->rowCount() && col < tableWidget->columnCount()) {
                    QTableWidgetItem* item = new QTableWidgetItem(courseText);
                    item->setTextAlignment(Qt::AlignCenter);
                    item->setFont(QFont("Microsoft YaHei", 10));
                    item->setBackground(QColor(225, 240, 255));
                    tableWidget->setItem(row, col, item);
                }
            }
        }
    }
}
