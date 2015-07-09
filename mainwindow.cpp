#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secdialog.h"
#include "musicplayerdialog.h"
#include <QtSql>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QStandardPaths>
#include <QFormLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->SearchByBox->addItem("Song_name");
    ui->SearchByBox->addItem("Artist_name");
    ui->SearchByBox->addItem("Album_name");
    ui->SearchByBox->addItem("Genre_type");

    QDialog *inputPanel = new QDialog(this);
    QFormLayout *form = new QFormLayout(inputPanel);
    QLabel *ip = new QLabel("IP Address :",inputPanel);
    QLineEdit *ipInput = new QLineEdit(inputPanel);
    form->addRow(ip,ipInput);
    QLabel *dbname = new QLabel("DataBase Name :",inputPanel);
    QLineEdit *dbInput = new QLineEdit(inputPanel);
    form->addRow(dbname,dbInput);
    QLabel *userName = new QLabel("Username :",inputPanel);
    QLineEdit *userNameInput = new QLineEdit(inputPanel);
    form->addRow(userName,userNameInput);
    QLabel *password = new QLabel("Password :",inputPanel);
    QLineEdit *passInput = new QLineEdit(inputPanel);
    form->addRow(password,passInput);
    QPushButton *connect_btn = new QPushButton(inputPanel);
    connect_btn->setText("Connect to Database");
    connect_btn->connect(connect_btn,SIGNAL(clicked()),inputPanel,SLOT(close()));
    form->addRow(connect_btn);

    inputPanel->exec();

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(ipInput->text());
    db.setDatabaseName(dbInput->text());
    db.setUserName(userNameInput->text());
    db.setPassword(passInput->text());

    if(db.open())
    {
        qDebug() << "Opened!";
        this->qmodel = new QSqlQueryModel();
        qmodel->setQuery("select songs.song_name,artist.artist_name,album.album_name, songs.address,genre.genre_type, songs.year from songs \
                         join artist on songs.artist = artist.artist_id \
                         join album on songs.album = album_id \
                         join genre on songs.genre = genre_id");
        ui->tableView->setModel(qmodel);
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Database didn't open!Probabbly wrong input.");
        messageBox.setFixedSize(500,200);
        qDebug() << "Error = " << db.lastError().text();
    }
}
/* Destructor
 * closes database connecting when called */
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
    QDesktopServices::openUrl(QUrl("file:///" + GoToStr, QUrl::TolerantMode));
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    selected = ui->tableView->model()->data(index).toString();
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

void MainWindow::on_pushButton_clicked()
{
    qmodel->setQuery("select songs.song_name,artist.artist_name,album.album_name, songs.address,genre.genre_type, songs.year from songs \
                     join artist on songs.artist = artist.artist_id \
                     join album on songs.album = album_id \
                     join genre on songs.genre = genre_id");
    ui->tableView->setModel(qmodel);
}
