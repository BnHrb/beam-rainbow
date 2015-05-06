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

    if(ui->surveyListWidget->currentItem() == NULL){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Attention", "Vous avez seléctionné aucun sondage.", QMessageBox::Ok);
    }
    else{
        current_sondage_id = ui->surveyListWidget->currentItem()->data(Qt::UserRole).value<int>();
        ui->label_chart_title->setText(ui->surveyListWidget->currentItem()->text());
        ui->stackedWidget->setCurrentIndex(3);

        /* chart exemple */
        QCustomPlot * customPlot = ui->chart_widget1;
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

        QCustomPlot * customPlot2 = ui->chart_widget2;
        // create empty bar chart objects:

        QCPBars *regen2 = new QCPBars(customPlot2->xAxis, customPlot2->yAxis);
        QCPBars *nuclear2 = new QCPBars(customPlot2->xAxis, customPlot2->yAxis);
        QCPBars *fossil2 = new QCPBars(customPlot2->xAxis, customPlot2->yAxis);
        customPlot2->addPlottable(regen2);
        customPlot2->addPlottable(nuclear2);
        customPlot2->addPlottable(fossil2);
        // set names and colors:
        QPen pen2;
        pen2.setWidthF(1.2);
        fossil2->setName("Danone");
        pen2.setColor(QColor(255, 131, 0));
        fossil2->setPen(pen);
        fossil2->setBrush(QColor(255, 131, 0, 50));
        nuclear2->setName("AuBonLait");
        pen2.setColor(QColor(1, 92, 191));
        nuclear2->setPen(pen);
        nuclear2->setBrush(QColor(1, 92, 191, 50));
        regen2->setName("Yoplait");
        pen2.setColor(QColor(150, 222, 0));
        regen2->setPen(pen);
        regen2->setBrush(QColor(150, 222, 0, 70));
        // stack bars ontop of each other:
        nuclear2->moveAbove(fossil2);
        regen2->moveAbove(nuclear2);

        // prepare x axis with country labels:
        QVector<double> ticks2;
        QVector<QString> labels2;
        ticks2 << 1 << 2 ;
        labels2 << "Homme" << "Femme" ;
        customPlot2->xAxis->setAutoTicks(false);
        customPlot2->xAxis->setAutoTickLabels(false);
        customPlot2->xAxis->setTickVector(ticks2);
        customPlot2->xAxis->setTickVectorLabels(labels2);
        customPlot2->xAxis->setTickLabelRotation(60);
        customPlot2->xAxis->setSubTickCount(0);
        customPlot2->xAxis->setTickLength(0, 8);
        customPlot2->xAxis->grid()->setVisible(true);
        customPlot2->xAxis->setRange(0, 8);

        // prepare y axis:
        customPlot2->yAxis->setRange(0, 12.1);
        customPlot2->yAxis->setPadding(5); // a bit more space to the left border
        customPlot2->yAxis->setLabel("Marques de Yaourts Acheté");
        customPlot2->yAxis->grid()->setSubGridVisible(true);
        QPen gridPen2;
        gridPen2.setStyle(Qt::SolidLine);
        gridPen2.setColor(QColor(0, 0, 0, 25));
        customPlot2->yAxis->grid()->setPen(gridPen2);
        gridPen2.setStyle(Qt::DotLine);
        customPlot2->yAxis->grid()->setSubGridPen(gridPen2);

        // Add data:
        QVector<double> fossilData2, nuclearData2, regenData2;
        fossilData2  << 0.50*10.5 << 0.55*5.5;
        nuclearData2 << 0.25*10.5 << 0.30*5.5;
        regenData2   << 0.25*10.5 << 0.15*5.5;
        fossil2->setData(ticks2, fossilData2);
        nuclear2->setData(ticks2, nuclearData2);
        regen2->setData(ticks2, regenData2);

        // setup legend:
        customPlot2->legend->setVisible(true);
        customPlot2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
        customPlot2->legend->setBrush(QColor(255, 255, 255, 200));
        QPen legendPen2;
        legendPen2.setColor(QColor(130, 130, 130, 200));
        customPlot2->legend->setBorderPen(legendPen2);
        QFont legendFont2 = font();
        legendFont2.setPointSize(10);
        customPlot2->legend->setFont(legendFont);
        customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    chartcreate *chartDialog = new chartcreate(this, current_sondage_id);
    chartDialog->setModal(true);
    chartDialog->exec();
}
