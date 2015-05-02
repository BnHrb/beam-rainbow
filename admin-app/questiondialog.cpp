#include "questiondialog.h"
#include "ui_questiondialog.h"
#include "iostream"

QuestionDialog::QuestionDialog(int s_id, bool flag, QListWidget &questions, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionDialog)
{
    ui->setupUi(this);
    qList = &questions;
    sondage_id = s_id;
    edit = flag;
    question_id = qList->currentItem()->data(Qt::UserRole).value<int>();

    if(edit){
        ui->plainTextEdit->appendPlainText(qList->currentItem()->text());

        QSqlQuery query;
        query.prepare("SELECT * FROM choixes WHERE question_id=(:id)");
        query.bindValue(":id", question_id);
        query.exec();

        QSqlQuery question;
        question.prepare("SELECT choix_multiple, champs_libre FROM questions WHERE id=(:id)");
        question.bindValue(":id", question_id);
        question.exec();

        if(question.next()){
            if(question.value(0) == 1)
                ui->checkBox->setChecked(true);
            if(question.value(1) == 1)
                ui->checkBox_2->setChecked(true);
        }

        while(query.next()){
            QListWidgetItem *item = new QListWidgetItem(query.value("valeur").toString());
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            QVariant v;
            v.setValue(query.value("id").toInt());
            item->setData(Qt::UserRole, v);

            ui->listWidget->addItem(item);
        }
    }

//    QComboBox* qCombo = new QComboBox(this);
//    qCombo->addItem("Texte");
//    qCombo->addItem("Nombre");

//    ui->tableWidget->insertRow(0);
//    ui->tableWidget->setCellWidget(0, 1, qCombo);
}

QuestionDialog::~QuestionDialog()
{
    delete ui;
}

void QuestionDialog::on_pushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(QString(""));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->listWidget->addItem(item);
    ui->listWidget->item(ui->listWidget->count()-1)->setSelected(true);
    //std::cout << ui->tableWidget->rowCount() << std::endl;
}

void QuestionDialog::on_pushButton_2_clicked()
{
    //ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    delete ui->listWidget->currentItem();
}

void QuestionDialog::on_buttonBox_accepted()
{
    if(!edit){
        QSqlQuery query;
        query.prepare("INSERT INTO questions (valeur, choix_multiple, champs_libre, sondage_id) VALUES (:valeur, :mul, :num, :s_id)");
        query.bindValue(":valeur", ui->plainTextEdit->toPlainText());
        query.bindValue(":mul", ui->checkBox->isChecked());
        query.bindValue(":num", ui->checkBox_2->isChecked());
        query.bindValue(":s_id", sondage_id);
        query.exec();

        int id = query.lastInsertId().toInt();
        qWarning() << id;
        while(query.next())
            id = query.value(0).toInt();

        for(int i=0; i < ui->listWidget->count(); i++){
            qWarning() << ui->listWidget->item(i)->text();
            QSqlQuery answer;
            answer.prepare("INSERT INTO choixes (valeur, question_id) VALUES (:valeur, :id)");
            answer.bindValue(":valeur", ui->listWidget->item(i)->text());
            answer.bindValue(":id", id);
            answer.exec();
        }

        QListWidgetItem *item = new QListWidgetItem(ui->plainTextEdit->toPlainText());
        QVariant v;
        v.setValue(id);
        item->setData(Qt::UserRole, v);
        qList->addItem(item);
    }
    else{
        QSqlQuery query;
        query.prepare("UPDATE questions SET valeur = :valeur, choix_multiple = :mul, champs_libre = :num WHERE id = :id");
        query.bindValue(":valeur", ui->plainTextEdit->toPlainText());
        query.bindValue(":mul", ui->checkBox->isChecked());
        query.bindValue(":num", ui->checkBox_2->isChecked());
        query.bindValue(":id", question_id);
        query.exec();

        for(int i=0; i < ui->listWidget->count(); i++){
            QSqlQuery answer;
            if(ui->listWidget->item(i)->data(Qt::UserRole).value<int>() > 0){
                answer.prepare("UPDATE choixes SET valeur = :valeur WHERE id = :id");
                answer.bindValue(":valeur", ui->listWidget->item(i)->text());
                answer.bindValue(":id", ui->listWidget->item(i)->data(Qt::UserRole).value<int>());
                answer.exec();
            }
            else{
                answer.prepare("INSERT INTO choixes (valeur, question_id) VALUES (:valeur, :id)");
                answer.bindValue(":valeur", ui->listWidget->item(i)->text());
                answer.bindValue(":id", question_id);
                answer.exec();
            }
        }

        qList->currentItem()->setText(ui->plainTextEdit->toPlainText());
    }

}
