#include "fileloader.h"
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>

FileLoader::FileLoader() {}

// 自动识别类型
FileDataType FileLoader::detectType(QString filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return FileDataType::Unknown;
    QTextStream in(&file);
    QString firstLine = in.readLine().trimmed();

    if (firstLine == "ARRAY1D") m_type = FileDataType::Array1D;
    else if (firstLine == "MATRIX") m_type = FileDataType::AdjacencyMatrix;
    else if (firstLine == "LIST") m_type = FileDataType::AdjacencyList;
    else m_type = FileDataType::Unknown;

    return m_type;
}

QVector<int> FileLoader::readArray1D(QString filePath) {
    QVector<int> array;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(nullptr, "错误", "文件打开失败！");
        return array;
    }

    QTextStream in(&file);

    // 读取第一行：ARRAY1D 长度
    QString header = in.readLine().trimmed();
    QStringList headerParts = header.split(' ', Qt::SkipEmptyParts);
    if (headerParts.size() < 2){
        QMessageBox::warning(nullptr, "错误", "文件格式错误！");
        return array;
    }
    if(headerParts[0] != "ARRAY1D"){
        QMessageBox::warning(nullptr, "错误", "该文件不是一维数组！");
        return array;
    }
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

QVector<QVector<int>> FileLoader::readAdjacencyMatrix(QString filePath) {
    QVector<QVector<int>> matrix;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(nullptr, "错误", "文件打开失败！");
        return matrix;
    }

    QTextStream in(&file);
    QString firstLine = in.readLine().trimmed();
    QStringList parts = firstLine.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 2){
        QMessageBox::warning(nullptr, "错误", "文件格式错误！");
        return matrix;
    }
    if(parts[0] != "MATRIX"){
        QMessageBox::warning(nullptr, "错误", "该文件不是邻接矩阵！");
        return matrix;
    }
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

QVector<QVector<QPair<int,int>>> FileLoader::readAdjacencyList(QString filePath) {
    QVector<QVector<QPair<int,int>>> adjList;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "错误", "文件打开失败！");
        return adjList;
    }

    QTextStream in(&file);
    QString firstLine = in.readLine().trimmed();
    QStringList parts = firstLine.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 2){
        QMessageBox::warning(nullptr, "错误", "文件格式错误！");
        return adjList;
    }
    if(parts[0] != "LIST"){
        QMessageBox::warning(nullptr, "错误", "该文件不是邻接表！");
        return adjList;
    }
    int n = (parts.size() > 1) ? parts[1].toInt() : 0;

    int r = 0;
    while (!in.atEnd() && r < n) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList tokens = line.split(' ', Qt::SkipEmptyParts);
        QVector<QPair<int,int>> neighbors;
        for (const QString &token : tokens) {
            QStringList pair = token.split(':');
            if (pair.size() == 2) {
                int v = pair[0].toInt();
                int w = pair[1].toInt();
                neighbors.append(qMakePair(v, w));
            } else {
                // 兼容旧格式（只有邻居编号，没有权重）
                int v = token.toInt();
                neighbors.append(qMakePair(v, 1)); // 默认权重=1
            }
        }
        adjList.append(neighbors);
        r++;
    }
    return adjList;
}

// 一维数组保存
void FileLoader::saveToFile(const QString &filePath, const QVector<int> &data) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "ARRAY1D " << data.size() << "\n";
    for (int v : data) out << v << " ";
    out << "\n";
}

// 二维数组保存（邻接矩阵或邻接表）
void FileLoader::saveToFile(const QString &filePath, const QVector<QVector<int>> &data) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    int rows = data.size();
    int cols = (rows > 0) ? data[0].size() : 0;
    out << "MATRIX " << rows << " " << cols << "\n";
    for (auto &row : data) {
        for (int v : row) out << v << " ";
        out << "\n";
    }
}

void FileLoader::saveToFile(QString filePath, const QVector<QVector<QPair<int,int>>> &adjList) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "n " << adjList.size() << "\n";
    for (const auto &neighbors : adjList) {
        QStringList line;
        for (auto [v, w] : neighbors) {
            line << QString("%1:%2").arg(v).arg(w);
        }
        out << line.join(" ") << "\n";
    }
}





