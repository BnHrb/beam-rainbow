#ifndef CHARTCREATE_H
#define CHARTCREATE_H

#include <QDialog>

namespace Ui {
class chartcreate;
}

class chartcreate : public QDialog
{
    Q_OBJECT

public:
    explicit chartcreate(QWidget *parent = 0);
    ~chartcreate();

private slots:
    void on_type_chart_combo_currentIndexChanged(int index);

    void on_type_chart_combo_currentIndexChanged(const QString &arg1);

    void on_comboBox_question_currentIndexChanged(int index);

private:
    Ui::chartcreate *ui;
};

#endif // CHARTCREATE_H
