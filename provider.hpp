#pragma once

#include <QObject>
#include <QVariantMap>

class QUrl;

//! \brief The Provider class
//! \details Объект который связывает расчётную часть и gui
class Provider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap map READ map WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(ushort completedPercent READ completedPercent WRITE setCompletedPercent NOTIFY completedPercentChanged)

    using start_func = std::function<void(const std::string &)>;
    using stop_func = std::function<void()>;
    using word_map = std::map<std::string, size_t>;

public:
    explicit Provider(start_func start, stop_func stop, QObject *parent = nullptr);

    void deliverMap(word_map words);
    void progress(ushort percent);

    Q_INVOKABLE QString start(const QUrl &path) const;
    Q_INVOKABLE void stop() const;

    QVariantMap map() const;
    ushort completedPercent() const;

public slots:
    void setMap(QVariantMap map);
    void setCompletedPercent(ushort completedPercent);

signals:
    void completedPercentChanged(ushort completedPercent);
    void mapChanged(QVariantMap map);

private:
    start_func mStart;
    stop_func mStop;
    ushort m_completedPercent;
    QVariantMap m_map;
};
