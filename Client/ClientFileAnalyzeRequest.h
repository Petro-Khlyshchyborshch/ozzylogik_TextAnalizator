#ifndef CLIENTFILEANALYZEREQUEST_H
#define CLIENTFILEANALYZEREQUEST_H

#include <QObject>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "AnalysisResult.h"

class ClientFileAnalyzeRequest : public QObject
{
    Q_OBJECT

public:
    ClientFileAnalyzeRequest(const QString &filePath, const QUrl &url);
    void start();

private slots:
    void onFinished();

private:
    QString filePath;
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    AnalysisResult parseJsonResponse(const QByteArray &response);
};

#endif // CLIENTFILEANALYZEREQUEST_H
