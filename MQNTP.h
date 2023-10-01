#ifndef MQNTP_H
#define MQNTP_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QUdpSocket>
#include <QDateTime>
#include <QString>
#include <QUrl>
#include <QHostInfo>


class MQTimeGetter:public QObject{
    Q_OBJECT
public:
    QDateTime time;
    QString serverIP;
    quint32 serverPort;
    QTimer* eventTimer;

    static const qint8 interval = 10;
private:
    QUdpSocket* udpSocket;
    QObject* parent;

    void __init__timer(){
        eventTimer = new QTimer();
        eventTimer->stop();
        eventTimer->setInterval(interval);
    };

    void __pre__construction(){
        __init__timer();
    };
public:
    MQTimeGetter():udpSocket(nullptr),parent(nullptr){
        __pre__construction();
    };
    MQTimeGetter(QObject* parent_,QString ip,quint32 port=123):
        parent(parent_),serverIP(ip),serverPort(port){
              __pre__construction();
    };
    ~MQTimeGetter(){
              delete eventTimer;
    };
    bool getTime(){
        udpSocket = new QUdpSocket(parent);

        //Lookup IPv4
        QHostInfo info = QHostInfo::fromName(serverIP);
        QString ipv4;
        for(QHostAddress addr : info.addresses()){
            if(addr.protocol()==QAbstractSocket::IPv4Protocol){
                ipv4 = addr.toString();
            };
        };

        connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        bool flag = false;
        quint32 tryport=10010;
        do{
            flag=udpSocket->bind(tryport++);
        }while(!flag);
        QHostAddress serverAddr(ipv4);
        QByteArray datagram;
        datagram.resize(48);
        datagram[0]=0x1B;
        for(quint32 i=1;i<48;i++){
            datagram[i]=0;
        };
        udpSocket->writeDatagram(datagram,serverAddr,serverPort);
        return true;
    };
private slots:
    void onReadyRead(){
        QByteArray datagram;
        QHostAddress addr;
        quint16 peerPort;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size(),&addr,&peerPort);
        quint64 offsetTransmitTime = 40,
            intpart = 0,
            fractpart = 0,
            milliseconds = 0;
        for (quint32 i = 0; i <= 3; i++) {
            intpart = 256 * intpart + quint8(datagram.at(offsetTransmitTime + i));
        };
        // Get the seconds fraction
        for (quint32 i = 4; i <= 7; i++) {
            fractpart = 256 * fractpart + quint8(datagram.at(offsetTransmitTime + i));
        };
        milliseconds = (intpart * 1000 + (fractpart * 1000) / 0x100000000)-quint64(2208988800000ULL);
        time.setMSecsSinceEpoch(milliseconds);
//        qDebug()<<"+"<<milliseconds;
//        qDebug()<<"+"<<1;
//        QDateTime now = QDateTime::fromMSecsSinceEpoch(0);
//        qDebug()<<now.toMSecsSinceEpoch();
//        qDebug()<<now.toLocalTime();
        delete udpSocket;
        udpSocket = nullptr;

        eventTimer->start();
    };
signals:
    void timeGet();
};



#endif // MQNTP_H
