#include "Validator.h"
#include <QUrl>
#include <QFileInfo>
#include <QDebug>

// Перевірка та валідація адреси сервера
bool validateServerAddress(const QString &serverAddress)
{
    QUrl url(serverAddress);
    if (!url.isValid() || url.scheme() != "http" || url.host().isEmpty())
    {
        qDebug() << "Error Server Address" << serverAddress;
        return false;
    }
    return true;
}

// Перевірка та валідація шляху до файлу
bool validateFilePath(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile())
    {
        qDebug() << "Error File Path" << filePath;
        return false;
    }
    return true;
}
