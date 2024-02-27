#include "csvparser.h"

#include <cstring>

QtCSV::Token::Token() { type = TokenType::Empty; }

QtCSV::Token::Token(char c)
{
    content = c;
    switch (c)
    {
    case newLineCharacter:
        type = TokenType::Newline;
        break;
    case quotesCharacter:
        type = TokenType::Quotes;
        break;
    case delimiterCharacter:
        type = TokenType::Delimiter;
        break;
    case escapedCharacter:
        type = TokenType::Escaped;
        break;
    default:
        type = TokenType::Character;
    }
    if (isspace(c) && c != newLineCharacter)
        type = TokenType::White;
}

QtCSV::CsvParser::CsvParser(QByteArray bytes) : _bytes(bytes)
{
    _bytes.replace(QByteArray("\r\n"), QByteArray("\n"));
    _bytes += '\n';
}

bool QtCSV::CsvParser::getError() const { return _error; }

QtCSV::Token QtCSV::CsvParser::getNextToken(int forward = 1)
{
    return _index + forward < _bytes.size() ? Token(_bytes[(_index = _index + forward)]) : Token();
}

QtCSV::Token QtCSV::CsvParser::peekNextToken(int forward = 1) const
{
    return _index + forward < _bytes.size() ? Token(_bytes[_index + forward]) : Token();
}

QtCSV::Token QtCSV::CsvParser::peekPreviousToken(int backwards = 1) const
{
    return _index - backwards >= 0 ? Token(_bytes[_index - backwards]) : Token();
}

/*! Parses data from bytes(plain text string) into a table of values. */
QList<QStringList> QtCSV::CsvParser::getValues()
{
    while (!_error)
    {
        Token next = getNextToken();
        switch (next.type)
        {
        case TokenType::Empty:
            if (_inQuotes)
                _error = true;
            return _resultSoFar;
        case TokenType::Newline:
            if (_inQuotes)
                _error = true;
            else
            {
                if (!_valueSoFar.isEmpty())
                {
                    _rowSoFar.append(QString(_valueSoFar));
                    _valueSoFar.clear();
                }
                if (!_rowSoFar.isEmpty())
                {
                    _resultSoFar.append(QStringList(_rowSoFar));
                    _rowSoFar.clear();
                }
            }
            break;
        case TokenType::Delimiter:
            if (_inQuotes)
            {
                _valueSoFar += next.content;
            }
            else
            {
                _rowSoFar.append(QString(_valueSoFar));
                _valueSoFar.clear();
            }
            break;
        case TokenType::Escaped:
            if (peekNextToken().type != TokenType::Quotes)
                _valueSoFar += next.content;
            else
                _valueSoFar += getNextToken().content;
            break;
        case TokenType::Character:
            _valueSoFar += next.content;
            break;
        case TokenType::White:
            if (_inQuotes)
                _valueSoFar += next.content;
            break;
        case TokenType::Quotes:
            if (_inQuotes)
            {
                _inQuotes = false;
            }
            else
            {
                TokenType previousType = peekPreviousToken().type;
                if (previousType != TokenType::Empty && previousType != TokenType::Newline
                    && previousType != TokenType::Delimiter && previousType != TokenType::White)
                    _error = true;
                _inQuotes = true;
            }
            break;
        }
    }
    return _resultSoFar;
}
