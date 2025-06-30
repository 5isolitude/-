#include "choosingwindow.h"
#include "ui_choosingwindow.h"
#include "schedule.h"
#include "Course.h"
#include <QApplication>
#include <QScrollArea>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QFileDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QPushButton>
#include <QPalette>
#include <QBrush>
#include <QPixmap>
#include <iostream>
#include<QMessageBox>
#include "showlist.h"
#include"coursemanager.h"
#include"filter.h"
#include"helpwindow.h"


// 格式化上课时间输出
std::string choosingWindow::formatClassTime(const ClassTime& time) {
    std::stringstream ss;
    ss << time.startWeek << "~" << time.endWeek << "周 ";

    // 星期几
    for (size_t i = 0; i < time.weekdays.size(); ++i) {
        if (i > 0) ss << " ";
        ss << "周" << time.weekdays[i];
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

// 计算已选课程总学分
double choosingWindow::calculateTotalCredits() const {
    double total = 0.0;
    const std::vector<Course>& selectedCourses = selectedWidget->getSelectedCourses();

    for (const auto& course : selectedCourses) {
        if (course.chosen) {  // 确保只计算已选课程
            total += course.iCredit;
        }
    }
    return total;
}

/*
// 更新学分显示
void choosingWindow::updateCreditDisplay() {
    credittotal = calculateTotalCredits();
    // 根据学分状态设置标签颜色
    if (credittotal > creditLimit) {
        // 超限时文字变红
        ui->totalCreditsLabel->setStyleSheet("color: red;");
    } else {
        // 未超限时恢复默认颜色
        ui->totalCreditsLabel->setStyleSheet("");
    }

    // 更新总学分显示
    credittotal=double(credittotal);
    ui->totalCreditsLabel->setText(QString("当前总学分: %1").arg(credittotal, 0, 'f', 1));

}
*/

// 更新学分显示
void choosingWindow::updateCreditDisplay() {
    credittotal = calculateTotalCredits();
    // 更新总学分显示
    credittotal = double(credittotal);
    ui->totalCreditsLabel->setText(QString("当前总学分: %1").arg(credittotal, 0, 'f', 1));

    // 使用 QPalette 设置字体颜色
    QPalette palette = ui->totalCreditsLabel->palette();

    if (credittotal > creditLimit) {
        // 超限时文字变红
        palette.setColor(QPalette::WindowText, Qt::red);
    } else {
        // 恢复默认颜色
        palette.setColor(QPalette::WindowText, Qt::black);
    }

    // 应用更新后的调色板
    ui->totalCreditsLabel->setPalette(palette);

    // 强制刷新UI
    ui->totalCreditsLabel->repaint();
}

void choosingWindow::onResetButtonClicked() {
    // 记录当前已选课程
    const std::vector<Course>& selectedCourses = selectedWidget->getSelectedCourses();

    // 清空所有输入框
    ui->teacherNameLineEdit->clear();
    ui->courseNameLineEdit->clear();
    ui->courseIDLineEdit->clear();
    ui->startPeriodLineEdit->clear();
    ui->endPeriodLineEdit->clear();
    ui->creditLineEdit->clear();
    ui->endWeekLineEdit->clear();
    ui->startWeekLineEdit->clear();

    // 重置周一到周天的勾选框
    ui->mondayCheckBox->setChecked(false);
    ui->tuesdayCheckBox->setChecked(false);
    ui->wednesdayCheckBox->setChecked(false);
    ui->thursdayCheckBox->setChecked(false);
    ui->fridayCheckBox->setChecked(false);
    ui->saturdayCheckBox->setChecked(false);
    ui->sundayCheckBox->setChecked(false);

    // 重置两个下拉列表
    ui->categoryComboBox->setCurrentIndex(0); // 假设第一个选项是默认选项
    ui->departmentComboBox->setCurrentIndex(0); // 假设第一个选项是默认选项

    // 创建一个默认的筛选器对象
    filter defaultFilter;
    // 将默认筛选器对象设置给课程管理器
    courseManager.setFilter(defaultFilter);

    // 更新可用课程列表
    courseManager.updateAvailableCourse();
    const std::vector<Course>& availableCourses = courseManager.getAvailableCourse();

    // 更新课程显示
    w->setCourses(availableCourses);

    // 重新标记冲突课程
    w->highlightConflictingCourses(selectedCourses);

    // 更新学分显示
    updateCreditDisplay();
}


choosingWindow::choosingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::choosingWindow)
    , mainWindow(nullptr)
    ,creditLimit(25)
    ,credittotal(0)
{
    ui->setupUi(this);
    this->resize(3200, 1600);
    this->setWindowTitle("选课窗口");

    //设置背景图片
    QPalette palette;
    //palette.setColor(QPalette::Window, Qt::white);
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/back/sky.jpeg")));
    this->setPalette(palette);
    this->setAutoFillBackground(true);



    // 创建用于显示图片的QLabel
    QLabel *imageLabel = new QLabel(this);
    imageLabel->setGeometry(50, 10, 850,125 ); // 设置位置和大小
    imageLabel->setStyleSheet("border: none;"); // 设置无框样式

    // 加载并显示图片
    QPixmap pixmap(":/back/pkuchoose.jpg"); // 替换为实际的图片路径
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));


    // 连接信号槽
    connect(ui->backButton, &QPushButton::clicked, this, &choosingWindow::on_backButton_clicked);
    connect(ui->helpButton, &QPushButton::clicked, this, [=](){
        helpWindow *h=new helpWindow(nullptr);
        h->show();
    });
    connect(ui->saveButton, &QPushButton::clicked, this, &choosingWindow::on_saveButton_clicked);
    connect(ui->checkButton, &QPushButton::clicked, this, [=](){
        schedule *x = new schedule(nullptr);
        x->setSelectedCourses(courses); // 传递已选课程
        x->showCourseSchedule();
        x->show();
    });
    connect(ui->filterButton, &QPushButton::clicked, this, &choosingWindow::on_filterButton_clicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &choosingWindow::onResetButtonClicked);

    // 读取课程数据
    QFile file(":/txt/course.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Failed to open course file");
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QStringList lines = content.split('\n');
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) {
            continue; // 跳过空行
        }

        QStringList fields = line.split(',');
        if (fields.size() != 14) {
            QMessageBox::warning(this, "Warning", QString("Invalid line: %1").arg(line));
            continue;
        }

        // 将字符串转换为相应的数据类型
        int credit = fields[3].toInt();
        int classIndex = fields[5].toInt();
        int grade = fields[8].toInt();
        int category = fields[2].toInt();

        // 处理时间字段
        QString timeStr = QString("%1,%2,%3").arg(fields[9].trimmed())
                              .arg(fields[10].trimmed())
                              .arg(fields[11].trimmed());

        try {
            // 创建 Course 对象并添加到 courses 容器中
            Course c(fields[0].trimmed().toStdString(),          // 课程号
                     fields[1].trimmed().toStdString(),          // 课程名称
                     category,                         // 课程类别
                     credit,                           // 学分
                     fields[4].trimmed().toStdString(),          // 授课教师
                     classIndex,                         // 班号
                     fields[6].trimmed().toStdString(),          // 开课单位
                     fields[7].trimmed().toStdString(),          // 专业
                     grade,                            // 年级
                     timeStr.toStdString(),          // 时间字符串
                     fields[12].trimmed().toStdString(),         // 教室
                     fields[13].trimmed().toStdString()          // 备注
                     );
            courses.push_back(c);
        } catch (const std::exception& e) {
            qCritical() << "Course loading exception:" << e.what() << "Line:" << line;
        }
    }

    // 打印读取的课程信息（仅用于调试）
    for (const Course& course : courses) {
        std::cout << "Course ID: " << course.icourseID
                  <<" , globalID"<<course.globalID
                  << ", Course Name: " << course.strCourseName
                  << ", Credit: " << course.iCredit
                  << ", Course Time: " << formatClassTime(course.Classtime)
                  << ", teacher:"<<course.strTeacher
                  <<" ,CourseCategory:"<<course.iCourseCategory
                  <<" ,Department:"<<course.strDepartment
                  <<" ,Major:"<<course.strMajor
                  <<" ,remark:"<<course.strRemark
                  <<std::endl;
        std::cout<<"starttime:"<<course.Classtime.startWeek<<std::endl;
        std::cout<<"endtime:"<<course.Classtime.endWeek<<std::endl;
        std::cout << std::endl;
    }
    courseManager.setallCourses(&courses);

    // 展示课程，打开一个showlist
    w = new ShowList(this);
    w->setFixedSize(1325, 500); // 设置滚动区域的固定大小
    w->setStyleSheet("QScrollArea { border: 1px solid #ccc; }");
    w->setGeometry(200, 400, 1325, 500); // 设置滚动区域的位置和大小
    w->setCourses(courses);
    w->show();

    // 连接ShowList的课程选择信号到槽函数
    connect(w, &ShowList::courseSelected, this, &choosingWindow::onCourseSelected);

    // 创建并显示Selected
    selectedWidget = new Selected(this);
    selectedWidget->setFixedSize(500, 200); // 设置滚动区域的固定大小
    selectedWidget->setStyleSheet("QScrollArea { border: 1px solid #ccc; }");
    selectedWidget->setGeometry(1000, 50, 500, 200); // 设置滚动区域的位置和大小
    selectedWidget->setSelectedCourses(courses);
    selectedWidget->show();

    // 连接Selected的课程删除信号到槽函数
    connect(selectedWidget, &Selected::courseDeleted, this, &choosingWindow::onCourseDeleted);

    //输入框提示信息
    ui->teacherNameLineEdit->setPlaceholderText("授课老师");
    ui->courseNameLineEdit->setPlaceholderText("课程名称");
    ui->courseIDLineEdit->setPlaceholderText("课程 ID");
    ui->startPeriodLineEdit->setPlaceholderText("第几节开始");
    ui->endPeriodLineEdit->setPlaceholderText("第几节结束");

    ui->creditLineEdit->setPlaceholderText("学分");
    ui->endWeekLineEdit->setPlaceholderText("结束周");
    ui->startWeekLineEdit->setPlaceholderText("开始周");

    vector<string> departments;
    departments.insert(departments.end(), {"英语语言文学系", "体育教研部", "地球与空间科学学院","城市与环境学院","工学院","材料科学与工程学院",
                                           "社会学系","生命科学学院","建筑与景观设计学院","国家发展研究院","现代农学院","教育学院","现代农学院"
                                           ,"教务部","信息管理系","中国社会科学调查中心","信息科学技术学院"});
    ui->departmentComboBox->setPlaceholderText("开课单位");
    ui->departmentComboBox->addItem("全部");
    for (const auto& department : departments) {
        ui->departmentComboBox->addItem(QString::fromStdString(department));
    }


    vector<string> categorys;

    categorys.insert(categorys.end(),{"通识课1","通识课2","通识课3","通识课4",
                                       "通识核心课1","通识核心课2","通识核心课3","通识核心课4",
                                       "全校任选","劳动教育课","英语课A","英语课B","英语课C","英语课C+",
                                       "全校必修","专业必修","限选","实习"});// 按顺序分别对应iCourseCatogory 1 2 3 4 ... 18

    ui->categoryComboBox->setPlaceholderText("课程类别");
    ui->categoryComboBox->addItem("全部");
    for (const auto& department : categorys) {
        ui->categoryComboBox->addItem(QString::fromStdString(department));
    }


    //初始化学分显示
    ui->totalCreditsLabel->setText("当前总学分: 0");
    ui->creditLimitLineEdit->setText("设定学分上限：25");

    // 连接学分相关信号槽
    connect(ui->creditLimitLineEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        QString inputText = ui->creditLimitLineEdit->text();
        QRegularExpression regex("设定学分上限：([0-9]+(?:\\.[0-9]+)?)");
        QRegularExpressionMatch match = regex.match(inputText);

        // 如果匹配成功，提取数字并转换
        if (match.hasMatch()) {
            QString numberStr = match.captured(1);  // 获取第一个捕获组，即数字部分
            double newLimit = numberStr.toDouble(); // 转换为 double 类型

            // 如果转换结果是有效数字，更新学分上限
            if (newLimit > 0) {
                creditLimit = newLimit;
                //QMessageBox::information(this, "设置成功", QString("学分上限已设置为: %1").arg(creditLimit, 0, 'f', 1));
                updateCreditDisplay();  // 更新学分显示
            } else if(newLimit<0){
                QMessageBox::warning(this, "输入错误", "请输入有效的学分上限值");
            }
        }
        // 可处理不合法输入，比如保留原上限或提示

        qDebug()<<creditLimit;
    });

    // 连接课程选择和删除信号到学分更新函数
    connect(w, &ShowList::courseSelected, this, [=](){
        updateCreditDisplay();
    });
    connect(selectedWidget, &Selected::courseDeleted, this, [=](){
        updateCreditDisplay();
    });

}

