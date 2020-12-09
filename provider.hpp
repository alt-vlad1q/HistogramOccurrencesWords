#pragma once

#include <QObject>
#include <QVariant>

class QUrl;
class Provider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList value NOTIFY valueChanged)

    Q_PROPERTY(QVariantMap map NOTIFY mapChanged)

    Q_PROPERTY(ushort completedPercent NOTIFY completedPercentChanged)

    using start_func = std::function<void(const std::string &)>;
    using stop_func = std::function<bool()>;
    using word_map = std::map<std::string, size_t>;

public:
    explicit Provider(start_func start, stop_func stop, QObject *parent = nullptr);

    void deliver(word_map words);
    void deliverMap(word_map words);
    void progress(ushort percent);

    Q_INVOKABLE QString start(const QUrl &path) const;
    Q_INVOKABLE bool stop() const;

signals:
    void valueChanged(QVariantList value);
    void mapChanged(QVariantMap map);
    void completedPercentChanged(ushort completedPercent);

private:
    QVariantList m_value;
    QVariantMap m_map;

    ushort m_completedPercent;
    start_func mStart;
    stop_func mStop;
};
