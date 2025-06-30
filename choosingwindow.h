#ifndef CHOOSINGWINDOW_H
#define CHOOSINGWINDOW_H

#include <QWidget>
#include<vector>
#include<QLabel>
#include"course.h"
#include"coursemanager.h"
#include"showlist.h"
#include "selected.h"

namespace Ui {
class choosingWindow;
}

class choosingWindow : public QWidget
{
    Q_OBJECT

public:
    //类别映射函数
    static QString mapCategoryToString(int category) {
        static const QMap<int, QString> categoryMap = {
            {1, "通识课1"}, {2, "通识课2"}, {3, "通识课3"}, {4, "通识课4"},
            {5, "通识核心课1"}, {6, "通识核心课2"}, {7, "通识核心课3"}, {8, "通识核心课4"},
            {9, "全校任选"}, {10, "劳动教育课"},
            {11, "英语课A"}, {12, "英语课B"}, {13, "英语课C"}, {14, "英语课C+"},
            {15, "全校必修"}, {16, "专业必修"}, {17, "限选"}, {18, "实习"}
        };
        return categoryMap.value(category, "未知类别");
    }

    explicit choosingWindow(QWidget *parent = nullptr);
    ~choosingWindow();

    void setMainWindow(QWidget *mainWindow); // 添加设置主窗口指针的函数
    std::string formatClassTime(const ClassTime& time);
    std::vector<Course> courses; // 存储课程信息
    void readFromHistory();
    void getMode(int i);


    // 新增：计算已选课程总学分
    double calculateTotalCredits() const;

    // 新增：更新学分显示
    void updateCreditDisplay();

private slots:
    void on_backButton_clicked(); // 添加Back按钮的槽函数
    void on_checkButton_clicked();
    void on_ceshiButton_clicked();
    void on_filterButton_clicked();
    void onCourseSelected(int row); // 处理课程选择信号的槽函数
    void onCourseDeleted(int index); // 新增槽函数，处理课程删除信号
    void on_saveButton_clicked();
    void onResetButtonClicked();


private:
    Ui::choosingWindow *ui;
    QWidget *mainWindow; // 存储主窗口指针
    bool mode=0;
    CourseManager courseManager; // 课程管理器
    ShowList *w; // 存储ShowList指针
    Selected *selectedWidget; // 存储Selected指针


    //新增：学分上限和当前总学分
    double creditLimit = 25; // 默认上限25学分
    double credittotal=0;

};

#endif // CHOOSINGWINDOW_H
