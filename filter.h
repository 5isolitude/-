#ifndef FILTER_H
#define FILTER_H

#pragma once
#include <string>
#include "Course.h"

using namespace std;

class filter { // 和course部分保持一致
public:
    int iCourseCategory; // 课程类别
    int iCredit; //学分
    string strDepartment;//开课单位
    string strMajor;//专业
    int iGrade;// 年级
    ClassTime Classtime;//上课时间
    string strClassroom;// 教室

    string strTeacher;
    string strCourseID;
    string strCourseName;
    filter();
    void setFilter();
};

#endif // FLITER_H
