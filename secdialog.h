#ifndef SECDIALOG_H
#define SECDIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QMessageBox>
#include "mainwindow.h"

namespace Ui {
class SecDialog;
}

class SecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecDialog(QWidget *parent = 0);
    ~SecDialog();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_ok_button_clicked();

    QVariant check_artist(char artist[31]);

    QVariant check_album(char album[31]);

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::SecDialog *ui;
    QSqlDatabase db;
    QString sPath;
    QMessageBox messageBox;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
};

#endif // SECDIALOG_H
