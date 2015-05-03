#ifndef QUESTIONDIALOG_H
#define QUESTIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QListWidget>
#include <QDebug>
#include <QSqlQuery>

namespace Ui {
class QuestionDialog;
}

class QuestionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionDialog(int s_id, bool flag, QListWidget &questions, QWidget *parent = 0);
    ~QuestionDialog();
    QListWidget *qList;
    int sondage_id;
    bool edit;
    int question_id;
    QList<int> remove;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_buttonBox_accepted();

private:
    Ui::QuestionDialog *ui;
};

#endif // QUESTIONDIALOG_H
