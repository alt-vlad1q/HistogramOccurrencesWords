#include "provider.hpp"

#include <QUrl>
#include <QFile>
#include <QMetaObject>
#include <QDebug>

Provider::Provider(start_func start, stop_func stop, QObject *parent) :
    QObject(parent),
    mStart(start),
    mStop(stop)
{}

void Provider::deliver(word_map words)
{
    [this, words] {
        for (const auto & [word, count] : words) {
            const auto variantList = QVariantList() << QString::fromStdString(word)
                                                    << static_cast<qulonglong>(count);
            QMetaObject::invokeMethod(this,
                                      "valueChanged",
                                      Qt::QueuedConnection,
                                      Q_ARG(QVariantList, variantList));
        }
    } ();
}

void Provider::deliverMap(word_map words)
{
    [this, words] {
        for (const auto & [word, count] : words) {
            m_map.insert(QString::fromStdString(word), QVariant::fromValue(count));
        }
        QMetaObject::invokeMethod(this,
                                  "mapChanged",
                                  Qt::QueuedConnection,
                                  Q_ARG(QVariantMap, m_map));
    } ();
}

void Provider::progress(ushort percent)
{
    [this, percent] {
        QMetaObject::invokeMethod(this,
                                  "completedPercentChanged",
                                  Qt::QueuedConnection,
                                  Q_ARG(ushort, percent));
    } ();
}

QString Provider::start(const QUrl & path) const
{
    const auto pathFile = path.toLocalFile();
    QFile file {pathFile};
    if (!file.open(QIODevice::ReadOnly))
        return {"Error opennig file."};
    if (!file.size())
        return {"File is empty."};
    file.close();

    mStart(pathFile.toStdString());

    return {};
}

bool Provider::stop() const
{
    return mStop();
}
