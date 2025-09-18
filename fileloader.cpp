#include "fileloader.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

FileLoader::FileLoader(const QString &filePath) : m_filePath(filePath) {}

// 自动识别类型
FileDataType FileLoader::detectType() {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return FileDataType::Unknown;
    QTextStream in(&file);
    QString firstLine = in.readLine().trimmed();

    if (firstLine == "ARRAY1D") m_type = FileDataType::Array1D;
    else if (firstLine == "MATRIX") m_type = FileDataType::AdjacencyMatrix;
    else if (firstLine == "LIST") m_type = FileDataType::AdjacencyList;
    else m_type = FileDataType::Unknown;

    return m_type;
}

QVector<int> FileLoader::readArray1D() {
    QVector<int> array;
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return array;

    QTextStream in(&file);

    // 读取第一行：ARRAY1D 长度
    QString header = in.readLine().trimmed();
    QStringList headerParts = header.split(' ', Qt::SkipEmptyParts);
    if (headerParts.size() < 2) return array;

    int length = headerParts[1].toInt();

    // 读取数据
    while (!in.atEnd() && array.size() < length) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList nums = line.split(' ', Qt::SkipEmptyParts);
        for (auto &n : nums) {
            if (array.size() < length)
                array.append(n.toInt());
        }
    }

    return array;
}

QVector<QVector<int>> FileLoader::readAdjacencyMatrix() {
    QVector<QVector<int>> matrix;
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return matrix;

    QTextStream in(&file);
    QString firstLine = in.readLine().trimmed();
    QStringList parts = firstLine.split(' ', Qt::SkipEmptyParts);
    int rows = (parts.size() > 1) ? parts[1].toInt() : 0;
    int cols = (parts.size() > 2) ? parts[2].toInt() : 0;

    int r = 0;
    while (!in.atEnd() && r < rows) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList nums = line.split(' ', Qt::SkipEmptyParts);
        QVector<int> row;
        for (int c = 0; c < cols && c < nums.size(); c++) row.append(nums[c].toInt());
        matrix.append(row);
        r++;
    }
    return matrix;
}

QVector<QVector<int>> FileLoader::readAdjacencyList() {
    QVector<QVector<int>> adjList;
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return adjList;

    QTextStream in(&file);
    QString firstLine = in.readLine().trimmed();
    QStringList parts = firstLine.split(' ', Qt::SkipEmptyParts);
    int n = (parts.size() > 1) ? parts[1].toInt() : 0;

    int r = 0;
    while (!in.atEnd() && r < n) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList nums = line.split(' ', Qt::SkipEmptyParts);
        QVector<int> neighbors;
        for (auto &n : nums) neighbors.append(n.toInt());
        adjList.append(neighbors);
        r++;
    }
    return adjList;
}



QVariant FileLoader::readGraph() {
    FileDataType type = detectType();

    switch(type) {
        case FileDataType::Array1D: {
            QVector<int> arr = readArray1D();
            QVariantList list;
            for (int v : arr) list.append(v);
            return QVariant(list);
        }

        case FileDataType::AdjacencyMatrix: {
            QVector<QVector<int>> mat = readAdjacencyMatrix();
            QVariantList outer;
            for (auto &row : mat) {
                QVariantList inner;
                for (int v : row) inner.append(v);
                outer.append(inner);
            }
            return QVariant(outer);
        }

        case FileDataType::AdjacencyList: {
            QVector<QVector<int>> adj = readAdjacencyList();
            QVariantList outer;
            for (auto &neighbors : adj) {
                QVariantList inner;
                for (int v : neighbors) inner.append(v);
                outer.append(inner);
            }
            return QVariant(outer);
        }

        default:
            return QVariant();
    }
}
