#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "MQNTP.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void syncTime();

private:
    Ui::MainWindow *ui;
    QTimer* basic1s;
    QTimer* sitesTimer;
    QStandardItemModel* sitesModel;
    QItemSelectionModel* selection;
    std::vector<std::shared_ptr<MQTimeGetter>> sitesGetter;

private slots:
    void basic1sTimeout();
    void refleshInfo();
    void sitesTimerTimeout();
};
#endif // MAINWINDOW_H
