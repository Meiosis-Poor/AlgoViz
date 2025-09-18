//
// Created by poorc on 2025/9/17.
//

#ifndef SIMULATOR_FILELOADER_H
#define SIMULATOR_FILELOADER_H

#include <QString>
#include <QVector>

#include <QVariant>

enum class FileDataType {
    Unknown,
    Array1D,
    AdjacencyMatrix,
    AdjacencyList
};

class FileLoader {
public:
    explicit FileLoader(const QString &filePath);

    // 自动识别文件类型
    FileDataType detectType();

    // 三种读取方法（保留）
    QVector<int> readArray1D();
    QVector<QVector<int>> readAdjacencyMatrix();
    QVector<QVector<int>> readAdjacencyList();

    // 统一读取
    QVariant readGraph();

private:
    QString m_filePath;
    FileDataType m_type = FileDataType::Unknown;
};


#endif //SIMULATOR_FILELOADER_H
