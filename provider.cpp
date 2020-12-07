#include "provider.hpp"

#include <QTimer>

Provider::Provider(ParserWrapper &parser, QObject *parent) :
    QObject(parent),
    mParserWrapper(parser)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Provider::run,
                                           Qt::ConnectionType::QueuedConnection);
    timer->start(100);
}

void Provider::run()
{
    const auto &words = mParserWrapper.getSubset(static_cast<ushort>
                                                 (Values::DefaultValuesInTop));
    for (const auto & word : words) {
        setValue(QVariantList() << QString::fromStdString(word.second)
                                << static_cast<qulonglong>(word.first));
    }
}

void Provider::close()
{
    mParserWrapper.stop();
}

QVariantList Provider::value() const
{
    return m_value;
}

void Provider::setValue(QVariantList value)
{
    if (m_value == value)
        return;

    m_value = value;
    emit valueChanged(m_value);
}
