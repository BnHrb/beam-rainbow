#include "chartcreate.h"
#include "ui_chartcreate.h"

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
    repaintplot();
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

void chartcreate::repaintplot()
{
    QCustomPlot * customPlot = ui->chart_preview;
    // create empty bar chart objects:

    QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(regen);
    customPlot->addPlottable(nuclear);
    customPlot->addPlottable(fossil);
    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    fossil->setName("Danone");
    pen.setColor(QColor(255, 131, 0));
    fossil->setPen(pen);
    fossil->setBrush(QColor(255, 131, 0, 50));
    nuclear->setName("AuBonLait");
    pen.setColor(QColor(1, 92, 191));
    nuclear->setPen(pen);
    nuclear->setBrush(QColor(1, 92, 191, 50));
    regen->setName("Yoplait");
    pen.setColor(QColor(150, 222, 0));
    regen->setPen(pen);
    regen->setBrush(QColor(150, 222, 0, 70));
    // stack bars ontop of each other:
    nuclear->moveAbove(fossil);
    regen->moveAbove(nuclear);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 ;
    labels << "Homme" << "Femme" ;
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, 8);

    // prepare y axis:
    customPlot->yAxis->setRange(0, 12.1);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Marque de Yaourt Connu");
    customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);

    // Add data:
    QVector<double> fossilData, nuclearData, regenData;
    fossilData  << 0.86*10.5 << 0.83*5.5;
    nuclearData << 0.08*10.5 << 0.12*5.5;
    regenData   << 0.06*10.5 << 0.05*5.5;
    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);
    regen->setData(ticks, regenData);

    // setup legend:
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
