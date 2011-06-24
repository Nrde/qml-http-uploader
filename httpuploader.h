/* Copyright 2011 Jarek Pelczar <jpelczar@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HTTPUPLOADER_H
#define HTTPUPLOADER_H

#include <QtDeclarative>
#include <QtNetwork>

// Register the types below by calling, e.g.:
//
// qmlRegisterType<HttpPostFieldValue>("HttpUp", 1, 0, "HttpPostFieldValue");
// qmlRegisterType<HttpPostFieldFile>("HttpUp", 1, 0, "HttpPostFieldFile");
// qmlRegisterType<HttpUploader>("HttpUp", 1, 0, "HttpUploader");

class HttpPostField : public QObject
{
    Q_OBJECT
    Q_ENUMS(FieldType)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(FieldType type READ type CONSTANT FINAL)
    Q_PROPERTY(int contentLength READ contentLength NOTIFY contentLengthChanged)
public:
    enum FieldType {
        FieldInvalid,
        FieldValue,
        FieldFile
    };

    HttpPostField(QObject * parent = 0);
    virtual ~HttpPostField();

    QString name() const;
    void setName(const QString& name);

    HttpPostField::FieldType type() const;
    void setType(HttpPostField::FieldType type);

    virtual int contentLength() = 0;
    virtual QIODevice * createIoDevice(QObject * parent = 0) = 0;

signals:
    void nameChanged();
    void contentLengthChanged();

protected:
    FieldType mType;
    QString mName;
};

QML_DECLARE_TYPEINFO(HttpPostField, 0)

class HttpPostFieldValue : public HttpPostField
{
    Q_OBJECT
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
public:
    HttpPostFieldValue(QObject * parent = 0);

    QString value() const;
    void setValue(const QString& value);

    virtual int contentLength();
    virtual QIODevice * createIoDevice(QObject * parent = 0);

signals:
    void valueChanged();

private:
    QByteArray mValue;
};

QML_DECLARE_TYPEINFO(HttpPostFieldValue, 0)

class HttpPostFieldFile : public HttpPostField
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
public:
    HttpPostFieldFile(QObject * parent = 0);

    virtual int contentLength();
    virtual QIODevice * createIoDevice(QObject * parent = 0);

    QUrl source() const;
    void setSource(const QUrl& url);

    QString mimeType() const;
    void setMimeType(const QString& mime);

signals:
    void sourceChanged();
    void mimeTypeChanged();

private:
    QUrl mSource;
    QString mMime;
};

QML_DECLARE_TYPEINFO(HttpPostFieldFile, 0)

class HttpUploaderDevice;

class HttpUploader : public QDeclarativeItem
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QDeclarativeListProperty<HttpPostField> postFields READ postFields)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString errorString READ errorString)
    Q_PROPERTY(QString responseText READ responseText)
    Q_CLASSINFO("DefaultProperty", "postFields")
public:
    enum State {
        Unsent,
        Opened,       
        Loading,
        Aborting,
        Done
    };

    explicit HttpUploader(QDeclarativeItem *parent = 0);
    virtual ~HttpUploader();

    QUrl url() const;
    void setUrl(const QUrl& url);

    QDeclarativeListProperty<HttpPostField> postFields();
    qreal progress() const;
    HttpUploader::State state() const;
    QString errorString() const;
    QString responseText() const;
    int status() const;

public slots:
    void clear();
    void open(const QUrl& url);
    void send();
    void sendFile(const QString& fileName);
    void abort();
    void addField(const QString& fieldName, const QString& fieldValue);
    void addFile(const QString& fieldName, const QString& fileName, const QString& mimeType = QString());

signals:
    void urlChanged();
    void progressChanged();
    void stateChanged();
    void statusChanged();

private:
    static void AppendFunction(QDeclarativeListProperty<HttpPostField> *, HttpPostField*);
    static int CountFunction(QDeclarativeListProperty<HttpPostField> *);
    static HttpPostField * AtFunction(QDeclarativeListProperty<HttpPostField> *, int);
    static void ClearFunction(QDeclarativeListProperty<HttpPostField> *);

private slots:
    void reply_finished();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    QNetworkAccessManager * mNetworkAccessManager;
    QUrl mUrl;
    QList< QPointer<HttpPostField> > mPostFields;
    qreal mProgress;
    State mState;
    QNetworkReply * mPendingReply;
    QString mErrorString;
    QByteArray mBoundaryString;
    QIODevice * mUploadDevice;
    int mStatus;
    QByteArray mResponse;
    friend class HttpUploaderDevice;
};

QML_DECLARE_TYPEINFO(HttpUploader, 0)

#endif // HTTPUPLOADER_H
