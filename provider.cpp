#include "provider.hpp"

#include <QUrl>
#include <QFile>

Provider::Provider(start_func start, stop_func stop, QObject *parent) :
    QObject(parent),
    mStart(start),
    mStop(stop)
{}

void Provider::deliverMap(word_map words)
{
    QVariantMap map;
    for (const auto & [word, count] : words) {
        map.insert(QString::fromStdString(word), static_cast<qulonglong>(count));
    }

    QMetaObject::invokeMethod(this,
                              "setMap",
                              Qt::QueuedConnection,
                              Q_ARG(QVariantMap, map));
}

void Provider::progress(ushort percent)
{
    QMetaObject::invokeMethod(this,
                              "setCompletedPercent",
                              Qt::QueuedConnection,
                              Q_ARG(ushort, percent));
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

void Provider::stop() const
{
    mStop();
}

QVariantMap Provider::map() const
{
    return m_map;
}

ushort Provider::completedPercent() const
{
    return m_completedPercent;
}

void Provider::setMap(QVariantMap map)
{
    m_map = map;
    emit mapChanged(m_map);
}

void Provider::setCompletedPercent(ushort completedPercent)
{
    m_completedPercent = completedPercent;
    emit completedPercentChanged(m_completedPercent);
}
