#include "testutils.h"

#include <QFile>

QByteArray
TestUtils::readJsonFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        return QByteArray();
    }

    auto data = file.readAll();

    file.close();

    return data;
}
