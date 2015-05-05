#include "chartcreate.h"
#include "ui_chartcreate.h"
#include<iostream>
#include <QDebug>

int idQuestion_comboSondage[255];
int idReponse = 0;

chartcreate::chartcreate(QWidget *parent, int idSondage) :
    QDialog(parent),
    ui(new Ui::chartcreate)
{
    int i = 0;
    ui->setupUi(this);
    ui->combopop->clear();
    for (i=0 ; i < 3; i++)
        ui->combopop->addItem(chart.population[i]);
    ui->combo_categ->clear();
    for (i=0 ; i < 6; i++)
        ui->combo_categ->addItem(chart.categ[i]);

    i=0;
    ui->type_chart_combo->clear();
    ui->type_chart_combo->addItem("Toutes les réponses");
    ui->type_chart_combo->addItem("Réponse spécifique");
    this->chart.set_id_sondage(idSondage);
    QSqlQuery query;
    query.prepare("SELECT * FROM questions WHERE sondage_id =(:idSondage)");
    query.bindValue(":idSondage", idSondage);
    query.exec();
    while (query.next())
    {
        i++;
        idQuestion_comboSondage[i] = query.value("id").toInt();
        ui->comboBox_question->addItem(query.value("valeur").toString(),
                                       query.value("id").toString());
    }
    query.prepare("SELECT * FROM choixes WHERE question_id=(:idQuestion)");
    query.bindValue(":idQuestion", idQuestion_comboSondage[1]);
    query.exec();
    while (query.next())
    {
        ui->comboBox_response->addItem(query.value("valeur").toString(),
                                       query.value("id").toString());
    }
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
    {
        ui->stack_form_chart->setCurrentIndex(1);
    }
}

void chartcreate::on_comboBox_question_currentIndexChanged(int index)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM choixes WHERE question_id=(:idQuestion)");
    query.bindValue(":idQuestion", idQuestion_comboSondage[index]);
    query.exec();
    while (query.next())
    {
        ui->comboBox_response->addItem(query.value("valeur").toString(),
                                       query.value("id").toString());
    }
}

void chartcreate::on_pushButton_clicked()
{
    chart.set_id_question(ui->comboBox_question->currentData().toInt());
    if (ui->type_chart_combo->currentIndex() == 0)
        chart.set_reponse_spe(false);
    else
        chart.set_reponse_spe(true);
    chart.set_reponse_given(ui->comboBox_response->currentData().toInt());
    chart.set_moyenne(ui->radio_pourcent->isChecked());
    chart.set_moyenne(ui->radioButton_2->isChecked());
    chart.set_pop_cible(ui->combopop->currentText());
    chart.set_axe_abscisse(ui->combo_categ->currentText());
    if (chart.save())
    {
        qDebug() << "Sauvegarde réussi";
        this->close();
    }
    else
        qDebug() << "Echec lors de la sauvegarde";

}

void chartcreate::on_pushButton_2_clicked()
{
    this->close();
}
