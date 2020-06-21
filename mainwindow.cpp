#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include <QMessageBox>
#include <QHostInfo>
#include "address.h"
#include<cstdlib> // 0. stdlib instead of windows.h means it will compile on Linux too. :)

#include<iostream>

#include <QProcess>
#include<sstream>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_agree_clicked()
{
    ui->addressIP->setEnabled(false);
    ui->agree->setEnabled(false);

    //jesli adres jest pusty
    if(ui->addressIP->text()=="")
    {
        try
        {
            QString localHostName=QHostInfo::localHostName();
            QString localHostIP;
            QString localMacAddress;
            QString localNetmask;

            QList<QHostAddress> hostList = QHostInfo::fromName(localHostName).addresses(); //lista adresow powiazanych z hostname

            foreach(const QHostAddress& address,hostList)
            {
               if(address.protocol()==QAbstractSocket::IPv4Protocol && !address.isLoopback())
               {
                    localHostIP=address.toString();
               }
            }

            foreach(const QNetworkInterface& network,QNetworkInterface::allInterfaces())
            {
                foreach(const QNetworkAddressEntry& entry, network.addressEntries())
                {
                    if(entry.ip().toString()==localHostIP){
                        localNetmask= entry.netmask().toString();
                        localMacAddress = network.hardwareAddress();
                    }
                }

            }

            QString newLocalAddress=localHostIP+'/'+localNetmask;

            Address localAdr(newLocalAddress);
            localAdr.doAll();
            ui->textBrowser->setText(localAdr.printAll());
            if(localAdr.isHostAddress())
            {
                QPushButton *a=new QPushButton();
                a->setText("Ping?");
                ui->verticalLayout_2->addWidget(a);
                connect(a,&QPushButton::clicked,[=]{on_pingButton_clicked(localAdr.getAddressToPing());});
            }
            QPushButton *a=new QPushButton();
            a->setText("Od nowa?");
            ui->verticalLayout_2->addWidget(a);
            connect(a,SIGNAL(clicked()),this,SLOT(on_a_click()));
        }
        catch(QString a)
        {
            qDebug()<<a;
            QMessageBox mess;
            mess.setText("Wprowadzony adres zawiera błąd!\nSpróbuj ponownie!");
            mess.exec();
            this->close();
            this->show();
            ui->addressIP->setEnabled(true);
            ui->agree->setEnabled(true);
        }


    }
    else
    {
        QString newAddress=ui->addressIP->text();

        try{
            Address adr(newAddress);
            adr.doAll();
            ui->textBrowser->setText(adr.printAll());
            if(adr.isHostAddress())
            {
                QPushButton *a=new QPushButton();
                a->setText("Ping?");
                ui->verticalLayout_2->addWidget(a);
                connect(a,&QPushButton::clicked,[=]{on_pingButton_clicked(adr.getAddressToPing());});
            }
            QPushButton *a=new QPushButton();
            a->setText("Od nowa?");
            ui->verticalLayout_2->addWidget(a);
            connect(a,SIGNAL(clicked()),this,SLOT(on_a_click()));
        }
        catch(QString a)
        {
            qDebug()<<a;
            QMessageBox mess;
            mess.setText("Wprowadzony adres zawiera błąd!\nSpróbuj ponownie!");
            mess.exec();
            this->close();
            this->show();
            ui->addressIP->setEnabled(true);
            ui->agree->setEnabled(true);

        }

    }
    // jesli jest cos w nim

}

void MainWindow::on_pingButton_clicked(QString ipToPing)
{
    // wywolanie polecenia ping do hosta
    QString  command( "cmd.exe" );
    QStringList params = QStringList() << "/k" << "ping" <<ipToPing;
    QProcess::startDetached( command, params );

}

void MainWindow::on_a_click()
{
    //wywolanie polecenia od nowa
    ui->addressIP->setEnabled(true);
    ui->agree->setEnabled(true);
    ui->textBrowser->setText("");
    QLayoutItem *child;
    while ((child = ui->verticalLayout_2->takeAt(3)) != 0) {
        delete child->widget();
        delete child;
    }

}

