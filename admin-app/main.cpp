#include "mainwindow.h"
#include <QApplication>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QTranslator>

bool createConnection();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!createConnection()){

        qDebug() << "Not connected!";
    }
    else{
        qDebug() << "Connected!";
    }

    MainWindow w;
    w.show();

    return a.exec();
}

bool createConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3");
    db.setHostName("btrd.me");
    db.setDatabaseName("library_development");
    db.setUserName("beam-rainbow");
    db.setPassword("boF7Os0yAup0iL2iaFt3");
    if (!db.open()) {
        qDebug() << "Database error occurred";
        return false;
    }
    return true;
}
