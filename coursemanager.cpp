#include "CourseManager.h"
#include <cctype>  // 包含tolower函数
#include <algorithm>  // 包含transform函数

// 辅助函数：不区分大小写的字符串包含检查（支持汉字）
static bool containsIgnoreCase(const string& str, const string& substr) {
    // 如果子串为空，总是返回true
    if (substr.empty()) return true;

    // 使用标准算法查找子串（区分大小写）
    auto it = std::search(
        str.begin(), str.end(),
        substr.begin(), substr.end(),
        [](char ch1, char ch2) {
            // 只对ASCII字母进行大小写转换
            if( isalpha(static_cast<unsigned char>(ch1)) )
            {
                return tolower(ch1) == tolower(ch2);
            }
            // 对于非字母字符（包括汉字）直接比较
            return ch1 == ch2;
        }
        );
    return it != str.end();
}

const vector<Course>& CourseManager::getAvailableCourse() const {
    return availableCourses;
}
void CourseManager::setallCourses(vector<Course>* p){
    allCourses=*p;
}

bool CourseManager::isTimeUnspecified(const ClassTime& time) const {

    return time.startWeek == 0 &&
           time.endWeek == 0 &&
           time.weekdays.empty() &&
           time.classPeriods.empty();
}

// 时间是否符合
bool CourseManager::isTimeWithinAvailable(const ClassTime& courseTime, const ClassTime& availableTime) const {
    // 检查周数范围
    if(availableTime.startWeek!=0 && availableTime.endWeek != 0 )
    {
        if (courseTime.startWeek < availableTime.startWeek ||
                 courseTime.endWeek > availableTime.endWeek) {
            return false;
        }
    }


    // 检查星期几
    for (int day : courseTime.weekdays) {
        if (availableTime.weekdays.empty()) continue; // 用户未限制星期几
        if (std::find(availableTime.weekdays.begin(), availableTime.weekdays.end(), day) == availableTime.weekdays.end()) {
            return false;
        }
    }

    // 检查节次
    for (int period : courseTime.classPeriods) {
        if (availableTime.classPeriods.empty()) continue; // 用户未限制节次
        if (std::find(availableTime.classPeriods.begin(), availableTime.classPeriods.end(), period) == availableTime.classPeriods.end()) {
            return false;
        }
    }

    return true;
}
const vector<Course>& CourseManager::getAllCourse() const
{
    // TODO: 在此处插入 return 语句
    return  CourseRepository::getInstance().getCourses();
}

const vector<Course>& CourseManager::getSelectedCourse() const
{
    return this->selectedCourses;
    // TODO: 在此处插入 return 语句
}

void CourseManager::updateAvailableCourse() {
    availableCourses.clear(); // 清空旧数据
    // const vector<Course>& allCourses = CourseRepository::getInstance().getCourses();
    availableCourses.reserve(allCourses.size());

    for (const Course& course : allCourses) {

        // 按教师筛选
        if (!userFilter.strTeacher.empty()) {
            // 检查课程教师是否包含筛选文本（不区分大小写）
            if (!containsIgnoreCase(course.strTeacher, userFilter.strTeacher)) {
                continue;
            }
        }

        // 新增：按课程名称筛选（空字符串=不筛选）
        if (!userFilter.strCourseName.empty()) {
            // 检查课程名称是否包含筛选文本（不区分大小写）
            if (!containsIgnoreCase(course.strCourseName, userFilter.strCourseName)) {
                continue;
            }
        }

        // 按ID筛选（空字符串=不筛选）
        if (!userFilter.strCourseID.empty() &&
            course.icourseID != userFilter.strCourseID) {
            continue;
        }

        // 按专业筛选（空字符串=不筛选）
        if (!userFilter.strMajor.empty() &&
            course.strMajor != userFilter.strMajor) {
            continue;
        }

        // 按开课单位筛选（空字符串=不筛选）
        if (!userFilter.strDepartment.empty() &&
            course.strDepartment != userFilter.strDepartment) {
            continue;
        }


        // 检查课程类别（0表示不筛选）
        if (userFilter.iCourseCategory != 0 &&
            course.iCourseCategory != userFilter.iCourseCategory) {
            continue;
        }

        // 检查学分（0表示不筛选）
        if (userFilter.iCredit != 0 &&
            course.iCredit != userFilter.iCredit) {
            continue;
        }

        // 检查院系（空字符串表示不筛选）
        if (!userFilter.strDepartment.empty() &&
            course.strDepartment != userFilter.strDepartment) {
            continue;
        }

        // 检查专业（空字符串表示不筛选）
        if (!userFilter.strMajor.empty() &&
            course.strMajor != userFilter.strMajor) {
            continue;
        }

        // 检查年级（0表示不筛选）
        if (userFilter.iGrade != 0 &&
            course.iGrade != userFilter.iGrade) {
            continue;
        }

        // 检查教室（空字符串表示不筛选）
        if (!userFilter.strClassroom.empty() &&
            course.strClassroom != userFilter.strClassroom) {
            continue;
        }

        // 检查上课时间（全部为默认值表示不筛选）
        if (!isTimeUnspecified(userFilter.Classtime) &&
            !isTimeWithinAvailable(course.Classtime, userFilter.Classtime)) {
            continue;
        }

        availableCourses.push_back(course);
    }

}

