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
    QSqlQuery query;
    query.prepare("SELECT * FROM entreprises WHERE nom=(:nom) AND mdp=(:mdp)");
    query.bindValue(":nom", ui->UserLineEdit->text());
    query.bindValue(":mdp", ui->PasswordLineEdit->text());
    query.exec();

    if(query.next()){
        QSqlQuery sondages;
        sondages.prepare("SELECT nom FROM sondages");
        sondages.bindValue(":id", query.value(0));
        sondages.exec();

        while (sondages.next()) {
            QString titre = sondages.value(0).toString();
            ui->surveyListWidget->addItem(QString(titre));
        }

        ui->stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_surveyDeleteButton_clicked()
{
    qWarning() << ui->surveyListWidget->currentItem()->text();
    //QSqlQuery query;
    //query.exec("DELETE FROM sondages WHERE nom="+ui->surveyListWidget->currentItem()->text());
    delete ui->surveyListWidget->currentItem();
}

void MainWindow::on_surveyAddButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_deleteQuestionButton_clicked()
{
    QSqlQuery query;
    query.exec("DELETE FROM questions WHERE valeur="+ui->questionsListWidget->currentItem()->text());
    delete ui->questionsListWidget->currentItem();
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

void MainWindow::on_surveyEditButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT titre FROM sondages WHERE titre=(:titre)");
    query.bindValue(":titre", ui->surveyListWidget->currentItem()->text());
    query.exec();

    ui->nameSurveyLineEdit->setText(ui->surveyListWidget->currentItem()->text());
    while(query.next()){
        QString valeur = query.value(0).toString();
        ui->questionsListWidget->addItem(QString(valeur));
    }

    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_editQuestionButton_clicked()
{
    ui->questionsListWidget->currentItem()->text();
}
