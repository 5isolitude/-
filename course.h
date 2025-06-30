// course.h
#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <numeric>
#include <iomanip>

using namespace std;

struct ClassTime {
    int startWeek; // 第一周
    int endWeek;   // 最后一周
    vector<int> weekdays; // 1=周一, ..., 7=周日
    vector<int> classPeriods; // 1=第一节, ..., 多少节
    ClassTime() : startWeek(0), endWeek(0) {}
};

// 解析上课时间字符串
inline ClassTime parseClassTime(const string& raw) {
    ClassTime result;
    smatch match;
    // 正则表达式匹配时间格式
    regex pattern(R"((\d+)~(\d+),([\d ]+),(\d+)~(\d+))");
    if (regex_search(raw, match, pattern)) {
        result.startWeek = stoi(match[1]);
        result.endWeek = stoi(match[2]);

        string days = match[3];
        stringstream daysStream(days);
        int day;
        while (daysStream >> day) {
            result.weekdays.push_back(day);
        }

        result.classPeriods.push_back(stoi(match[4]));
        result.classPeriods.push_back(stoi(match[5]));
    }
    return result;
}

class Course {
public:
    string icourseID;     // 课程号
    string strCourseName; // 课程名称
    int iCourseCategory;  // 课程类别
    int iCredit;          // 学分
    string strTeacher;    // 授课教师
    int iClassIndex;      // 班号
    string strDepartment; // 开课单位
    string strMajor;      // 专业
    int iGrade;           // 年级
    ClassTime Classtime;  // 上课时间
    string strClassroom;  // 教室
    string strRemark;     // 备注
    bool chosen;          // 是否选择

    // 新加的！
    string globalID; // 唯一标识

    // 构造函数
    Course(string id, string name, int category, int credit, string teacher,
           int index, string department, string major, int grade, const string& timestr,
           string classroom, string remark)
        : icourseID(id), strCourseName(name), iCourseCategory(category),
        iCredit(credit), strTeacher(teacher), iClassIndex(index), strDepartment(department),
        strMajor(major), iGrade(grade), Classtime(parseClassTime(timestr)),
        strClassroom(classroom), strRemark(remark), chosen(false) {
        int sum = accumulate(Classtime.classPeriods.begin(),
                             Classtime.classPeriods.end(), 0);
        stringstream ss;
        ss << setw(2) << setfill('0') << sum;
        globalID = id + "-" + ss.str();
    }
    Course(string id, string name, int category, int credit, string teacher,
                   int index, string department, string major, int grade,
                   ClassTime classtime,
                   string classroom, string remark);
};

#endif
