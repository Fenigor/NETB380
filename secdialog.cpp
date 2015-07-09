#include "secdialog.h"
#include "ui_secdialog.h"
#include "mainwindow.h"
#include <QSql>
#include <cstdlib>
#include <fstream>
#include <string>
#include <QStandardPaths>

using namespace std;
/* Constructor
 * Creates the second dialog window
 * populates the tree and list view */
SecDialog::SecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDialog)
{
    ui->setupUi(this);

    QString sPath = QString(QStandardPaths::HomeLocation);

    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->treeView->setModel(dirmodel);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::Files);
    //filemodel->setNameFilters(QStringList()<<"*.mp3");
    filemodel->setRootPath(sPath);
    ui->listView->setModel(filemodel);
    ui->label->setText("");
}

SecDialog::~SecDialog()
{
    delete ui;
}
/* on click in the treeview adds up the path in the sPath string
 * so that its possible to go down the tree even more */
void SecDialog::on_treeView_clicked(const QModelIndex &index)
{
    sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));
}
/* on click in the listview adds up the path in the sPath string
 * when selecting a file it checks the extension at the end
 * if not mp3 display a message and turns of the ok button
 * if yes then adds it to the string sPath*/
void SecDialog::on_listView_clicked(const QModelIndex &index)
{
    qDebug() << filemodel->fileInfo(index).absoluteFilePath() << endl;
    if(index.data(Qt::DisplayRole ).toString().endsWith(".mp3"))
    {
        ui->ok_button->setEnabled(true);
        sPath= filemodel->fileInfo(index).absoluteFilePath();
    }
    else
    {
        ui->ok_button->setEnabled(false);
        messageBox.critical(0,"Error","The file you chose is not a mp3!");
        messageBox.setFixedSize(500,200);
    }
}
/* declares variables used to exctract the meta data from an mp3 file
 * creates an input and output streams
 * reads from the file, cloases it
 * uses querys to check if meta data is not present in the database
 * if not places stores in the database may require a reboot to display everything */
void SecDialog::on_ok_button_clicked()
{
    char tag[4]; //the 3 character (byte) portion of the mp3 file
    char songTitle[31]; //30 characters (bytes)
    char artist[31]; //30 characters (bytes)
    char album[31]; //30 characters (bytes)
    char year[5]; //4 characters (bytes)
    char comment[29]; //28 characters (bytes)

    //variable declaration (binary portion of file)
    char zero; //this is always 0
    char track; //track number for mp3 file
    char genre; //number telling which genre the song belongs to

    //variable declaration (string)
    string mp3Name = sPath.toStdString(); //name of mp3 file
    qDebug() << sPath << endl;
    //variable declaration (read input from a file)
    ifstream mp3In; //read information from mp3 file

    //Open the file, converting the mp3 file name to a c string
    mp3In.open(mp3Name.c_str(), ios::binary);

    //Check to make sure that the file can be opened
    //If not, display an ERROR message and end the program
    if (!mp3In.is_open())
    {
        qDebug() << "ERROR! Couldn't open mp3 file.Try Again" << endl;
    }
    else
    {
        //set ifstream pointer to the beginning of the last 128 bytes in the file
        mp3In.seekg(-128, ios::end);

        //read in the bytes and store them in appropriate variables
        //At the end of each unformatted variable (or variable without a \0), add a terminating character to parse the
        //byte sequence
        mp3In.read(tag, 3);
        tag[3] = '\0';
        mp3In.read(songTitle, 30);
        songTitle[30] = '\0';
        mp3In.read(artist, 30);
        artist[30] = '\0';
        mp3In.read(album, 30);
        album[30] = '\0';
        mp3In.read(year, 4);
        year[4] = '\0';
        mp3In.read(comment, 28);
        comment[28] = '\0';
        mp3In >> zero;
        mp3In >> track;
        mp3In >> genre;

        //display current ID3v1.1 information to the user
        qDebug() << "Tag: " << tag << endl
        << "Song Title: " << songTitle << endl
        << "Artist: " << artist << endl
        << "Album: " << album << endl
        << "Year: " << year << endl
        << "Comment: " << comment << endl
        << "Zero byte: " << (int)zero << endl
        << "Track byte: " << (int)track << endl
        << "Genre byte: " << (int)genre << endl;
    }
    mp3In.close();
    int xnum = check_artist(artist).toInt(0);
    QSqlQuery query;
    QString queryString = QString("Select count(*) from songs WHERE Song_name ='%1' AND Artist =%2;").arg(songTitle).arg(xnum);
    query.exec(queryString);
    query.first();
    if(query.value(0).toInt(0) == 0)
    {
        queryString = "INSERT INTO songs(Song_Name ,Artist ,Album ,Address ,Genre ,Year ) VALUES (:first,:second,:third,:forth,:fifth,:sixth);";
        query.prepare(queryString);
        query.bindValue(":first",songTitle);
        query.bindValue(":second",check_artist(artist).toInt(0));
        query.bindValue(":third",check_album(album).toInt(0));
        query.bindValue(":forth",sPath);
        query.bindValue(":fifth",(int)genre);
        query.bindValue(":sixth",(int)year);
        query.exec();
        ui->label->setText("Record inserted");
    }
    else
    {
        messageBox.critical(0,"Error","Record already exists or it's missing data!");
        messageBox.setFixedSize(500,200);
    }
}
/* checks if artist is present in database
 * if yes returns his id
 * if not returns his just added id */
QVariant SecDialog::check_artist(char artist[31])
{
    QSqlQuery query;
    QString queryString = QString("Select count(*) from Artist where Artist_Name='%1'").arg(artist);
    query.exec(queryString);
    query.first();
    if(query.value(0).toInt(0) == 0)
    {
        query.exec(QString("INSERT INTO artist(Artist_Name) VALUES ('%1');").arg(artist));
        return query.lastInsertId();
    }
    else
    {
        query.exec(QString("SELECT artist_id FROM artist WHERE Artist_Name = ('%1');").arg(artist));
        query.first();
        return query.value(0);
    }
}
/* checks if album is present in database
 * if yes returns his id
 * if not returns his just added id */
QVariant SecDialog::check_album(char album[31])
{
    QSqlQuery query;
    QString queryString = QString("Select count(*) from Album where Album_Name='%1'").arg(album);
    query.exec(queryString);
    query.first();
    if(query.value(0).toInt(0) == 0)
    {
        query.exec(QString("INSERT INTO album(Album_Name) VALUES ('%1');").arg(album));
        return query.lastInsertId();
    }
    else
    {
        query.exec(QString("SELECT album_id FROM album WHERE Album_Name = ('%1');").arg(album));
        query.first();
        return query.value(0);
    }
}
