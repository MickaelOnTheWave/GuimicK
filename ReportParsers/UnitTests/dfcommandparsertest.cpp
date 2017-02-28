#include "dfcommandparsertest.h"

#include <QStringList>
#include <QTest>

#include "dfcommandparser.h"
#include "filetools.h"

using namespace std;

DfCommandParserTest::DfCommandParserTest()
{
}

void DfCommandParserTest::init()
{

}

void DfCommandParserTest::cleanup()
{

}

void DfCommandParserTest::testParse_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QStringList>("driveNames");
    QTest::addColumn<QStringList>("sizes");
    QTest::addColumn<QStringList>("usedSpaces");
    QTest::addColumn<QStringList>("freeSpaces");
    QTest::addColumn<QStringList>("occupationRatios");

    QTest::newRow("One drive")
        << "oneDrive.txt"
        << QStringList({"/dev/sda2"})
        << QStringList({"230G"})
        << QStringList({"168G"})
        << QStringList({"50G"})
        << QStringList({"78%"});

    QTest::newRow("Multiple drives")
        << "allDrives.txt"
        << QStringList({"/dev/sda2", "/dev/sda1", "/dev/sdb1", "/dev/sdc2"})
        << QStringList({"230 Gb", "37 Mb", "334 Gb", "962 Gb"})
        << QStringList({"168 Gb", "6.3 Mb", "268 Gb", "72 Mb"})
        << QStringList({"50 Gb", "31 Mb", "67 Gb", "913 Gb"})
        << QStringList({"78%", "18%", "81%", "1%"});
}

void DfCommandParserTest::testParse()
{
    QFETCH(QString, file);
    string content = FileTools::GetTextFileContent(file.toStdString());

    DfCommandParser parser;
    bool ok = parser.ParseBuffer(content);
    QCOMPARE(ok, true);

    vector<Drive> drives;
    parser.GetDrives(drives);

    QFETCH(QStringList, driveNames);
    CheckDataIsTheSame(drives, driveNames, [] (const Drive& d) { return d.name; } );

    QFETCH(QStringList, sizes);
    CheckDataIsTheSame(drives, sizes,  [] (const Drive& d) { return d.totalSpace; } );

    QFETCH(QStringList, usedSpaces);
    CheckDataIsTheSame(drives, usedSpaces,  [] (const Drive& d) { return d.usedSpace; } );

    QFETCH(QStringList, freeSpaces);
    CheckDataIsTheSame(drives, freeSpaces,  [] (const Drive& d) { return d.freeSpace; } );

    QFETCH(QStringList, occupationRatios);
    CheckDataIsTheSame(drives, occupationRatios,  [] (const Drive& d) { return d.ratio; } );
}

void DfCommandParserTest::CheckDataIsTheSame(const vector<Drive> &data,
                                             const QStringList &expectedData,
                                             function<string(Drive)> propertyGetter)
{
    QCOMPARE(data.size(), static_cast<unsigned long>(expectedData.size()));

    for (unsigned int i=0; i<data.size(); ++i)
        QCOMPARE(propertyGetter(data[i]), expectedData.at(i).toStdString());
}
