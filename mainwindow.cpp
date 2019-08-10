#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QtDebug>
#include <QMessageBox>
#include <utility>

using std::make_pair;
using std::pair;
using std::vector;
using std::string;

double label_mark = 0;
vector< pair<double,QString> > marks{
    make_pair(.5,"F"),make_pair(1.5,"D-"),make_pair(1.75,"D"),make_pair(2,"D+"),
            make_pair(2.25,"C-"),make_pair(2.5,"C"),make_pair(2.75,"C+"),
            make_pair(3,"B-"),make_pair(3.25,"B"),make_pair(3.5,"B+"),
            make_pair(3.75,"A-"),make_pair(4,"A"),make_pair(4.2,"A+")
};
vector <int> credits{0,1,2,3,4,5};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mark_view->setColumnCount(3);
    ui->mark_view->setHorizontalHeaderItem(0,new QTableWidgetItem("Credit"));
    ui->mark_view->setHorizontalHeaderItem(1,new QTableWidgetItem("Mark"));
    ui->mark_view->setHorizontalHeaderItem(2,new QTableWidgetItem("Letter"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mark_add_button_clicked()
{

    if(ui->credit_edit->displayText() != "" && ui->mark_edit->displayText() != "")
    {
        QString curr_mark = ui->mark_edit->displayText();
        QString curr_credit = ui->credit_edit->displayText();
        QString curr_letter;

        bool is_mark_correct = false ,is_credit_correct = false;
        for(auto &x : marks)
            if(curr_mark.toDouble() == x.first)
            {
                is_mark_correct = true;
                curr_letter = x.second;
                break;
            }
        for(auto &x : credits)
            if(curr_credit.toDouble() == x)
            {
                is_credit_correct = true;
                break;
            }


        if(is_mark_correct == true && is_mark_correct == true)
        {
            int row_count = ui->mark_view->rowCount();
            ui->mark_view->insertRow(row_count);
            QTableWidgetItem *newMark = new QTableWidgetItem();
            QTableWidgetItem *newCredit = new QTableWidgetItem();
            QTableWidgetItem *newCreditLetter = new QTableWidgetItem();

            newCredit->setText(ui->credit_edit->displayText());
            newMark->setText(ui->mark_edit->displayText());
            newCreditLetter->setText(curr_letter);

            ui->mark_view->setItem(row_count,0,newCredit);
            ui->mark_view->setItem(row_count,1,newMark);
            ui->mark_view->setItem(row_count,2,newCreditLetter);

            label_mark += curr_credit.toDouble();
            ui->curr_credit_label->setText("Current credit: "+QString::number(label_mark));

        }
        else
        {
            QMessageBox::critical(this,"Wrong Input","Numbers out of range/not correct!",QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::critical(this,"Wrong Input","Can't add empty numbers!",QMessageBox::Ok);
    }



}
void MainWindow::on_mark_delete_button_clicked()
{
    QList<QTableWidgetItem* > items = ui->mark_view->selectedItems();
     for(auto &x : items)
        label_mark -= ui->mark_view->model()->index(x->row(),0).data().toString().toDouble();
    /*  Note to future self: this peice of code is taken from
     *  https://stackoverflow.com/questions/8845069/crash-in-deleting-selected-items-from-qtablewidget
     */
    QList<QModelIndex> indecies = ui->mark_view->selectionModel()->selectedIndexes();
    for(auto &x : indecies)
        ui->mark_view->removeRow(x.row());
    ui->curr_credit_label->setText("Current credit: "+QString::number(label_mark));
}

void MainWindow::on_gpa_button_clicked()
{
    int row_count = ui->mark_view->rowCount();
    int column_count = ui->mark_view->columnCount();
    double credit_sum = 0;
    double dem_sum =0 ;
    for(int i = 0 ;i < row_count ; i++)
        for(int j = 0 ;j < column_count ; j++)
        {
            credit_sum += ui->mark_view->item(i,0)->text().toDouble();
            dem_sum += ui->mark_view->item(i,0)->text().toDouble() * ui->mark_view->item(i,1)->text().toDouble();
        }
    double gpa = dem_sum/credit_sum;
    QMessageBox::information(this,"Result","Your Gpa is: " + QString::number(gpa));
}
