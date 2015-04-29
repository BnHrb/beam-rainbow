#include "questiondialog.h"
#include "ui_questiondialog.h"
#include "iostream"

QuestionDialog::QuestionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionDialog)
{
    ui->setupUi(this);

    QComboBox* qCombo = new QComboBox(this);
    qCombo->addItem("Texte");
    qCombo->addItem("Nombre");

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setCellWidget(0, 1, qCombo);
}

QuestionDialog::~QuestionDialog()
{
    delete ui;
}

void QuestionDialog::on_pushButton_clicked()
{
    QComboBox* qCombo = new QComboBox(this);
    qCombo->addItem("Texte");
    qCombo->addItem("Nombre");

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, 1, qCombo);
    //std::cout << ui->tableWidget->rowCount() << std::endl;
}

void QuestionDialog::on_pushButton_2_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}
