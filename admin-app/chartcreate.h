#ifndef CHARTCREATE_H
#define CHARTCREATE_H

#include <QDialog>
#include <QSqlQuery>
#include "chart.h"

namespace Ui {
class chartcreate;
}

class chartcreate : public QDialog
{
    Q_OBJECT

public:
    explicit chartcreate(QWidget *parent = 0, int id_sondage = 0);
    ~chartcreate();
    Chart chart;

private slots:
    void on_type_chart_combo_currentIndexChanged(int index);

    void on_type_chart_combo_currentIndexChanged(const QString &arg1);

    void on_comboBox_question_currentIndexChanged(int index);

    void on_comboBox_question_activated(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::chartcreate *ui;
};

#endif // CHARTCREATE_H
