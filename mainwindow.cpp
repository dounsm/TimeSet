#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTimer>
#include "infoContent.h"

const std::vector<std::string> SITES = {
    "ntp.ntsc.ac.cn",
    "ntp.aliyun.com",
    "ntp.sjtu.edu.cn",
    "time.edu.cn"
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("时间校对工具");

    //1s Timer init
    this->basic1s = new QTimer(this);
    basic1s->stop();
    basic1s->setInterval(1000);
    connect(basic1s,SIGNAL(timeout()),this,SLOT(basic1sTimeout()));
    basic1s->start();

    //slots init
    connect(ui->btn_refleshInfo,SIGNAL(clicked()),this,SLOT(refleshInfo()));
    refleshInfo();

    //table init
    {
    sitesModel = new QStandardItemModel(SITES.size()+1,2,this);
    selection = new QItemSelectionModel(sitesModel);
    ui->NTPSites->setModel(sitesModel);
    ui->NTPSites->setSelectionModel(selection);
    ui->NTPSites->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->NTPSites->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->NTPSites->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //-->Header init
    QStringList headerList = {"主机","时间"};
    sitesModel->setHorizontalHeaderLabels(headerList);
    //-->Body init
    QStandardItem* item;
    item = new QStandardItem("本机");
    sitesModel->setItem(0,0,item);
    for(int i=0;i<SITES.size();i++){
        item = new QStandardItem(QString::fromStdString(SITES[i]));
        sitesModel->setItem(i+1,0,item);
    };
    };

    //Sites Timer init
    this->sitesTimer = new QTimer(this);
    sitesTimer->stop();
    sitesTimer->setInterval(5000);
    connect(sitesTimer,SIGNAL(timeout()),this,SLOT(sitesTimerTimeout()));
    syncTime();
    sitesTimer->start();

    //timegetter init
    for(int i=0;i<SITES.size();i++){
        sitesGetter.push_back(std::make_shared<MQTimeGetter>(new MQTimeGetter(this,
                                                                          QString::fromStdString(SITES[i]))));
    };


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refleshInfo(){
    //reflesh text
    ui->InfoText->clear();
    ui->InfoText->append(INFOCONTENT);
};

void MainWindow::syncTime(){

};

void MainWindow::basic1sTimeout(){



};

void MainWindow::sitesTimerTimeout(){
    syncTime();
};
