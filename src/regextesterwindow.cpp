/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "regextesterwindow.h"
#include "ui_regextesterwindow.h"

RegexTesterWindow::RegexTesterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegexTesterWindow),
    _regexp(new RegExpController(this))
{
    ui->setupUi(this);
    ui->caretModeComboBox->addItems(_regexp->getCaretModeList());
    ui->patternSyntaxComboBox->addItems(_regexp->getPatternSyntaxList());
    setWindowTitle(tr("Untitled"));
}

RegexTesterWindow::~RegexTesterWindow()
{
    delete ui;
}

void RegexTesterWindow::clearResultTable()
{
    ui->resultTable->clear();
    ui->resultTable->setRowCount(0);

    QStringList header;
    header << "Item" << "Result";
    ui->resultTable->setColumnCount(header.length());
    ui->resultTable->setHorizontalHeaderLabels(header);
}

void RegexTesterWindow::insertResultToTable(const QString &item, const QString &result)
{
    int rowIndex = ui->resultTable->rowCount();
    ui->resultTable->insertRow(rowIndex);
    ui->resultTable->setItem(rowIndex, 0, new QTableWidgetItem(item));
    ui->resultTable->setItem(rowIndex, 1, new QTableWidgetItem(result));
}

QString RegexTesterWindow::boolToString(bool value)
{
    return value ? QString("true") : QString("false");
}

void RegexTesterWindow::loadTesteeFromFile(const QString &filename)
{
    ui->testeeEdit->clear();

    QFile testeeFile(filename);
    if (testeeFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream testeeStream(&testeeFile);
        ui->testeeEdit->setText(testeeStream.readAll());
    }
}

void RegexTesterWindow::run()
{
    syncUiToController();

    ui->resultEdit->clear();
    clearResultTable();

    if (!_regexp->isPatternValid())
    {
        ui->resultEdit->setTextColor(Qt::red);
        ui->resultEdit->insertPlainText(tr("ERROR: "));
        ui->resultEdit->insertPlainText(_regexp->errorString());
        return;
    }
    else if(_regexp->getPattern().trimmed().isEmpty())
    {
        ui->resultEdit->setTextColor(Qt::red);
        ui->resultEdit->insertPlainText(tr("ERROR: pattern content is empty."));
        return;
    }
    else if(ui->testeeEdit->toPlainText().trimmed().isEmpty())
    {
        ui->resultEdit->setTextColor(Qt::red);
        ui->resultEdit->insertPlainText(tr("ERROR: testee content is empty."));
        return;
    }
    else
    {
        ui->resultEdit->setTextColor(Qt::blue);
        ui->resultEdit->insertPlainText(tr("Pattern is valid."));
    }

    ui->resultEdit->setTextColor(Qt::black);
    if (ui->exactMatchRadioBox->isChecked())
    {
        doExactMatch();
    }
    else
    {
        doIndexIn();
    }
    ui->resultTable->resizeColumnsToContents();
}

bool RegexTesterWindow::saveToFile(const QString &filename)
{
    syncUiToController();
    if (_regexp->saveToFile(filename))
    {
        setWindowTitle(filename);
        return true;
    }
    return false;
}

bool RegexTesterWindow::loadFromFile(const QString &filename)
{
    if (_regexp->loadFromFile(filename))
    {
        syncUiFromController();
        setWindowTitle(filename);
        return true;
    }
    return false;
}

QString RegexTesterWindow::getFilename()
{
    return _regexp->getFilename();
}

QString RegexTesterWindow::getFileError()
{
    return _regexp->getFileError();
}

void RegexTesterWindow::syncUiToController()
{
    _regexp->setPattern(ui->patternEdit->toPlainText());
    _regexp->setPatternSyntax(ui->patternSyntaxComboBox->currentText());
    _regexp->setCaseSensitive(ui->caseSensitiveCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
    _regexp->setCaretMode(ui->caretModeComboBox->currentText());
    _regexp->setMinimal(ui->minimalCheckBox->isChecked());
}

void RegexTesterWindow::syncUiFromController()
{
    ui->patternEdit->setPlainText(_regexp->getPattern());
    ui->patternSyntaxComboBox->setCurrentIndex(int(_regexp->getPatternSyntax()));
    ui->caseSensitiveCheckBox->setChecked(_regexp->getCaseSensitive() == Qt::CaseSensitive);
    ui->caretModeComboBox->setCurrentIndex(int(_regexp->getCaretMode()));
    ui->minimalCheckBox->setChecked(_regexp->isMinimal());
}

bool RegexTesterWindow::isPatternEmpty() const
{
    return ui->patternEdit->toPlainText().trimmed().isEmpty();
}

void RegexTesterWindow::clearTestee()
{
    ui->testeeEdit->clear();
}

void RegexTesterWindow::generatePatternForCode() const
{
    ui->resultEdit->clear();
    ui->resultEdit->setTextColor(Qt::blue);
    ui->resultEdit->setText(QString("QRegExp(\"%1\")").arg(_regexp->generatePatternForCode()));
}

void RegexTesterWindow::doExactMatch()
{
    bool matchResult = _regexp->exactMatch(ui->testeeEdit->toPlainText());

    insertResultToTable(tr("exactMatch(...)"), boolToString(matchResult));
    insertResultToTable(tr("matchedLength"), QString::number(_regexp->matchedLength()));
    insertResultToTable(tr("captureCount"), QString::number(_regexp->captureCount()));
    for (int i = 0; i <= _regexp->captureCount(); ++i)
    {
        QString itemText = QString(tr("cap(%1)")).arg(i);
        insertResultToTable(itemText, _regexp->cap(i));
    }
}

void RegexTesterWindow::doIndexIn()
{
    QString itemTitle;
    QString testeeString = ui->testeeEdit->toPlainText();
    int pos = 0;
    int len;

    while ((pos = _regexp->indexIn(testeeString, pos)) != -1)
    {
        len = _regexp->matchedLength();
        itemTitle = QString(tr("Testee[%1, %2]")).arg(pos).arg(pos + len - 1);
        insertResultToTable(itemTitle, testeeString.mid(pos, len));
        pos += len;
    }
}

void RegexTesterWindow::clear()
{
    ui->patternSyntaxComboBox->setCurrentIndex(0);
    ui->caretModeComboBox->setCurrentIndex(0);
    ui->caseSensitiveCheckBox->setChecked(true);
    ui->minimalCheckBox->setChecked(false);
    ui->exactMatchRadioBox->setChecked(true);
    ui->patternEdit->clear();
    ui->resultEdit->clear();
    ui->testeeEdit->clear();
    clearResultTable();
}

void RegexTesterWindow::on_runButton_clicked()
{
    run();
}
