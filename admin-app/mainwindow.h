#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int entreprise_id;

private slots:
    void on_surveyButton_clicked();

    void on_surveyDeleteButton_clicked();

    void on_surveyAddButton_clicked();

    void on_deleteQuestionButton_clicked();

    void on_addQuestionButton_clicked();

    void on_newSurveyButtonBox_rejected();

    void on_newSurveyButtonBox_accepted();

    void on_surveyEditButton_clicked();

    void on_editQuestionButton_clicked();

    void on_logoutPushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
