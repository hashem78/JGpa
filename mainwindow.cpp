#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ios>
#include <sstream>
using std::make_pair;
using std::pair;
using std::vector;
using std::string;
using std::ofstream;

double credit_sum = 0;
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
    connect(ui->marks_combo_box, QOverload<int>::of(&QComboBox::highlighted),
        [&](int index){ ui->mark_edit->setText(ui->marks_combo_box->itemText(index).mid(0,4)); });

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


        if(is_credit_correct == true && is_mark_correct == true)
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

            credit_sum += curr_credit.toDouble();
            calculateMark();

            ui->curr_credit_label->setText("Current credit: "+QString::number(credit_sum));

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
         credit_sum -= ui->mark_view->model()->index(x->row(),0).data().toString().toDouble();

    /*  Note to future self: this peice of code is taken from
     *  https://stackoverflow.com/questions/8845069/crash-in-deleting-selected-items-from-qtablewidget
     */
    QList<QModelIndex> indecies = ui->mark_view->selectionModel()->selectedIndexes();
    for(auto &x : indecies)
    {
        ui->mark_view->removeRow(x.row());
        calculateMark();
    }
    if(credit_sum > 0)
    ui->curr_credit_label->setText("Current credit: "+QString::number(credit_sum));
    else ui->curr_credit_label->setText("0");
}

void MainWindow::on_gpa_button_clicked()
{
    QMessageBox::information(this,"Result","Your Gpa is: " + QString::number(calculateMark()));
}
double MainWindow::calculateMark()
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
    ui->curr_mark_label->setText("Current mark: " + QString::number(gpa));
    return gpa;
}

void MainWindow::on_actionSave_triggered()
{
    QFileDialog *file_dialog = new QFileDialog;
    file_dialog->setDirectory("C:/");
    QString file_name =  file_dialog->getSaveFileName(this,
                                 tr("Save profile"), "",
                                 tr("txt File (*.txt);;All Files (*)"));
    vector < pair<double,string> > table_items;
    int row_count = ui->mark_view->rowCount();
    for(int i = 0 ; i < row_count; i++)
            table_items.push_back( make_pair(ui->mark_view->item(i,1)->text().toDouble(),ui->mark_view->item(i,0)->text().toStdString()));
    ofstream file(file_name.toStdString());
    if(!file.is_open())
        return;
    for(auto &x : table_items)
        file << x.first << " " << x.second << std::endl;

    file.close();


}

void MainWindow::on_actionLoad_triggered()
{
    ui->mark_view->setRowCount(0);
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Load profile"), "",
                                            tr("txt File (*.txt);;All Files (*)"));

    std::ifstream file(filename.toStdString());

    QString curr_letter;
    QString curr_credit;
    QString curr_mark;
    //
    string temp;

    double temp_double;
    int temp_int;
    //
    vector < pair<double,int> > table_items;

    while(std::getline(file,temp))
    {
        std::stringstream tempStream(temp);
        tempStream >> temp_double >> temp_int;
        table_items.push_back(make_pair(temp_double,temp_int));
    }

    for(auto &x : table_items){

    int row_count = ui->mark_view->rowCount();
    ui->mark_view->insertRow(row_count);
    QTableWidgetItem *newMark = new QTableWidgetItem();
    QTableWidgetItem *newCredit = new QTableWidgetItem();
    QTableWidgetItem *newCreditLetter = new QTableWidgetItem();

    curr_mark = QString::number(x.first);
    curr_credit = QString::number(x.second);
    for(auto &y : marks)
        if(curr_mark == QString::number(y.first))
        {
            curr_letter = y.second;
            qDebug() << curr_mark;
            break;
        }
    newCredit->setText(curr_credit);
    newMark->setText(curr_mark);
    newCreditLetter->setText(curr_letter);

    ui->mark_view->setItem(row_count,0,newCredit);
    ui->mark_view->setItem(row_count,1,newMark);
    ui->mark_view->setItem(row_count,2,newCreditLetter);

    credit_sum += curr_credit.toDouble();

    ui->curr_credit_label->setText("Current credit: "+QString::number(credit_sum));
    }
    calculateMark();

}
