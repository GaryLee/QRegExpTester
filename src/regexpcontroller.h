/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */
#ifndef REGEXPCONTROLLER_H
#define REGEXPCONTROLLER_H

#include <QtGlobal>
#include <QObject>
#include <QRegExp>
#include <QString>
#include <QStringList>

class RegExpController : public QObject
{
    Q_OBJECT    
public:
    explicit RegExpController(QObject *parent = 0);
    ~RegExpController();
    const QStringList &getCaretModeList();
    const QStringList &getPatternSyntaxList();
    QRegExp::CaretMode caretModeStringToId(const QString &caretMode);
    QString caretModeIdToString(QRegExp::CaretMode id);
    QRegExp::PatternSyntax patternSyntaxStringToId(const QString &patternSyntax);
    QString patternSyntaxIdToString(QRegExp::PatternSyntax id);
    QRegExp::PatternSyntax getPatternSyntax() const;
    QRegExp::CaretMode getCaretMode() const;
    void setCaretMode(QRegExp::CaretMode caretMode);
    void setCaretMode(const QString &caretMode);
    void setPatternSyntax(QRegExp::PatternSyntax pattern);
    void setPatternSyntax(const QString &pattern);
    void setPattern(const QString &pattern);
    QString getPattern();
    void setCaseSensitive(Qt::CaseSensitivity caseSensitivity);
    Qt::CaseSensitivity getCaseSensitive();
    bool isPatternValid();
    QString errorString();
    bool exactMatch(const QString &str) const;
    int	indexIn(const QString &str, int offset=0) const;
    int captureCount() const;
    QString cap(int nth=0) const;
    QStringList capturedTexts() const;
    void setMinimal(bool minimal);
    bool isMinimal() const;
    int matchedLength() const;
    bool saveToFile(const QString &filename);
    bool loadFromFile(const QString &filename);
    QString getFilename() const;
    QString getFileError() const;
    QString generatePatternForCode() const;
signals:
    
public slots:

private:
    QStringList _caretModeList;
    QStringList _patternSyntaxList;
    QRegExp *_regexp;
    QRegExp::CaretMode _caretMode;
    QString _filename;
    QString _fileError;
};

#endif // REGEXPCONTROLLER_H
