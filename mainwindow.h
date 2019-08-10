#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
extern std::vector< std::pair<double,QString> > marks;
extern std::vector <int> credits;

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
