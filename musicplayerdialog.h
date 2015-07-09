#ifndef MUSICPLAYERDIALOG_H
#define MUSICPLAYERDIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QDebug>

namespace Ui {
class MusicPlayerDialog;
}

class MusicPlayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MusicPlayerDialog(QWidget *parent = 0);
    ~MusicPlayerDialog();
    void get_string_to_play(QString passed);

private slots:
    void on_Progress_Slider_sliderMoved(int position);

    void on_Volume_Slider_sliderMoved(int position);

    void on_Play_Button_clicked();

    void on_Pause_Button_clicked();

    void on_Stop_Button_clicked();

    void on_durationChanged(qint64 position);

    void on_positonChanged(qint64 position);

private:
    Ui::MusicPlayerDialog *ui;
    QMediaPlayer *player;
    QString selected;
};

#endif // MUSICPLAYERDIALOG_H
