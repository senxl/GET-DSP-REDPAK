#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QNetworkAccessManager>
#include <regex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *label1;

    QNetworkRequest reqest_token;
    QNetworkRequest reqest_redpak;
    QNetworkRequest reqest_finish;
    QNetworkReply *reply;
    QNetworkAccessManager *network_manager_getToken;
    QNetworkAccessManager *network_manager_getRedpak;
    QNetworkAccessManager *network_manager_finish;

    std::string my_id;
    std::string link_base;
    std::string link_geturl;
    std::string link_getRedPak;
    std::string link_finishUrl;

    int retry = 0;

    int st_h=22;  //时
    int st_min=00; //分

    int late = 2;
    int showpr = 1;


    void getMyToken();
    void getMyRedpak();
    void getFinish();

    void windowInit();
    void getInit();

    void Sleep(int msec);
};
#endif // MAINWINDOW_H
