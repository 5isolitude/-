#ifndef COURSEREPOSITORY_H
#define COURSEREPOSITORY_H

#pragma once
#include <vector>
#include <string>
#include "Course.h"
#include <QFile>
#include <QTextStream>

using namespace std;

class CourseRepository {
public:
    //单例接口（每个人用同一个实例）
    static CourseRepository& getInstance();

    //读取课程数据
    //bool loadFromCSV(const vector<string>& filenames);

    bool loadFromTxt(const QString& filename);


    //提供课程数据给别人用
    const vector<Course>& getCourses() const;

    CourseRepository(){}
    vector<Course> courses;
};

#endif // COURSEREPOSITORY_H
