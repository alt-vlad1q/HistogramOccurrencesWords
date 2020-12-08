#pragma once

#include <QObject>
#include <QPair>
#include <QVariant>
#include <random>
#include <map>

class Provider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(ushort completedPercent READ completedPercent WRITE setCompletedPercent NOTIFY completedPercentChanged)

    enum class Values {
        DefaultValuesInTop = 15
    };

public:
    explicit Provider(QObject *parent = nullptr);

    void run(std::map<std::string, size_t> words);
    Q_INVOKABLE void close();

    QVariantList value() const;
    ushort completedPercent() const;

public slots:
    void setValue(QVariantList value);
    void setCompletedPercent(ushort completedPercent);

signals:
    void valueChanged(QVariantList value);
    void completedPercentChanged(ushort completedPercent);

private:
    QVariantList m_value;
    ushort m_completedPercent;
};
