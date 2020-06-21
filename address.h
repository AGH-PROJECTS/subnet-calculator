#ifndef ADDRESS_H
#define ADDRESS_H
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>

class Address
{
public:
    Address(QString new_address);
    void netAddressSearch();
    void broadcastAddressSearch();
    void classAddressSearch();
    void publicOrPrivateAddress();
    void howManyHosts();
    void lastAndFirstHost();
    QString printAll();
    void doAll(); //wykonanie wszystkich polecen
    bool isHostAddress(); //sprawdzenie czy jest adresm hosta dla pingu
    QString getAddressToPing() const; //pobranie adresu do pinogowania

private:
    QStringList addressIp;
    QStringList addressMask;
    QStringList addressIpBin;
    QStringList addressMaskBin;
    QStringList netAddressBin;
    QStringList broadcastAddressBin;
    QStringList netAddress;
    QStringList broadcastAddress;
    QStringList firstHostBin;
    QStringList firstHost;
    QStringList lastHostBin;
    QStringList lastHost;
    QString classAddress;
    QString addressToPing;
    bool isPrivate;
    int maskOne; //zamiana z 255.255.... na 24
    long hosts; //liczba hostow
    int maskOneCount(); //zamiana maski z 255.255... na 24
    bool checkInputAddress(QStringList checkingAddress, int whichPart); // sprawdzanie poprawnosc, whichPart ktora czesc adresu
    QString changeMaskToDecimal(QString oldMask); //zamiana maski z 24 na 255.255.255.255
    QString changeToString(QStringList listToChange); //zamiana kazdej listy na stringa 129.9.0.0
    QStringList binaryAddress(QStringList decimalAddress); //zamiana z decimal na binar
    QStringList decimalAddress(QStringList binaryAddress); //zamiana z binar na decimal
};

#endif // ADDRESS_H
