#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QNetworkReply>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    windowInit();

}

void MainWindow::getMyToken()
{
    if (reply->NoError == QNetworkReply::NoError) {
        QByteArray resultContent = reply->readAll();
        QString reqtext(resultContent);

        if(reply->rawHeaderList().length() > 5)
        {
            qDebug() << "当前header大于5条";
            retry = 0;
            if(reply->rawHeader(QByteArray("Set-Cookie")).length() > 0)
            {
                qDebug() << "更新cookie";
                reqest_redpak.setRawHeader("Cookie",reply->rawHeader(QByteArray("Set-Cookie")));
            }

            QRegExp rx(tr("var token = \"(.{40})\";"));
            if (rx.indexIn(reqtext) != -1)
            {
                qDebug() << QString::fromStdString((link_getRedPak + rx.capturedTexts().at(1).toStdString()));
                reqest_redpak.setUrl(QUrl((link_getRedPak + rx.capturedTexts().at(1).toStdString()).c_str()));
                reply = network_manager_getRedpak->get(reqest_redpak);
            }
        }else {
            qDebug() << "当前header不足5条";
            if(retry<3)
            {
                reply = network_manager_getToken->get(reqest_token);
                retry++;
            }else {
                qDebug() << "无法获取Cookie，访问失败！";
            }
        }
    }
}

void MainWindow::getMyRedpak()
{
    if (reply->NoError == QNetworkReply::NoError) {
        QByteArray resultContent = reply->readAll();


        if(QString(resultContent).contains("活动未开始"))
        {
            QTime t;
            int h=t.currentTime().hour();
            int min=t.currentTime().minute();
            int sec=t.currentTime().second();
            if(h == st_h && min == st_min && (sec >= (0 + late)) && sec <= (2 + late))
            {
                ui->textBrowser->append("活动可能未开始，再次尝试...");
                getInit();
                reply = network_manager_getToken->get(reqest_token);
            }else {
                ui->textBrowser->append("时间已经过了！");
            }
        }

        ui->textBrowser->append( QString(resultContent));
        reply = network_manager_finish->get(reqest_finish);

    }
}


void MainWindow::getFinish()
{
    if (reply->NoError == QNetworkReply::NoError) {
        QByteArray resultContent = reply->readAll();

        ui->textBrowser->append( QString(resultContent));
    }
}

void MainWindow::getInit()
{
    network_manager_getToken = new QNetworkAccessManager();
    network_manager_getRedpak = new QNetworkAccessManager();
    network_manager_finish = new QNetworkAccessManager();

    delete network_manager_getToken;
    network_manager_getToken = new QNetworkAccessManager();
    delete network_manager_getRedpak;
    network_manager_getRedpak = new QNetworkAccessManager();
    delete network_manager_finish;
    network_manager_finish = new QNetworkAccessManager();

    connect(network_manager_getToken, &QNetworkAccessManager::finished, this, &MainWindow::getMyToken);
    connect(network_manager_getRedpak, &QNetworkAccessManager::finished, this, &MainWindow::getMyRedpak);
    connect(network_manager_finish, &QNetworkAccessManager::finished, this, &MainWindow::getFinish);

    my_id = ui->lineEdit->text().toStdString();

    link_base = "http://xy.disubang.com";
    link_geturl = link_base + "/mobile/Rob/underway/activity_id/59/user_id/" + my_id + ".html";
    link_getRedPak = link_base + "/mobile/Rob/prize.html?user_id=" + my_id + "&activity_id=59&__token__=";
    link_finishUrl = link_base + "/mobile/Rob/finished/activity_id/59/user_id/" + my_id + ".html";

    reqest_token = QNetworkRequest(QUrl(link_geturl.c_str()));
    reqest_token.setRawHeader("Host", "xy.disubang.com");
    reqest_token.setRawHeader("Connection", "keep-alive");
    reqest_token.setRawHeader("User-Agent", "Mozilla/5.0 (Linux; Android 9; MI 6 Build/PKQ1.190118.001; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/77.0.3865.120 MQQBrowser/6.2 TBS/045525 Mobile Safari/537.36 ");

    reqest_redpak = QNetworkRequest();
    reqest_redpak.setRawHeader("Host", "xy.disubang.com");
    reqest_redpak.setRawHeader("Connection", "keep-alive");
    reqest_redpak.setRawHeader("User-Agent", "Mozilla/5.0 (Linux; Android 9; MI 6 Build/PKQ1.190118.001; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/77.0.3865.120 MQQBrowser/6.2 TBS/045525 Mobile Safari/537.36 ");

    reqest_finish = QNetworkRequest(QUrl(link_finishUrl.c_str()));
    reqest_finish.setRawHeader("Host", "xy.disubang.com");
    reqest_finish.setRawHeader("Connection", "keep-alive");
    reqest_finish.setRawHeader("User-Agent", "Mozilla/5.0 (Linux; Android 9; MI 6 Build/PKQ1.190118.001; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/77.0.3865.120 MQQBrowser/6.2 TBS/045525 Mobile Safari/537.36 ");

}


void MainWindow::windowInit()
{
    setWindowIcon(QIcon(":/android/ico/icon.png"));
    QTimer *tim = new QTimer();

    QTimer *tim_go = new QTimer();
    label1=new QLabel();
    connect(ui->pushButton,&QPushButton::clicked,ui->textBrowser,[=](){
        ui->textBrowser->append("程序已就绪...等待22点到来");
        ui->pushButton->hide();
        tim_go->start(50);
    });

    connect(tim_go,&QTimer::timeout,ui->statusbar,[=](){
        QTime t;
        int h=t.currentTime().hour();
        int min=t.currentTime().minute();
        int sec=t.currentTime().second();

        if(h == st_h && min == st_min && (sec >= (0)) && sec <= (late) && showpr)
        {
            showpr = 0;
            ui->textBrowser->append("正在努力抢红包！！！");
            for (int i=0; i<=100; i++) {
                Sleep(15);
                ui->progressBar->setValue(i);
            }
        }

        if(h == st_h && min == st_min && (sec >= (0 + late)) && sec <= (2 + late))
        {
            getInit();
            reply = network_manager_getToken->get(reqest_token);
            tim_go->stop();
        }

    });

    connect(tim,&QTimer::timeout,ui->statusbar,[=](){
        ui->statusbar->addWidget(label1);
        QString timeLabel="当前时间:";
        QDateTime Currtime=QDateTime::currentDateTime();
        QString CurrTimeStr=Currtime.toString("yyyy-MM-dd hh:mm:ss");
        timeLabel.append(CurrTimeStr);
        label1->setText(timeLabel);
    });
    tim->start(1000);
}

void MainWindow::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

