//
// Created by poorc on 2025/9/17.
//

#ifndef SIMULATOR_FILELOADER_H
#define SIMULATOR_FILELOADER_H

#include <QString>
#include <QVector>

#include <QVariant>
#include <QFile>

enum class FileDataType {
    Unknown,
    Array1D,
    AdjacencyMatrix,
    AdjacencyList
};

class FileLoader {
public:
    explicit FileLoader();

    // 自动识别文件类型
    FileDataType detectType(QString filePath);

    // 三种读取方法（保留）
    QVector<int> readArray1D(QString filePath);
    QVector<QVector<int>> readAdjacencyMatrix(QString filePath);
    QVector<QVector<QPair<int, int>>> readAdjacencyList(QString filePath);

    void saveToFile(const QString &filePath,  const QVector<int> &data);
    void saveToFile(const QString &filePath,  const QVector<QVector<int>> &data);
    void saveToFile(QString filePath, const QVector<QVector<QPair<int, int>>> &adjList);


private:
    FileDataType m_type = FileDataType::Unknown;


};


#endif //SIMULATOR_FILELOADER_H
