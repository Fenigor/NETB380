#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

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
    void on_Add_clicked();

    void on_Play_clicked();

    void on_GoTo_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_Search_Button_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQueryModel *qmodel;
    QString selected;
};

#endif // MAINWINDOW_H
