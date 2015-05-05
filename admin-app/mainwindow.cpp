#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    if(ui->UserLineEdit->text() == "" || ui->PasswordLineEdit->text() == ""){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Attention", "Vous n'avez pas rempli tous les champs.", QMessageBox::Ok);
    }
    else {
        QSqlQuery query;
        query.prepare("SELECT * FROM entreprises WHERE nom=(:nom) AND mdp=(:mdp)");
        query.bindValue(":nom", ui->UserLineEdit->text());
        query.bindValue(":mdp", ui->PasswordLineEdit->text());
        query.exec();

        ui->newSurveyButtonBox->button(QDialogButtonBox::Cancel)->setText("Annuler");

        ui->surveyListWidget->clear();

        if(query.next()){

            entreprise_id = query.value("id").toInt();

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
        else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, "Attention", "L'utilisateur ou le mot de passe est incorect.", QMessageBox::Ok);
        }
    }
}

void MainWindow::on_surveyDeleteButton_clicked()
{
    if(ui->surveyListWidget->currentItem() != NULL){
        int sondage_id = ui->surveyListWidget->currentItem()->data(Qt::UserRole).value<int>();
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer ce sondage ?", QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::Yes){
            QSqlQuery query;
            query.prepare("DELETE c.* FROM choixes c INNER JOIN questions q ON q.id = c.question_id WHERE (q.sondage_id = :id) ;"
                          "DELETE FROM questions WHERE sondage_id = :id ;"
                          "DELETE FROM sondages WHERE id = :id");
            query.bindValue(":id", sondage_id);
            query.exec();

            delete ui->surveyListWidget->currentItem();
        }
    }
    else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Attention", "Vous avez seléctionné aucun sondage.", QMessageBox::Ok);
    }
}

void MainWindow::on_surveyAddButton_clicked()
{
    QSqlQuery query;
    query.prepare("INSERT INTO sondages (nom, entreprise_id) VALUES (:nom, :id)");
    query.bindValue(":nom", "nouveau sondage");
    query.bindValue(":id", entreprise_id);
    query.exec();

    int id = query.lastInsertId().toInt();
    QListWidgetItem *item = new QListWidgetItem("nouveau sondage");
    QVariant v;
    v.setValue(id);
    item->setData(Qt::UserRole, v);

    ui->surveyListWidget->addItem(item);
    ui->surveyListWidget->setCurrentItem(item);

    ui->questionsListWidget->clear();
    ui->nameSurveyLineEdit->clear();
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_deleteQuestionButton_clicked()
{
    if(ui->questionsListWidget->currentItem() == NULL){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Attention", "Vous n'avez pas seléctionné de question.", QMessageBox::Ok);
    }
    else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer cette question ?", QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::Yes){
            QSqlQuery query;
            query.prepare("DELETE FROM choixes WHERE question_id=(:id) ;"
                          "DELETE FROM questions WHERE id=(:id)");
            query.bindValue(":id", ui->questionsListWidget->currentItem()->data(Qt::UserRole).value<int>());
            query.exec();

            delete ui->questionsListWidget->currentItem();
        }
    }
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
    if(ui->nameSurveyLineEdit->text() == ""){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Oubli ?", "Vous n'avez pas nommer votre sondage, voulez vous quand même continuer ?", QMessageBox::Yes|QMessageBox::No);
    }

    QSqlQuery query;
    query.prepare("UPDATE sondages SET nom=(:nom) WHERE id=(:id)");
    query.bindValue(":nom", ui->nameSurveyLineEdit->text());
    query.bindValue(":id", ui->surveyListWidget->currentItem()->data(Qt::UserRole).value<int>());
    query.exec();

    ui->surveyListWidget->currentItem()->setText(ui->nameSurveyLineEdit->text());
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_surveyEditButton_clicked()
{
    if(ui->surveyListWidget->currentItem() == NULL){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Attention", "Vous avez seléctionné aucun sondage.", QMessageBox::Ok);
    }
    else{
        QSqlQuery query;
        query.prepare("SELECT * FROM questions WHERE sondage_id=(:id)");
        query.bindValue(":id", ui->surveyListWidget->currentItem()->data(Qt::UserRole).value<int>());
        query.exec();

        ui->nameSurveyLineEdit->setText(ui->surveyListWidget->currentItem()->text());
        ui->questionsListWidget->clear();
        while(query.next()){
            QListWidgetItem *item = new QListWidgetItem(query.value("valeur").toString());
            QVariant v;
            v.setValue(query.value("id").toInt());
            item->setData(Qt::UserRole, v);

            ui->questionsListWidget->addItem(item);
        }

        ui->stackedWidget->setCurrentIndex(2);
    }
}

void MainWindow::on_editQuestionButton_clicked()
{
    if(ui->questionsListWidget->currentItem() == NULL){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Attention", "Vous n'avez pas seléctionné de question.", QMessageBox::Ok);
    }
    else{
        QuestionDialog *qDialog = new QuestionDialog(ui->surveyListWidget->currentItem()->data(Qt::UserRole).value<int>(),1,*ui->questionsListWidget, this);
        qDialog->setModal(true);
        qDialog->exec();
    }
}

void MainWindow::on_logoutPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionA_Propos_triggered()
{
    QMessageBox::about(this, QString("À Propos..."), QString("\t    Beam Rainbow 1.0\n\nJean Bertrand - Benjamin Herb - Gwenaël Larmet\n\n\t    2015 - Licence MIT"));
}

void MainWindow::on_actionAfficher_aide_triggered()
{
    HelpDialog *hDialog = new HelpDialog(this);
    hDialog->setModal(true);
    hDialog->exec();
}

void MainWindow::on_surveySeeResults_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    chartcreate *chartDialog = new chartcreate(this);
    chartDialog->setModal(true);
    chartDialog->exec();
}