choosingWindow::~choosingWindow()
{
    delete ui;
}

void choosingWindow::setMainWindow(QWidget *window) {
    mainWindow = window;
}

void choosingWindow::on_backButton_clicked() {
    mainWindow->show();
    this->deleteLater(); // 销毁自身
}

// 已经弃用了
void choosingWindow::on_checkButton_clicked(){
    // schedule *p=new schedule(nullptr);
    // p->show();
}

// 分割符号
void choosingWindow::on_ceshiButton_clicked(){
    // ShowList *w=new ShowList(this);
    // w->setFixedSize(1400, 500); // 设置滚动区域的固定大小
    // w->setStyleSheet("QScrollArea { border: 1px solid #ccc; }");
    // w->setGeometry(100, 300, 1400, 500); // 设置滚动区域的位置和大小
    // w->setCourses(courses);
    // w->show();
}

void choosingWindow::on_filterButton_clicked() {
    filter userFilter;
    std::cout<<"filter on clicked"<<std::endl;
    // 时间筛选
    int startWeek = ui->startWeekLineEdit->text().toInt();
    int endWeek = ui->endWeekLineEdit->text().toInt();
    userFilter.Classtime.startWeek = startWeek;
    userFilter.Classtime.endWeek = endWeek;

    std::vector<int> weekdays;
    if (ui->mondayCheckBox->isChecked()) weekdays.push_back(1);
    if (ui->tuesdayCheckBox->isChecked()) weekdays.push_back(2);
    if (ui->wednesdayCheckBox->isChecked()) weekdays.push_back(3);
    if (ui->thursdayCheckBox->isChecked()) weekdays.push_back(4);
    if (ui->fridayCheckBox->isChecked()) weekdays.push_back(5);
    if (ui->saturdayCheckBox->isChecked()) weekdays.push_back(6);
    if (ui->sundayCheckBox->isChecked()) weekdays.push_back(7);
    userFilter.Classtime.weekdays = weekdays;

    int startPeriod = ui->startPeriodLineEdit->text().toInt();
    int endPeriod = ui->endPeriodLineEdit->text().toInt();
    if(startPeriod==0&&endPeriod==0){

    }
    else{
        for (int i = startPeriod; i <= endPeriod; ++i) {
            userFilter.Classtime.classPeriods.push_back(i);
        }
    }

    // 学分筛选
    int credit = ui->creditLineEdit->text().toInt();
    userFilter.iCredit = credit;

    // 新增：老师姓名筛选
    QString teacherName = ui->teacherNameLineEdit->text();
    userFilter.strTeacher = teacherName.toStdString();

    // 新增：课程名称筛选
    QString courseName = ui->courseNameLineEdit->text();
    userFilter.strCourseName = courseName.toStdString();

    // 新增：课程ID筛选
    QString courseID = ui->courseIDLineEdit->text();
    userFilter.strCourseID = courseID.toStdString();

    // 获取开课单位筛选条件
    QString departmentText = ui->departmentComboBox->currentText();
    if (departmentText != "全部") {
        userFilter.strDepartment = departmentText.toStdString();
    }

    // 课程类别筛选
    QString categoryText = ui->categoryComboBox->currentText();
    if (categoryText != "全部") {
        // 将中文类别映射到对应的数字ID
        static const QMap<QString, int> categoryMap = {
            {"通识课1", 1}, {"通识课2", 2}, {"通识课3", 3}, {"通识课4", 4},
            {"通识核心课1", 5}, {"通识核心课2", 6}, {"通识核心课3", 7}, {"通识核心课4", 8},
            {"全校任选", 9}, {"劳动教育课", 10},
            {"英语课A", 11}, {"英语课B", 12}, {"英语课C", 13}, {"英语课C+", 14},
            {"全校必修", 15}, {"专业必修", 16}, {"限选", 17}, {"实习", 18}
        };

        if (categoryMap.contains(categoryText)) {
            userFilter.iCourseCategory = categoryMap.value(categoryText);
        }
    }


    courseManager.setFilter(userFilter);
    courseManager.updateAvailableCourse();
    w->setCourses(courseManager.getAvailableCourse());
}

