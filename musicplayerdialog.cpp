#include "musicplayerdialog.h"
#include "ui_musicplayerdialog.h"
#include "mainwindow.h"

/* Constructor
 * Creates the dialog window and connects the sliders to functions */
MusicPlayerDialog::MusicPlayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicPlayerDialog)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);

    connect(player, &QMediaPlayer::positionChanged, this, &MusicPlayerDialog::on_positonChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicPlayerDialog::on_durationChanged);
}

MusicPlayerDialog::~MusicPlayerDialog()
{
    delete ui;
}
/* acquires the address from table view */
void MusicPlayerDialog::get_string_to_play(QString passed)
{
    selected = passed;
}
/* player sets signals to the possition slider on current progress */
void MusicPlayerDialog::on_Progress_Slider_sliderMoved(int position)
{
    player->setPosition(position);
}
/* player sets sounds */
void MusicPlayerDialog::on_Volume_Slider_sliderMoved(int position)
{
    player->setVolume(position);
}
/* when clicked player if not playing sets media and starts playing */
void MusicPlayerDialog::on_Play_Button_clicked()
{
    if(player->state() != QMediaPlayer::PausedState && player->state() != QMediaPlayer::PlayingState)
    {
        player->setMedia(QUrl(selected, QUrl::TolerantMode));
    }
    player->play();
    qDebug() << player->errorString();
}
/* when clicked player paused */
void MusicPlayerDialog::on_Pause_Button_clicked()
{
    player->pause();
}
/* when clicked player stoped */
void MusicPlayerDialog::on_Stop_Button_clicked()
{
    player->stop();
}
/* sets the durration of the song on the progress bar */
void MusicPlayerDialog::on_durationChanged(qint64 position)
{
    ui->Progress_Slider->setMaximum(position);
}
/* updates the progress bar while playing */
void MusicPlayerDialog::on_positonChanged(qint64 position)
{
    ui->Progress_Slider->setValue(position);
}
