/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "regextesterwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    static const QString aboutInfo;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    RegexTesterWindow *getActiveTesterWindow();

private slots:
    void on_actionNewTester_triggered();
    void on_actionExit_triggered();
    void on_actionClear_triggered();
    void on_actionRun_triggered();

    void on_action_Import_Testee_triggered();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

    void on_actionClearTestee_triggered();

    void on_actionGeneratePatternForCoding_triggered();

    void on_actionAbout_triggered();

    void on_actionHelp_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
