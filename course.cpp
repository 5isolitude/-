#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <cctype>
#include <regex>
#include "course.h"
using namespace std;

Course::Course(string id, string name, int category, int credit, string teacher,
               int index, string department, string major, int grade,
               ClassTime classtime,
               string classroom, string remark)
    : icourseID(id), strCourseName(name), iCourseCategory(category),
    iCredit(credit), strTeacher(teacher), iClassIndex(index), strDepartment(department),
    strMajor(major), iGrade(grade), Classtime(classtime),
    strClassroom(classroom), strRemark(remark), chosen(false) {
    // 生成 globalID
    int sum = accumulate(Classtime.classPeriods.begin(),
                         Classtime.classPeriods.end(), 0);
    stringstream ss;
    ss << setw(2) << setfill('0') << sum;
    globalID = id + "-" + ss.str();
}

