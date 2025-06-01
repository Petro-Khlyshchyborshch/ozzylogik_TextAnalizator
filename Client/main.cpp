#include <QCoreApplication>
#include <QByteArray>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

#include <QCoreApplication>
#include <QTextStream>
#include "ClientFileAnalyzeRequest.h"
#include "Validator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString serverAddress;
    QString filePath;

    QTextStream stdinStream(stdin);


    serverAddress = "http://localhost:8080/analyze";
    //serverAddress = stdinStream.readLine();
    if (!validateServerAddress(serverAddress))
    {
        return 1;
    }

    qDebug() << "Enter file path:";
    stdinStream>>filePath;
    if (!validateFilePath(filePath))
    {
        return 1;
    }

    QUrl url(serverAddress);
    ClientFileAnalyzeRequest worker(filePath, url);
    worker.start();

    return a.exec();
}

