#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <vector>
#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>
#include <QtDebug>

#include <utility>


extern std::vector< std::pair<double,QString> > marks;
extern std::vector <int> credits;
extern double credit_sum;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_mark_add_button_clicked();

    void on_mark_delete_button_clicked();

    void on_gpa_button_clicked();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    double calculateMark();
};

#endif // MAINWINDOW_H
