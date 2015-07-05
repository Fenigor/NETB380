#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secdialog.h"
#include "musicplayerdialog.h"
#include <QtSql>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->SearchByBox->addItem("Song_name");
    ui->SearchByBox->addItem("Artist_name");
    ui->SearchByBox->addItem("Album_name");
    ui->SearchByBox->addItem("Genre_type");

    QString servername = "localhost";
    QString dbname = "test";
    QString pass = "admin";

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(servername);
    db.setDatabaseName(dbname);
    db.setUserName("postgres");
    db.setPassword(pass);

    if(db.open())
    {
        qDebug() << "Opened!";
    }
    else
    {
        qDebug() << "Error = " << db.lastError().text();
    }

    this->qmodel = new QSqlQueryModel();
    qmodel->setQuery("select songs.song_name,artist.artist_name,album.album_name, songs.address,genre.genre_type, songs.year from songs \
                     join artist on songs.artist = artist.artist_id \
                     join album on songs.album = album_id \
                     join genre on songs.genre = genre_id");
    ui->tableView->setModel(qmodel);

}

MainWindow::~MainWindow()
{
    db.close();
    qDebug() << "DB CLOSED!"<< endl;
    delete ui;

}

void MainWindow::on_Add_clicked()
{
    SecDialog secdialog;
    secdialog.setModal(true);
    secdialog.exec();
}

void MainWindow::on_Play_clicked()
{
    MusicPlayerDialog musicplayerdialog;
    musicplayerdialog.get_string_to_play(selected);
    musicplayerdialog.setModal(true);
    musicplayerdialog.exec();
}

void MainWindow::on_GoTo_clicked()
{
    QString GoToStr = selected;
    int i = GoToStr.size();
    while(GoToStr[i]!= '/')
    {
        i--;
    }
    i= GoToStr.size() - i;
    GoToStr.chop(i-1);
    qDebug() << GoToStr << endl;
    QDesktopServices::openUrl(QUrl("file:///" + GoToStr, QUrl::TolerantMode));
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    selected = ui->tableView->model()->data(index).toString();
    qDebug() << index << endl;
}

void MainWindow::on_Search_Button_clicked()
{
    qmodel->setQuery(QString("select songs.song_name, artist.artist_name, album.album_name,genre.genre_type, songs.address from songs \
        join artist on songs.artist = artist.artist_id \
        join album on songs.album = album_id \
        join genre on songs.genre = genre_id \
        where %2 = '%1'").arg(ui->lineEdit->text()).arg(ui->SearchByBox->currentText()));
    qDebug() << ui->lineEdit->text() << endl;
    ui->tableView->setModel(qmodel);
}
