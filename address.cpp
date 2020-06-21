#include "address.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

Address::Address(QString new_address)
{
    QStringList maskAndIp = new_address.split('/');
    if(maskAndIp.size()<2)
    {
        QString a="Podano tylko Stringa";
        throw a;
    }
    int sizeOfMask=maskAndIp[1].size();
    if(sizeOfMask<=2)
    {
        maskAndIp[1]=changeMaskToDecimal(maskAndIp[1]);
    }


    QStringList splitIP= maskAndIp[0].split('.');
    QStringList splitMask= maskAndIp[1].split('.');
    if(checkInputAddress(splitIP,0) && checkInputAddress(splitMask,1))
    {
        this->addressIp=splitIP;
        this->addressMask=splitMask;
        QStringList binIp=this->binaryAddress(splitIP);
        QStringList binMask=this->binaryAddress(splitMask);
        this->addressIpBin=binIp;
        this->addressMaskBin=binMask;
        maskOne=maskOneCount();
    }
    else
    {
        QString wyjatek = "Błąd-> string w adresie!";
        throw wyjatek;
    }

}

void Address::netAddressSearch()
{

    for(int i=0;i<4;i++)
    {
        QString oktet;
        for(int j=0;j<8;j++)
        {
            if(addressMaskBin[i][j]=='1')
            {
                oktet.append(addressIpBin[i][j]);
            }
            else
            {
                oktet.append('0');
            }

        }
        netAddressBin+=oktet;
    }

    netAddress=this->decimalAddress(netAddressBin);
}

void Address::broadcastAddressSearch()
{
    for(int i=0;i<4;i++)
    {
        QString oktet;
        for(int j=0;j<8;j++)
        {
            if(addressMaskBin[i][j]=='1')
            {
                oktet.append(addressIpBin[i][j]);
            }
            else
            {
                oktet.append('1');
            }

        }
        broadcastAddressBin+=oktet;
    }
    broadcastAddress=this->decimalAddress(broadcastAddressBin);
}

void Address::classAddressSearch()
{
    QString kwartet;
    for(int i=0;i<4;i++)
    {
        kwartet+=addressIpBin[0][i];
    }
    if(kwartet[0]=='0')
    {
        classAddress="A";
    }
    else if(kwartet[0]=='1' && kwartet[1]=='0')
    {
        classAddress='B';
    }
    else if(kwartet[0]=='1' && kwartet[1]=='1' && kwartet[2]=='0')
    {
        classAddress='C';
    }
    else if(kwartet[0]=='1'&& kwartet[1]=='1'&& kwartet[2]=='1'&&kwartet[3]=='1')
    {
        classAddress='E';
    }
    else {
        classAddress='D';
    }
}

void Address::publicOrPrivateAddress()
{
    if(classAddress=="A")
    {
        if(addressIp[0]=="10" && maskOne==8 )
        {
            isPrivate=true;
        }
        else {
            isPrivate=false;
        }

    }
    else if(classAddress=="B" && maskOne==12)
    {
        if(addressIp[0]=="172" && addressIp[1].toInt()>=16 && addressIp[1].toInt()<=31)
        {
            isPrivate=true;
        }
        else {
            isPrivate=false;
        }
    }
    else if(classAddress=="C" && maskOne==16)
    {
        if(addressIp[0]=="192" && addressIp[1]=="168")
        {
            isPrivate=true;
        }
        else {
            isPrivate=false;
        }

    }
    else
    {
       isPrivate=false;
    }
}

void Address::howManyHosts()
{
    hosts=pow(2,(32-maskOne))-2;
}

void Address::lastAndFirstHost()
{

    for(int i=0;i<4;i++)
    {
        if(i==3)
        {
            firstHost.append(QString::number(netAddress[i].toInt()+1));
        }
        else
        {
            firstHost.append(netAddress[i]);
        }
    }


    for(int i=0;i<4;i++)
    {
        if(i==3)
        {
            lastHost.append(QString::number(broadcastAddress[i].toInt()-1));
        }
        else
        {
            lastHost.append(broadcastAddress[i]);
        }
    }
    firstHostBin=this->binaryAddress(firstHost);
    lastHostBin=this->binaryAddress(lastHost);
}

QString Address::printAll()
{
     string linia;
     fstream plik;


    QString returnMessage;
    returnMessage.append("Podsumowanie: \n");
    returnMessage.append("Adres sieci  dec-> "+changeToString(netAddress)+"\n");
    returnMessage.append("Adres sieci bin-> "+changeToString(netAddressBin)+"\n");
    returnMessage.append("Klasa sieci-> "+classAddress+"\n");
    if(isPrivate)
    {
        returnMessage.append("Adres sieci jest prywatny \n");
    }
    else
    {
        returnMessage.append("Adres sieci jest publiczny \n");
    }
    returnMessage.append("Maska sieci  dec-> "+changeToString(addressMask)+"\n");
    returnMessage.append("Maska sieci  bin-> "+changeToString(addressMaskBin)+"\n");
    returnMessage.append("Broadcast adres  dec-> "+changeToString(broadcastAddress)+"\n");
    returnMessage.append("Broadcast adres  bin-> "+changeToString(broadcastAddressBin)+"\n");
    returnMessage.append("Adres pierwszego hosta dec-> "+changeToString(firstHost)+"\n");
    returnMessage.append("Adres pierwszego hosta bin-> "+changeToString(firstHostBin)+"\n");
    returnMessage.append("Adres ostatniego hosta dec-> "+changeToString(lastHost)+"\n");
    returnMessage.append("Adres ostatniego hosta bin-> "+changeToString(lastHostBin)+"\n");
    returnMessage.append("Liczba wszystkich hostów-> "+QString::number(hosts)+"\n");

    plik.open("plik.txt", ios::out | ios::trunc);
    if(plik.good() == true)
    {
        plik << returnMessage.toStdString();
        plik.close();
    }

    return returnMessage;
}

