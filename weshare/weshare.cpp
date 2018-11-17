#include "weshare.h"
#include "ui_weshare.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QtWidgets>
#include <QMessageBox>
#define MAX 54

static QProcess *proses = new QProcess;
struct DATA{
    QString selectedFile;
    QString webView[MAX]={
        "<html>",
        "<head>",
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">",
        "<title>WeShare</title>",
        "<style>",
        "html{",
        "background: linear-gradient(to right,#74b9ff, #00cec9);",
        "color: #ffffff;"
        "text-align: center;}",
        "#warp{",
        "margin: 5%;",
        "width: auto;",
        "height: auto;",
        "padding: 10px;}",
        ".filelist{",
        "background-color: #ffffff;",
        "border-bottom:5px solid #0984e3;",
        "padding: 1%;",
        "color: #0984e3;}",
        "a{",
        "float: right;",
        "margin:1%;",
        "text-decoration: none;",
        "font-size: 25px;",
        "border: 2px solid #0984e3;",
        "border-radius: 50px;",
        "height: 30px;",
        "width: 30px;",
        "padding: 10px;",
        "color: #0984e3;",
        "font-weight: bolder;",
        "transition: 300ms;}",
        "h1{",
        "background-color:#0984e3;",
        "border-radius:10px;",
        "width:auto;",
        "height:auto;",
        "padding:10px;",
        "border-bottom:5px solid #b2bec3;}",
        "a:hover{",
        "background-color:#0984e3;",
        "color:#ffffff;",
        "width: 80px;",
        "transition: 300ms;}",
        "p{font-size:20px;font-weight:bold;margin-right:1.5%;margin-left:2%;}",
        "</style>",
        "</head>",
        "<body>",
        "<div id=\"warp\">",
        "<h1>We want to share something for you</h1>",
        "<!--LINE-->",
        "</div>",
        "</body>",
        "</html>"
    };
};

QString strrev(QString txt){
    QString temp;
    int rev=0;
    for(int scan=txt.length();scan>0;scan--&&rev++)
        temp[rev]=txt[scan];
    return temp;
}
QString ftext(QString txt){
    QString filter;
    for(int scan=txt.length();scan>0;scan--){
        if(txt[scan]=="/" || txt[scan]=="\\") break;
        else filter+=txt[scan];
    }
    return strrev(filter);
}
QString runicode(QString txt){
    QString fix;
    int fix_range=0;
    for(int x=1;x<txt.length()&&fix_range<txt.length();x++&&fix_range++)
        fix[fix_range]=txt[x];
    return fix;
}

WeShare::WeShare(QWidget *parent):QMainWindow(parent),ui(new Ui::WeShare){
    ui->setupUi(this);
    ui->share->setStyleSheet(
        "QPushButton#share{"
        "color: #ffffff;"
        "background-color: #2c3e50;"
        "border: 2px solid #2980b9;"
        "padding: 10px;"
        "font-size: 20px;"
        "border-radius: 25px;}"
        "QPushButton#share:hover{"
        "background-color:#2980b9;"
        "color:#2c3e50;}"
        "QPushButton#share:pressed{"
        "font-size: 15px;}"
    );
    ui->browseFile->setStyleSheet(
        "QPushButton#browseFile{"
        "color: #ffffff;"
        "background-color: #2c3e50;"
        "border: 2px solid #2980b9;"
        "padding: 10px;"
        "font-size: 20px;"
        "border-radius: 25px;}"
        "QPushButton#browseFile:hover{"
        "background-color:#2980b9;"
        "color:#2c3e50;}"
        "QPushButton#browseFile:pressed{"
        "font-size: 15px;}"
    );
}
WeShare::~WeShare(){delete ui;}

void WeShare::on_share_clicked(){
    QString ip,read;
    QFile indexopen("index.html");
    QStringList arg;
    DATA Data;

    foreach(const QHostAddress &addr,QNetworkInterface::allAddresses())
        if(addr.protocol()==QAbstractSocket::IPv4Protocol && addr!=QHostAddress(QHostAddress::LocalHost))
            ip=addr.toString();

    if(ip.isEmpty())
        ip="localhost";
    ui->ipaddr->setText(ip.append(":8080"));
    if(indexopen.open(QIODevice::WriteOnly)){
        QTextStream write(&indexopen);
        QString flname;

        for(int line=0;line<MAX;line++){
            if(Data.webView[line]=="<!--LINE-->"){
                for(int scan=0;scan<(ui->fileList->count());scan++){
                    flname=runicode(ftext(ui->fileList->item(scan)->text()));
                    write<<"<div class=\"filelist\">\n";
                    write<<"<a href=\""<<flname<<"\">&darr;</a>\n";
                    write<<"<p>"<<flname<<"</p>\n";
                    write<<"</div>\n";
                }
            }else
                write<<Data.webView[line]<<"\n";
        }
        indexopen.close();
    }
    arg<<"-S"<<ip+":8080";
    proses->start("php",arg);
}

void WeShare::on_browseFile_clicked(){
    QString file=QFileDialog::getOpenFileName(this);
    DATA data;

    ui->fileList->addItem(data.selectedFile=file);
}

void WeShare::closeEvent(QCloseEvent *event){
    QMessageBox ask;
    QFile file;

    ask.setWindowTitle("WeShare");
    ask.setText("Are you sure?\nyou will also close your shared file(s)");
    ask.addButton(QMessageBox::Yes)->setStyleSheet(
        "border:2px solid #2980b9;"
        "border-radius:15px;"
        "width:100px;"
        "margin-right:50px;"
    );
    ask.addButton(QMessageBox::No)->setStyleSheet(
        "border:2px solid #2980b9;"
        "border-radius:15px;"
        "width:100px;"
    );
    ask.setStyleSheet(
        "color:#ffffff;"
        "background-color:#2c3e50;"
        "padding:5px;"
    );

    if(ask.exec() != QMessageBox::No){
        proses->kill();
        event->accept();
        file.remove("index.html");
    }else
        event->ignore();
}
