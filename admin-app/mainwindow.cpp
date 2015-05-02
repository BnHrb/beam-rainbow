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

    ui->surveyListWidget->clear();

    if(query.next()){
        QSqlQuery sondages;
        sondages.prepare("SELECT * FROM sondages");
        sondages.bindValue(":id", query.value(0));
        sondages.exec();

        while (sondages.next()) {
            QListWidgetItem *item = new QListWidgetItem(sondages.value("nom").toString());
            QVariant v;
            v.setValue(sondages.value("id").toInt());
            item->setData(Qt::UserRole, v);

            ui->surveyListWidget->addItem(item);
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
    QuestionDialog *qDialog = new QuestionDialog(ui->surveyListWidget->currentItem()->data(Qt::UserRole).value<int>(),0, *ui->questionsListWidget, this);
    qDialog->setModal(true);
    qDialog->exec();
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
    query.prepare("SELECT * FROM questions WHERE sondage_id=(:id)");
    query.bindValue(":id", ui->surveyListWidget->currentItem()->data(Qt::UserRole).value<int>());
    query.exec();

    ui->nameSurveyLineEdit->setText(ui->surveyListWidget->currentItem()->text());
    while(query.next()){
        QListWidgetItem *item = new QListWidgetItem(query.value("valeur").toString());
        QVariant v;
        v.setValue(query.value("id").toInt());
        item->setData(Qt::UserRole, v);

        ui->questionsListWidget->addItem(item);
    }

    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_editQuestionButton_clicked()
{
    QuestionDialog *qDialog = new QuestionDialog(ui->surveyListWidget->currentItem()->data(Qt::UserRole).value<int>(),1,*ui->questionsListWidget, this);
    qDialog->setModal(true);
    qDialog->exec();
}
