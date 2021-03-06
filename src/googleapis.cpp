#include "googleapis.h"
#include <QtNetwork/QtNetwork>
#include <QJsonDocument>
#include <QMessageBox>
#include <QUrlQuery>

QDebug operator<<(QDebug dbg, const OCRBox &box)
{
    dbg << "OCRBox(" << box.description << "," << box.vertices << ")";
    return dbg;
}

QString getKey()
{
    QFile file(":/txt/key");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    QString line = in.readLine();

    file.close();

    return line;
}


OCRResult doOCR(const QPixmap& pixmap)
{
    static QString key = getKey();

    QNetworkAccessManager manager;
    QEventLoop loop;

    QUrl url("https://vision.googleapis.com/v1/images:annotate?key=" + key);
    QNetworkRequest request(url);

    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    QJsonDocument json(QJsonObject{
                           {"requests", QJsonArray{
                                QJsonObject{
                                    {"image", QJsonObject{
                                         {"content", QString(bArray.toBase64())}
                                     }},
                                    {"features", QJsonArray{ QJsonObject{
                                         {"type", "TEXT_DETECTION"},
                                         {"model", "builtin/stable"}
                                     }}}
                                }
                            }}
                       });

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager.post(request, json.toJson());
    QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));

    loop.exec();

    QJsonDocument result = QJsonDocument::fromJson(reply->readAll());

    OCRResult ocrResult;
    if(result.isEmpty())
    {
        qDebug() << "empty result";
    }
    else
    {
        auto obj = result.object();
        auto errorProcess = [](const QJsonObject& obj){
            auto error = obj.find("error");
            if(error != obj.end())
            {
                qDebug() << "error: " << error.value().toObject()["message"].toString();
                return false;
            }

            return true;
        };
        if(errorProcess(obj))
        {
            auto res = obj.find("responses");
            if(res != obj.end())
            {
                auto one = res.value().toArray()[0].toObject();
                if(errorProcess(one))
                {
                    for(auto&& box : one["textAnnotations"].toArray())
                    {
                        auto boxObj = box.toObject();
                        QVector<QPointF> vers;
                        for(auto&& ver : boxObj["boundingPoly"].toObject()["vertices"].toArray())
                        {
                            vers.push_back(QPointF(ver.toObject()["x"].toDouble(), ver.toObject()["y"].toDouble()));
                        }
                        ocrResult.push_back(OCRBox{boxObj["description"].toString(), vers});
                    }
                }
            }
        }
    }

    return ocrResult;
}


QVector<QString> getSupportedLanguages()
{
    static QString key = getKey();

    QNetworkAccessManager manager;
    QEventLoop loop;

    QUrl url("https://translation.googleapis.com/language/translate/v2/languages?key=" + key);
    QNetworkRequest request(url);

    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));

    loop.exec();

    QJsonDocument result = QJsonDocument::fromJson(reply->readAll());

    auto errorProcess = [](const QJsonObject& obj){
        auto error = obj.find("error");
        if(error != obj.end())
        {
            qDebug() << "error: " << error.value().toObject()["message"].toString();
            return false;
        }

        return true;
    };

    auto object = result.object();
    QVector<QString> res;
    if(errorProcess(object))
    {
        for(auto&& lang : object["data"].toObject()["languages"].toArray())
        {
            res.push_back(lang.toObject()["language"].toString());
        }
    }

    return res;
}

QVector<QString> doTranslate(const QVector<QString> &text, const QString &targetLang)
{
    static QString key = getKey();

    QNetworkAccessManager manager;
    QEventLoop loop;

    QUrl url("https://translation.googleapis.com/language/translate/v2?key=" + key);
    QNetworkRequest request(url);

    QUrlQuery query;
    for(auto&& t : text)
    {
        query.addQueryItem("q", t);
    }
    query.addQueryItem("target", targetLang);
    query.addQueryItem("format", "text");

    QNetworkReply *reply = manager.post(request, query.toString().toUtf8());
    QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));

    loop.exec();

    QJsonDocument result = QJsonDocument::fromJson(reply->readAll());

    auto errorProcess = [](const QJsonObject& obj){
        auto error = obj.find("error");
        if(error != obj.end())
        {
            qDebug() << "error: " << error.value().toObject()["message"].toString();
            return false;
        }

        return true;
    };

    auto object = result.object();
    QVector<QString> res;
    if(errorProcess(object))
    {
        for(auto&& translated : object["data"].toObject()["translations"].toArray())
        {
            res.push_back(translated.toObject()["translatedText"].toString());
        }
    }

    return res;
}

QMap<QString, QString> translateOCRResult(const OCRResult &ocr, const QString &targetLang)
{
    QMap<QString, QString> map;

    QVector<QString> text;
    bool isFirst = true;
    for(auto&& box : ocr)
    {
        if(isFirst)
        {
            isFirst = false;
            continue;
        }
        text.push_back(box.description);
    }

    QVector<QString> translated = doTranslate(text, targetLang);
    if(text.size() != translated.size())
    {
        qDebug() << "translateOCRResult: doTranslate failed";
        return map;
    }
    for(int i = 0; i < text.size(); ++i)
    {
        map.insert(text[i], translated[i]);
    }

    return map;
}
