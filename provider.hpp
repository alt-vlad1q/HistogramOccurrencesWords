#pragma once

#include "parser-wrapper.hpp"

#include <QObject>
#include <QPair>
#include <QVariant>
#include <random>

class Provider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList value READ value WRITE setValue NOTIFY valueChanged)

    enum {
        DefaultValuesInTop = 15
    };

public:
    explicit Provider(ParserWrapper &parser, QObject *parent = nullptr);

    void run();
    QVariantList value() const;

public slots:
    void setValue(QVariantList value);

signals:
    void valueChanged(QVariantList value);

private:
    QVariantList m_value;
    ParserWrapper &mParserWrapper;
};
