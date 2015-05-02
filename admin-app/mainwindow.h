#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_surveyButton_clicked();

    void on_surveyDeleteButton_clicked();

    void on_surveyAddButton_clicked();

    void on_deleteQuestionButton_clicked();

    void on_addQuestionButton_clicked();

    void on_newSurveyButtonBox_rejected();

    void on_newSurveyButtonBox_accepted();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
