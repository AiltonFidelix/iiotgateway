#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QObject>

class TestUtils
{
public:
    TestUtils() = default;
    ~TestUtils() = default;

    static QByteArray readJsonFile(const QString &filename);
};

#endif // TESTUTILS_H
