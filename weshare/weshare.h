#ifndef WESHARE_H
#define WESHARE_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QFile>
#include <QProcess>

namespace Ui {class WeShare;}

class WeShare : public QMainWindow{
    Q_OBJECT

public:
    explicit WeShare(QWidget *parent = nullptr);
    ~WeShare();

private slots:
    void on_share_clicked();
    void on_browseFile_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::WeShare *ui;
    //QProcess *proses;
};

#endif // WESHARE_H
