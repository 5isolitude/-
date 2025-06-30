#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#pragma once
#include "Course.h"
#include "CourseRepository.h"
#include "filter.h"
#include <vector>
#include <set>
#include <unordered_map>
#include <string>

using namespace std;
// 用户给出的筛选条件


class CourseManager
{
public:
    CourseManager() :selectedCourses(), curCredit(0), userFilter() {}
    ~CourseManager() {}

    pair<bool, string> addCourse(string ID); // 根据globalID加减课程,返回成功与否&错误信息
    pair<bool, string> removeCourse(string ID);

    // 重载get
    const vector<Course>& getAvailableCourse() const; // 获取符合筛选条件的课程vector
    void updateAvailableCourse(); // 根据当前筛选条件更新可用课程列表
    unordered_map<string, vector<Course>> getAlternativeCourses() const;//寻找同名课程
    vector<Course> getConflictingCourses() const;//寻找冲突课程


    const vector<Course>& getAllCourse() const; // 获取所有课程vector
    const vector<Course>& getSelectedCourse() const; // 获取已有课程vector
    int getCurCredit()const { return curCredit; } // 获取已选学分

    bool isTimeUnspecified(const ClassTime& time) const; // 时间是否是默认值
    bool isTimeWithinAvailable(const ClassTime& courseTime, const ClassTime& availableTime) const; // 时间是否合适

    // vector<Course> getRecomendedCourse();
    void setFilter(filter& userFilter) { this->userFilter = userFilter; } // 设置用户filter
    bool isTimeConflict(const ClassTime& time1, const ClassTime& time2) const; // 判断时间冲突
    filter userFilter; // 筛选条件
    std::vector<Course> allCourses;//存储课程信息
    void setallCourses(vector<Course>* p);

private:
    vector<Course> selectedCourses; // 已选课程
    vector<Course> availableCourses; // 筛选后的可选课程
    int curCredit = 0; // 现有的已选学分
    const int totalCredit = 4; // 学分上限


};

#endif // COURSEMANAGER_H