bool CourseManager::isTimeConflict(const ClassTime& time1, const ClassTime& time2) const {
    // 检查周次范围是否有重叠
    if (time1.endWeek < time2.startWeek || time1.startWeek > time2.endWeek) {
        return false; // 周次不重叠，无冲突
    }

    // 检查星期几是否有重叠
    for (int day1 : time1.weekdays) {
        for (int day2 : time2.weekdays) {
            if (day1 == day2) { // 同一天
                // 检查节次是否有重叠
                for (int period1 : time1.classPeriods) {
                    for (int period2 : time2.classPeriods) {
                        if (period1 == period2) {
                            return true; // 发现冲突
                        }
                    }
                }
            }
        }
    }

    return false; // 无冲突
}

pair<bool, string> CourseManager::addCourse(string ID) { // 应该传入globalID

    // 1. 在可选课程中查找指定ID的课程
    auto it = find_if(availableCourses.begin(), availableCourses.end(),
                      [&ID](const Course& c) { return c.globalID == ID; });

    if (it == availableCourses.end()) {
        return make_pair(false, "未找到指定ID的课程或课程不可选");
    }

    // 2. 检查课程是否已被选择（通过比较ID）
    auto selectedIt = find_if(selectedCourses.begin(), selectedCourses.end(),
                              [&ID](const Course& c) { return c.globalID == ID; });

    if (selectedIt != selectedCourses.end()) {
        return make_pair(false, "该课程已被选择");
    }

    // 3. 检查学分是否超过限制
    if (curCredit + it->iCredit > totalCredit) {
        return make_pair(false, "选择该课程将超过学分限制");
    }

    // 4. 检查时间冲突
    for (const Course& selected : selectedCourses) {
        if (isTimeConflict(it->Classtime, selected.Classtime)) {
            return make_pair(false, "时间冲突: 与已选课程 " + selected.icourseID + " 冲突");
        }
    }

    // 5. 添加课程到已选列表
    selectedCourses.push_back(*it);
    curCredit += it->iCredit;


    return make_pair(true, "成功添加课程: " + it->strCourseName);
}
pair<bool, string> CourseManager::removeCourse(string ID) {
    // 在已选课程中查找指定ID的课程
    auto it = std::find_if(selectedCourses.begin(), selectedCourses.end(),
                           [&ID](const Course& course) { return course.globalID == ID; });

    if (it == selectedCourses.end()) {
        // 未找到课程
        return { false, "未找到指定ID的课程" };
    }

    // 减少当前学分
    curCredit -= it->iCredit;

    // 从已选课程中移除
    selectedCourses.erase(it);

    return { true, "成功移除课程" };
}

//已选课程替代：快速检视对于已选课程的同名课程时间地点
// 返回：map<课程名, vector<可替代课程>>
unordered_map<string, vector<Course>> CourseManager::getAlternativeCourses() const {
    unordered_map<string, vector<Course>> alternatives;

    for (const Course& selected : selectedCourses) {
        for (const Course& candidate : availableCourses) {
            if (candidate.strCourseName == selected.strCourseName &&
                candidate.globalID != selected.globalID) {
                alternatives[selected.strCourseName].push_back(candidate);
            }
        }
    }

    return alternatives;
}

//时间冲突提醒：在待选课程中找出和已选课程冲突的课程
vector<Course> CourseManager::getConflictingCourses() const {
    vector<Course> conflicts;

    for (const Course& candidate : availableCourses) {
        for (const Course& selected : selectedCourses) {
            if (isTimeConflict(candidate.Classtime, selected.Classtime)) {
                conflicts.push_back(candidate);
                break; // 一个冲突就够了
            }
        }
    }

    return conflicts;
}
