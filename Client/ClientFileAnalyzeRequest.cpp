#include "ClientFileAnalyzeRequest.h"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <QFile>

ClientFileAnalyzeRequest::ClientFileAnalyzeRequest(const QString &filePath, const QUrl &url)
    : filePath(filePath), url(url), manager(new QNetworkAccessManager(this)) {}

void ClientFileAnalyzeRequest::start()
{
    QFile file(filePath);
    if (!file.exists())
    {
        qDebug() << "Error" << filePath;
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Не вдалося відкрити файл:" << filePath;
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

    reply = manager->post(request, fileData);
    connect(reply, &QNetworkReply::finished, this, &ClientFileAnalyzeRequest::onFinished);
}

void ClientFileAnalyzeRequest::onFinished()
{
    if (reply->error())
    {
        qDebug() << "Error" << reply->errorString();
    } else
    {
        qDebug() << "Answer received:";
        QByteArray responseData = reply->readAll();
        //qDebug() << responseData;

        // Парсинг JSON
        AnalysisResult result = parseJsonResponse(responseData);
        qDebug() << "Total words:" << result.total_words;
        qDebug() << "Unique words:" << result.unique_words;
        qDebug() << "Longest sequence length:" << result.longest_sequence_length;
        qDebug() << "Longest sequence:" << QString::fromStdString(result.longest_sequence);
    }
    reply->deleteLater();
    manager->deleteLater();
    QCoreApplication::quit();
}

AnalysisResult ClientFileAnalyzeRequest::parseJsonResponse(const QByteArray &response)
{
    AnalysisResult result;

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        result.total_words = obj["total_words"].toInt();
        result.unique_words = obj["unique_words"].toInt();
        result.longest_sequence_length = obj["longest_sequence_length"].toInt();
        result.longest_sequence = obj["longest_sequence"].toString().toStdString();
    }
    return result;
}
