#include "csv_parser/csvdocument.h"
#include "csvparser.h"

#include <QFile>

#include <string>

using namespace QtCSV;

static QByteArray readDataFromFile(const QString filePath, bool &error)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        error = true;
        return QByteArray();
    }
    QByteArray data = file.readAll();
    file.close();
    error = false;
    return data;
}

static bool writeDataToFile(const QByteArray bytes, const QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    file.write(bytes);
    file.close();
    return true;
}

QtCSV::CsvDocument::CsvDocument() : _colsNum(0), _rowsNum(0) {}

/*! Constructs a CSVDoc class with one row and @colsNum[in] columns with name "Col#" */
QtCSV::CsvDocument::CsvDocument(size_t colsNum) : _colsNum(colsNum), _rowsNum(1)
{
    QStringList row;
    for (int index = 0; index < _colsNum; ++index)
        row.append(QString((std::string("Col") + std::to_string(index)).c_str()));
    _data = { row };
    _rowsNum = 1;
    if (colsNum == 0)
        _rowsNum = 0;
}

/*! Constructs a CSVDoc class from a a table(List of StringLists) of string values (@data[in]). */
QtCSV::CsvDocument::CsvDocument(QList<QStringList> data) : _data(data), _rowsNum(data.size())
{
    if (data.size())
        _colsNum = data[0].size();
    else
        _colsNum = 0;
}

size_t QtCSV::CsvDocument::colsNum() const { return _colsNum; };

size_t QtCSV::CsvDocument::rowsNum() const { return _rowsNum; };

/*! Adds a row of string values (@values[in]) in the CSVDoc. */
bool QtCSV::CsvDocument::addRow(const QStringList values)
{
    if (values.size() != _colsNum)
        return false;
    _data.append(values);
    ++_rowsNum;
    return true;
}

/*! Returns the value at @rowIndex row and @colIndex column, as a string. */
QString QtCSV::CsvDocument::getItem(const size_t rowIndex, const size_t colIndex, bool &error) const
{
    if (rowIndex >= _rowsNum || colIndex >= _colsNum)
    {
        error = true;
        return QString();
    }

    error = false;
    return _data[rowIndex][colIndex];
}

QList<QStringList> QtCSV::CsvDocument::getData() const { return _data; }

QtCSV::CsvDocument QtCSV::fromByteArray(const QByteArray bytes, bool *error)
{
    CsvParser parser(bytes);
    CsvDocument csv(parser.getValues());
    if (parser.getError())
    {
        *error = true;
        return CsvDocument(0);
    }
    *error = false;
    return csv;
}

QtCSV::CsvDocument QtCSV::fromFile(const QString filePath, bool *error)
{
    QByteArray bytes = readDataFromFile(filePath, *error);
    if (*error)
        return CsvDocument(0);
    return fromByteArray(bytes, error);
}

/*! Creates and returns a CSVDoc object from a string value(all csv contents in one string). */
QtCSV::CsvDocument QtCSV::fromString(const QString content, bool *error)
{
    QByteArray bytes(content.toStdString().c_str());
    if (*error)
        return CsvDocument(0);
    return fromByteArray(bytes, error);
}

/*! Serializes (in plain text) and returns all CSVDoc contents in one string. */
QString QtCSV::toString(QtCSV::CsvDocument csv)
{
    if (csv.colsNum() == 0 || csv.rowsNum() == 0)
        return QString();
    bool error;
    QString result;
    for (int rindex = 0; rindex < csv.rowsNum(); ++rindex)
    {
        for (int cindex = 0; cindex < csv.colsNum() - 1; ++cindex)
        {
            result += QString(quotesCharacter + csv.getItem(rindex, cindex, error) + quotesCharacter
                              + delimiterCharacter);
        }
        result += QString(quotesCharacter + csv.getItem(rindex, csv.colsNum() - 1, error)
                          + quotesCharacter + newLineCharacter);
    }
    result.chop(1);
    return result;
}

/*! Writes the contents of CSVDoc in a specific file as a string (plain text) */
bool QtCSV::toFile(QtCSV::CsvDocument csv, const QString filePath)
{
    QString qstr = toString(csv);
    std::string str = qstr.toStdString();
    const char *cstr = str.c_str();
    QByteArray data(cstr, str.size());
    return writeDataToFile(data, filePath);
}
