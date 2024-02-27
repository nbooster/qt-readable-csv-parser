#pragma once

#include <QByteArray>
#include <QStringList>

namespace QtCSV
{
constexpr char newLineCharacter = '\n';
constexpr char quotesCharacter = '"';
constexpr char delimiterCharacter = ',';
constexpr char escapedCharacter = '\\';

enum TokenType
{
    Newline,
    Delimiter,
    Quotes,
    Escaped,
    White,
    Character,
    Empty
};

struct Token
{
    TokenType type;
    char content = '\0';

    Token();
    Token(char c);
};

/*! A class for parsing a CSV document. */

class CsvParser
{
public:
    CsvParser(QByteArray bytes);
    QList<QStringList> getValues();
    bool getError() const;

private:
    QByteArray _bytes;
    bool _error = false;
    bool _inQuotes = false;
    int _index = -1;
    QString _valueSoFar;
    QStringList _rowSoFar;
    QList<QStringList> _resultSoFar;

    Token getNextToken(int forward);
    Token peekNextToken(int forward) const;
    Token peekPreviousToken(int backwards) const;
};

} // namespace QtCSV
