#include "provider.hpp"

#include <QTimer>

Provider::Provider(QObject *parent) :
    QObject(parent)
{}

void Provider::run(std::map<std::string, size_t> words)
{
    for (const auto & word : words) {
        setValue(QVariantList() << QString::fromStdString(word.first)
                 << static_cast<qulonglong>(word.second));
    }
}

void Provider::close()
{
    //    mParserWrapper.stop();
}

QVariantList Provider::value() const
{
    return m_value;
}

ushort Provider::completedPercent() const
{
    return m_completedPercent;
}

void Provider::setValue(QVariantList value)
{
    if (m_value == value)
        return;

    m_value = value;
    emit valueChanged(m_value);
}

void Provider::setCompletedPercent(ushort completedPercent)
{
    if (m_completedPercent == completedPercent)
        return;

    m_completedPercent = completedPercent;
    emit completedPercentChanged(m_completedPercent);
}
