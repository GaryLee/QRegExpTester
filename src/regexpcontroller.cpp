/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "regexpcontroller.h"

#define BOOL_TO_STRING(_val_) ((_val_) ? "true" : "false")
#define STRING_TO_BOOL(_val_) QRegExp("true|yes|1|on", Qt::CaseInsensitive).exactMatch(_val_)

RegExpController::RegExpController(QObject *parent) :
    QObject(parent),
    _regexp(new QRegExp()),
    _caretMode(QRegExp::CaretAtZero)
{
    _caretModeList << "CaretAtZero" << "CaretAtOffset" <<
                   "CaretWontMatch";

    _patternSyntaxList << "RegExp" << "RegExp2" <<
                      "Wildcard" << "WildcardUnix" <<
                          "FixedString" << "W3CXmlSchema11";
}

RegExpController::~RegExpController()
{
    Q_ASSERT_X(_regexp != NULL,
               "RegExpController::~RegExpController",
               "_regexp object is missing(NULL).");
    delete _regexp;
}

const QStringList &RegExpController::getCaretModeList()
{
    return _caretModeList;
}

const QStringList &RegExpController::getPatternSyntaxList()
{
    return _patternSyntaxList;
}

QRegExp::CaretMode RegExpController::caretModeStringToId(const QString &caretMode)
{
    return QRegExp::CaretMode(_caretModeList.indexOf(caretMode));
}

QString RegExpController::caretModeIdToString(QRegExp::CaretMode id)
{
    return _caretModeList.at(id);
}

QRegExp::PatternSyntax RegExpController::patternSyntaxStringToId(const QString &patternSyntax)
{
    return QRegExp::PatternSyntax(_patternSyntaxList.indexOf(patternSyntax));
}

QString RegExpController::patternSyntaxIdToString(QRegExp::PatternSyntax id)
{
    return _patternSyntaxList.at(id);
}

QRegExp::PatternSyntax RegExpController::getPatternSyntax() const
{
    return _regexp->patternSyntax();
}

QRegExp::CaretMode RegExpController::getCaretMode() const
{
    return _caretMode;
}

void RegExpController::setCaretMode(QRegExp::CaretMode caretMode)
{
    _caretMode = caretMode;
}

void RegExpController::setCaretMode(const QString &caretMode)
{
    setCaretMode(caretModeStringToId(caretMode));
}

void RegExpController::setPatternSyntax(QRegExp::PatternSyntax pattern)
{
    _regexp->setPatternSyntax(pattern);
}

void RegExpController::setPatternSyntax(const QString &pattern)
{
    setPatternSyntax(patternSyntaxStringToId(pattern));
}

void RegExpController::setPattern(const QString &pattern)
{
    _regexp->setPattern(pattern);
}

QString RegExpController::getPattern()
{
    return _regexp->pattern();
}

void RegExpController::setCaseSensitive(Qt::CaseSensitivity caseSensitivity)
{
    _regexp->setCaseSensitivity(caseSensitivity);
}

Qt::CaseSensitivity RegExpController::getCaseSensitive()
{
    return _regexp->caseSensitivity();
}

bool RegExpController::isPatternValid()
{
    return _regexp->isValid();
}

QString RegExpController::errorString()
{
    return _regexp->errorString();
}

bool RegExpController::exactMatch(const QString &str) const
{
    return _regexp->exactMatch(str);
}

int RegExpController::indexIn(const QString &str, int offset) const
{
    return _regexp->indexIn(str, offset, _caretMode);
}

int RegExpController::captureCount() const
{
    return _regexp->captureCount();
}

QString RegExpController::cap(int nth) const
{
    return _regexp->cap(nth);
}

QStringList RegExpController::capturedTexts() const
{
    return _regexp->capturedTexts();
}

void RegExpController::setMinimal(bool minimal)
{
    return _regexp->setMinimal(minimal);
}

bool RegExpController::isMinimal() const
{
    return _regexp->isMinimal();
}

int RegExpController::matchedLength() const
{
    return _regexp->matchedLength();
}

bool RegExpController::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly|QFile::Truncate|QFile::Text))
    {
        return false;
    }

    QTextStream stream(&file);

    stream << "[Pattern]" << endl;
    stream << "pattern = " << _regexp->pattern() << endl;
    stream << "patternSyntax = " << patternSyntaxIdToString(_regexp->patternSyntax()) << endl;
    stream << "caretMode = " << caretModeIdToString(_caretMode) << endl;
    stream << "caseSensitivity = " << BOOL_TO_STRING(_regexp->caseSensitivity()) << endl;
    stream << "minimal = " << BOOL_TO_STRING(_regexp->isMinimal()) << endl;

    file.close();

    _filename = filename;

    return true;
}

bool RegExpController::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }

    QRegExp keyValuePattern("\\s*(\\w+)\\s*=\\s*(.*)");
    QTextStream stream(&file);
    QString line;
    QString pattern("");
    QRegExp::PatternSyntax patternSyntax = QRegExp::RegExp;
    QRegExp::CaretMode caretMode = QRegExp::CaretAtZero;
    Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive;
    bool minimal = false;
    QString key;
    QString value;
    int lineNum = 0;
    bool parseSuccessful = false;

    _fileError.clear();

    try
    {
        while (!stream.atEnd())
        {
            lineNum ++;
            line = stream.readLine().trimmed();
            if (!keyValuePattern.exactMatch(line))
            {
                continue;
            }
            key = keyValuePattern.cap(1);
            value = keyValuePattern.cap(2).trimmed();
            // qDebug() << QString("%1 = %2").arg(key).arg(value);
            if (key.compare("pattern", Qt::CaseInsensitive) == 0)
            {
                pattern = value;
            }
            else if (key.compare("patternSyntax", Qt::CaseInsensitive) == 0)
            {
                patternSyntax = patternSyntaxStringToId(value);
            }
            else if (key.compare("caretMode", Qt::CaseInsensitive) == 0)
            {
                caretMode = caretModeStringToId(value);
            }
            else if (key.compare("caseSensitivity", Qt::CaseInsensitive) == 0)
            {
                caseSensitivity = STRING_TO_BOOL(value) ? Qt::CaseSensitive : Qt::CaseInsensitive;
            }
            else if (key.compare("minimal", Qt::CaseInsensitive) == 0)
            {
                minimal = STRING_TO_BOOL(value);
            }
        }
        parseSuccessful = true;
    }
    catch (...)
    {
        _fileError = QString("Error(line#=%1) bad syntax or unknown items").arg(lineNum);
    }

    file.close();

    if (parseSuccessful)
    {
        _regexp->setPattern(pattern);
        _regexp->setPatternSyntax(patternSyntax);
        _caretMode = caretMode;
        _regexp->setCaseSensitivity(caseSensitivity);
        _regexp->setMinimal(minimal);
        _filename = filename;
    }

    return parseSuccessful;
}

QString RegExpController::getFilename() const
{
    return _filename;
}

QString RegExpController::getFileError() const
{
    return _fileError;
}

QString RegExpController::generatePatternForCode() const
{
    return _regexp->pattern().replace("\\", "\\\\");
}