void Address::doAll()
{
    this->netAddressSearch();
    this->broadcastAddressSearch();
    this->publicOrPrivateAddress();
    this->classAddressSearch();
    this->lastAndFirstHost();
    this->howManyHosts();

}

int Address::maskOneCount()
{
    int newMaskOne=0;
    foreach(QString val,addressMaskBin)
    {
        foreach(QString c,val)
        {
            if(c.toInt()==1)
            {
                newMaskOne++;
            }
        }
    }
    return newMaskOne;
}

bool Address::checkInputAddress(QStringList checkingAddress, int whichPart)
{
    QRegExp re("\\d*");
    if(whichPart == 0)
    {
        if(checkingAddress.size()==4)
        {
            foreach(QString val,checkingAddress)
            {
                if (re.exactMatch(val))
                {
                    if(val=="")
                    {
                        QMessageBox mess;
                        mess.setText("Pusty string w adresie!");
                        mess.exec();
                        return false;
                    }

                    if(val.size()>=2 && val[0]=='0' && val[1]!='0')
                    {
                        QMessageBox mess;
                        mess.setText("Podano błędnę Ip!");
                        mess.exec();
                        return false;
                    }

                    if(val.toInt()>255 || val.toInt()<0)
                    {
                        QMessageBox mess;
                        mess.setText("Podano błędnę Ip!");
                        mess.exec();
                        return false;
                    }
                }
                else {
                    QMessageBox mess;
                    mess.setText("Podano błędnę Ip!");
                    mess.exec();
                    return false;
                }
            }

        }
        else
        {
            QString wyjatek = "Błąd-> maska max 32bit!";
            throw wyjatek;
        }
    }
    else
    {
        if(checkingAddress.size()==4)
        {
            foreach(QString val,checkingAddress)
            {
                if (re.exactMatch(val))
                {
                    if(val=="")
                    {
                        QMessageBox mess;
                        mess.setText("Pusty string w adresie!");
                        mess.exec();
                        return false;
                    }


                   if(val.size()>=2 && val[0]=='0' && val[1]!='0')
                   {
                       QMessageBox mess;
                       mess.setText("Podano błędną maskę!");
                       mess.exec();
                       return false;
                   }

                    if(val.toInt()>255 || val.toInt()<0)
                    {
                        QMessageBox mess;
                        mess.setText("Podano błędną maskę!");
                        mess.exec();
                        return false;
                    }
                }
                else
                {
                    QMessageBox mess;
                    mess.setText("Podano błędną maskę!");
                    mess.exec();
                    return false;
                }

            }

        }
        else
        {
            QString wyjatek = "Błąd-> maska max 32bit!";
            throw wyjatek;
        }
    }
    return true;
}

QString Address::changeMaskToDecimal(QString oldMask)
{
    QStringList newMaskBin;
    int size=oldMask.toInt();

    if(size>32)
    {
        QString wyjatek = "Błąd-> maska za duża!";
        throw wyjatek;
    }

    int licznik=0;
    for(int i=0;i<4;i++)
    {
        QString newOktet;
        for(int j=0;j<8;j++)
        {
            if(licznik<size)
            {
                newOktet.append("1");
            }
            else
            {
                newOktet.append("0");
            }
            licznik++;
        }

        newMaskBin.append(newOktet);
    }


    QStringList newMaskDec=decimalAddress(newMaskBin);
    oldMask=changeToString(newMaskDec);
    return oldMask;
}

bool Address::isHostAddress()
{
    int netString=0;

    foreach(QString val,netAddress)
    {
        netString+=val.toInt();
    }


    int broadString=0;

    foreach(QString val,broadcastAddress)
    {
        broadString+=val.toInt();
    }

    int addressString=0;

    foreach(QString val,addressIp)
    {
        addressString+=val.toInt();
    }

    if(netString<addressString && addressString<broadString)
    {
        addressToPing=changeToString(addressIp);
        return true;
    }
    else {
        return false;
    }
}

QString Address::getAddressToPing() const
{
    return addressToPing;
}

QString Address::changeToString(QStringList listToChange)
{
    QString newString;
    for(int i=0;i<listToChange.size();i++)
    {
        if(i==3)
        {
            newString.append(listToChange.at(i));
        }
        else
        {
          newString.append(listToChange.at(i)+".");
        }

    }

    return newString;
}

QStringList Address::binaryAddress(QStringList decimalAddress)
{
    QStringList newBinaryAddress;
    foreach(QString val, decimalAddress)
    {

        int podzial=val.toInt();
        QString oktet;
        while(podzial>0)
        {
            int reszta=podzial%2;
            oktet.push_front(QString::number(reszta));
            podzial=podzial/2;
        }

        if(oktet.size()==8)
        {
            newBinaryAddress+=oktet;
        }
        else
        {
            int fistSize=oktet.size();
            for(int i=0;i<8-fistSize;i++)
            {
                oktet.push_front("0");
            }
            newBinaryAddress+=oktet;
        }
    }
    return newBinaryAddress;
}

QStringList Address::decimalAddress(QStringList binaryAddress)
{
    QStringList newBinaryAddress;
    foreach(QString val, binaryAddress)
    {
      int oktet=0;
      int potega=7;
      foreach(QString v,val)
      {
        oktet+=v.toInt()*pow(2,potega);
        potega--;
      }
      newBinaryAddress+=QString::number(oktet);
    }

    return newBinaryAddress;
}

