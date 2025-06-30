#include "CourseRepository.h"
#include <QDebug>

CourseRepository& CourseRepository::getInstance() {
    static CourseRepository instance;
    return instance;
}

bool CourseRepository::loadFromTxt(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file:" << filename;
        return false;
    }

    QTextStream in(&file);
    QStringList lines = in.readAll().split('\n');
    file.close();

    courses.clear(); // 清空旧数据

    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;

        QStringList fields = line.split(',');
        if (fields.size() < 14) {
            qWarning() << "Insufficient fields:" << fields.size() << "Content:" << line;
            continue;
        }

        // 处理时间字段
        QString timeStr = QString("%1,%2,%3").arg(fields[9].trimmed())
                              .arg(fields[10].trimmed())
                              .arg(fields[11].trimmed());

        // 类型转换
        bool ok;
        int category = fields[2].toInt(&ok);
        int credit = fields[3].toInt(&ok);
        int classIndex = fields[5].toInt(&ok);
        int grade = fields[8].toInt(&ok);

        try {
            courses.emplace_back(
                fields[0].trimmed().toStdString(),
                fields[1].trimmed().toStdString(),
                category,
                credit,
                fields[4].trimmed().toStdString(),
                classIndex,
                fields[6].trimmed().toStdString(),
                fields[7].trimmed().toStdString(),
                grade,
                timeStr.toStdString(), // 修改为传递完整的时间字符串
                fields[12].trimmed().toStdString(),
                fields[13].trimmed().toStdString()
                );
        } catch (const std::exception& e) {
            qCritical() << "Course loading exception:" << e.what() << "Line:" << line;
        }
    }

    qInfo() << "Course repository loaded successfully, total courses:" << courses.size();
    return true;
}

const vector<Course>& CourseRepository::getCourses() const {
    return courses;
}
