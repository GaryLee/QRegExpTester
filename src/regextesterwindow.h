/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */
#ifndef REGEXTESTERWINDOW_H
#define REGEXTESTERWINDOW_H

#include <QWidget>
#include "regexpcontroller.h"

namespace Ui {
class RegexTesterWindow;
}

class RegexTesterWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit RegexTesterWindow(QWidget *parent = 0);
    ~RegexTesterWindow();
    void clearResultTable();
    void insertResultToTable(const QString &item, const QString &result);
    QString boolToString(bool value);
    void loadTesteeFromFile(const QString &filename);
    void run();
    bool saveToFile(const QString &filename);
    bool loadFromFile(const QString &filename);
    QString getFilename();
    QString getFileError();
    void syncUiToController();
    void syncUiFromController();
    bool isPatternEmpty() const;
    void clearTestee();
    void generatePatternForCode() const;

public slots:
    void doExactMatch();
    void doIndexIn();
    void clear();    

private slots:
    void on_runButton_clicked();

private:
    Ui::RegexTesterWindow *ui;
    RegExpController *_regexp;
};

#endif // REGEXTESTERWINDOW_H
