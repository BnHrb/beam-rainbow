#include "chartcreate.h"
#include "ui_chartcreate.h"

chartcreate::chartcreate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chartcreate)
{
    ui->setupUi(this);
    ui->comboBox_question->addItem("Question 1","1");
    ui->comboBox_question->addItem("Question 2","2");
    ui->comboBox_question->addItem("Question 3","3");
}

chartcreate::~chartcreate()
{
    delete ui;
}

void chartcreate::on_type_chart_combo_currentIndexChanged(int index)
{
    if (index == 0)
        ui->stack_form_chart->setCurrentIndex(0);
    else
        ui->stack_form_chart->setCurrentIndex(1);
}

void chartcreate::on_comboBox_question_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->comboBox_response->clear();
        ui->comboBox_response->addItem("Reponse 1","1");
    }
    else if (index == 1)
    {
        ui->comboBox_response->clear();
        ui->comboBox_response->addItem("Reponse 1","1");
        ui->comboBox_response->addItem("Reponse 2","2");
    }
    else if (index == 2)
    {
        ui->comboBox_response->clear();
        ui->comboBox_response->addItem("Reponse 1","1");
        ui->comboBox_response->addItem("Reponse 2","2");
        ui->comboBox_response->addItem("Reponse 3","3");
    }
}
