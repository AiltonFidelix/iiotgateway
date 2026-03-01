#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QObject>

class TestUtils
{
public:
    TestUtils() = delete;

    static QByteArray readJsonFile(const QString &filename);
};

#endif // TESTUTILS_H