void choosingWindow::onCourseSelected(int row)
{
    const Course& selectedCourse = w->courses[row];
    for (Course& course : courses) {
        if (course.globalID == selectedCourse.globalID) {
            course.chosen = true;
            break;
        }
    }
    selectedWidget->setSelectedCourses(courses);
    w->highlightConflictingCourses(selectedWidget->selectedCourses);

    // 更新学分显示
    updateCreditDisplay();
}

void choosingWindow::onCourseDeleted(int index)
{
    if (index < 0 || index >= static_cast<int>(selectedWidget->selectedCourses.size())) {
        return;
    }

    const Course& deletedCourse = selectedWidget->selectedCourses[index];
    for (Course& course : courses) {
        if (course.globalID == deletedCourse.globalID) {
            course.chosen = false;
            break;
        }
    }
    selectedWidget->setSelectedCourses(courses);
    w->highlightConflictingCourses(selectedWidget->selectedCourses);

    // 更新学分显示
    updateCreditDisplay();
}

void choosingWindow::getMode(int i){
    mode=i;
}


// 存盘功能
void choosingWindow::on_saveButton_clicked() {
    // 获取保存文件的路径
    QString filePath = QFileDialog::getSaveFileName(this, "保存课程选择", "", "文本文件 (*.txt)");
    if (filePath.isEmpty()) {
        return; // 用户取消保存
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "无法打开文件进行保存");
        return;
    }

    QTextStream out(&file);
    // 遍历所有课程，找出被选中的课程
    for (size_t i = 0; i < courses.size(); ++i) {
        if (courses[i].chosen) {
            out << i << "\n"; // 保存课程在courses向量中的索引
        }
    }

    file.close();
    QMessageBox::information(this, "提示", "课程选择已保存");
}

// 读盘功能
void choosingWindow::readFromHistory() {
    // 获取打开文件的路径
    QString filePath = QFileDialog::getOpenFileName(this, "选择课程选择文件", "", "文本文件 (*.txt)");
    if (filePath.isEmpty()) {
        return; // 用户取消打开
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "无法打开文件进行读取");
        return;
    }

    QTextStream in(&file);
    // 清空之前的选择
    for (auto& course : courses) {
        course.chosen = false;
    }

    // 读取文件中的索引并选中相应的课程
    while (!in.atEnd()) {
        QString line = in.readLine();
        bool ok;
        int index = line.toInt(&ok);
        if (ok && index >= 0 && static_cast<size_t>(index) < courses.size()) {
            courses[index].chosen = true;
        }
    }

    file.close();

    // 更新显示
    w->setCourses(courses);
    selectedWidget->setSelectedCourses(courses);
    w->highlightConflictingCourses(selectedWidget->selectedCourses);
    QMessageBox::information(this, "提示", "课程选择已加载");
}
