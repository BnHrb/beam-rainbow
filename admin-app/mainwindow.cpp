#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "questiondialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString val;
    QFile file;
    file.setFileName("/Users/Ben/Documents/Cours/Licence/S6/IHM/Projet/beam-rainbow/admin-app/test.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    //qWarning() << val;
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    jsonSondages = d.object();
    qWarning() << d;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_surveyButton_clicked()
{
    QJsonObject sondages = jsonSondages;
    qWarning() << sondages;
    //ui->surveyListWidget;
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

void MainWindow::on_newSurveyButtonBox_rejected()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_newSurveyButtonBox_accepted()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_buttonBox_accepted()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_buttonBox_rejected()
{
    ui->stackedWidget->setCurrentIndex(0);
}
