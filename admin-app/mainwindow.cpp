#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "questiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_surveyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_surveyDeleteButton_clicked()
{
    qDeleteAll(ui->surveyListWidget->selectedItems());
}

void MainWindow::on_surveyAddButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_deleteQuestionButton_clicked()
{
    qDeleteAll(ui->questionsListWidget->selectedItems());
}

void MainWindow::on_addQuestionButton_clicked()
{
    QuestionDialog qDialog;
    qDialog.setModal(true);
    qDialog.exec();
}
