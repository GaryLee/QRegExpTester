/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regextesterwindow.h"

#define APP_TITLE "QRegExpTester"
#define APP_VERSION "0.9a"
#define APP_DATE "2013/05/21"
#define APP_AUTHOR "\"Gary Lee\" <garywlee@gmail.com>"

const QString MainWindow::aboutInfo(APP_TITLE " " APP_VERSION " (" APP_DATE ")\n\nAuthor: " APP_AUTHOR);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionNewTester->trigger();
}

MainWindow::~MainWindow()
{
    delete ui;
}

RegexTesterWindow *MainWindow::getActiveTesterWindow()
{
    QMdiSubWindow *subWindow = ui->mdiArea->activeSubWindow();
    if (subWindow == NULL)
    {
        return NULL;
    }

    return qobject_cast<RegexTesterWindow *>(subWindow->widget());
}

void MainWindow::on_actionNewTester_triggered()
{
    RegexTesterWindow *subWindow = new RegexTesterWindow(this);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(subWindow);    
    if (ui->mdiArea->subWindowList().count() == 1)
    {
        subWindow->showMaximized();
    }
    else
    {
        subWindow->show();
    }
    subWindow->activateWindow();
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionClear_triggered()
{
    QMdiSubWindow *subWindow = ui->mdiArea->activeSubWindow();
    if (subWindow == NULL)
    {
        return;
    }

    RegexTesterWindow *regexpTesterWindow = getActiveTesterWindow();
    if (regexpTesterWindow == NULL)
    {
        return;
    }
    regexpTesterWindow->clear();
}

void MainWindow::on_actionRun_triggered()
{
    RegexTesterWindow *regexpTesterWindow = getActiveTesterWindow();
    if (regexpTesterWindow == NULL)
    {
        return;
    }
    regexpTesterWindow->run();
}

void MainWindow::on_action_Import_Testee_triggered()
{
    RegexTesterWindow *regexpTesterWindow = getActiveTesterWindow();
    if (regexpTesterWindow == NULL)
    {
        return;
    }

    QString testeeFilename = QFileDialog::getOpenFileName();
    if (testeeFilename.isEmpty())
    {
        return;
    }
    regexpTesterWindow->loadTesteeFromFile(testeeFilename);
}

void MainWindow::on_actionSave_triggered()
{
    RegexTesterWindow *regexpTesterWindow = getActiveTesterWindow();
    if (regexpTesterWindow == NULL)
    {
        return;
    }

    if (regexpTesterWindow->isPatternEmpty())
    {
        QMessageBox::warning(this, "Empty Pattern", "Pattern can't be empty.");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save to file"), regexpTesterWindow->getFilename(), tr("Pattern File (*.qregexp);;All files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }

    if (!regexpTesterWindow->saveToFile(filename))
    {
        statusBar()->showMessage(tr("Save file failed."));
    }
    else
    {
        statusBar()->showMessage(tr("Save file successfully."));
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load from file"), QString(), tr("Pattern File (*.qregexp);;All files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    ui->actionNewTester->trigger();
    RegexTesterWindow *regexpTesterWindow = getActiveTesterWindow();
    if (regexpTesterWindow == NULL)
    {
        return;
    }

    if (!regexpTesterWindow->loadFromFile(filename))
    {
        statusBar()->showMessage(QString("Pattern loadded fail! ") + regexpTesterWindow->getFileError());
    }
    else
    {
        statusBar()->showMessage("Pattern loadded successfully!");
    }
}

void MainWindow::on_actionClearTestee_triggered()
{
    RegexTesterWindow *regexpTesterWindow = getActiveTesterWindow();
    if (regexpTesterWindow == NULL)
    {
        return;
    }
    regexpTesterWindow->clearTestee();
}

void MainWindow::on_actionGeneratePatternForCoding_triggered()
{
    RegexTesterWindow *regexpTesterWindow = getActiveTesterWindow();
    if (regexpTesterWindow == NULL)
    {
        return;
    }
    regexpTesterWindow->generatePatternForCode();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", aboutInfo);
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::warning(this, "Help", "Not implement yet!");
}
