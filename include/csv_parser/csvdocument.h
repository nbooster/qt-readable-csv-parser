#pragma once

#include <QString>
#include <QStringList>

/*! A library for handling CSV files with the help of Qt */

namespace QtCSV
{
/*! A class that represents the logical structure and operations of a CSV document. */

class CsvDocument
{
public:
    CsvDocument();

    CsvDocument(size_t colsNum);

    CsvDocument(QList<QStringList> data);

    size_t colsNum() const;

    size_t rowsNum() const;

    bool addRow(const QStringList values);

    QString getItem(const size_t rowIndex, const size_t colIndex, bool &error) const;

    QList<QStringList> getData() const;

private:
    QList<QStringList> _data;
    size_t _colsNum = 0;
    size_t _rowsNum = 0;
};

CsvDocument fromByteArray(const QByteArray, bool *error = nullptr);

CsvDocument fromFile(const QString filePath, bool *error = nullptr);

CsvDocument fromString(const QString content, bool *error = nullptr);

bool toFile(CsvDocument csv, const QString filePath);

QString toString(CsvDocument csv);

} // namespace QtCSV
