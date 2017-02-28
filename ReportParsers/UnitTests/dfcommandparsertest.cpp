#include "dfcommandparsertest.h"

#include <QStringList>
#include <QTest>

#include "dfcommandparser.h"
#include "filetools.h"

using namespace std;

const string suiteFolder = "DfCommand/";

DfCommandParserTest::DfCommandParserTest()
{
}

void DfCommandParserTest::init()
{

}

void DfCommandParserTest::cleanup()
{

}

void DfCommandParserTest::testRawData_data()
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
        << QStringList({"230 Gb"})
        << QStringList({"168 Gb"})
        << QStringList({"50 Gb"})
        << QStringList({"78%"});

    QTest::newRow("Multiple drives")
        << "allDrives.txt"
        << QStringList({"/dev/sda2", "/dev/sda1", "/dev/sdb1", "/dev/sdc2"})
        << QStringList({"230 Gb", "37 Mb", "334 Gb", "962 Gb"})
        << QStringList({"168 Gb", "6.3 Mb", "268 Gb", "72 Mb"})
        << QStringList({"50 Gb", "31 Mb", "67 Gb", "913 Gb"})
        << QStringList({"78%", "18%", "81%", "1%"});
}

void DfCommandParserTest::testRawData()
{
    DfCommandParser parser;
    CheckAndParse(parser);

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

void DfCommandParserTest::testDescriptions_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("miniDescription");
    QTest::addColumn<QString>("fullDescription");

    QTest::newRow("One drive")
        << "oneDrive.txt"
        << QString("50 Gb available (78% used)")
        << QString("");

    QTest::newRow("Multiple drives")
        << "allDrives.txt"
        << QString("4 drives checked, see report")
        << QString("/dev/sda2 : 230 Gb total, 50 Gb available (78% used)\n"
                   "/dev/sda1 : 37 Mb total, 31 Mb available (18% used)\n"
                   "/dev/sdb1 : 334 Gb total, 67 Gb available (81% used)\n"
                   "/dev/sdc2 : 962 Gb total, 913 Gb available (1% used)\n"
                   );
}

void DfCommandParserTest::testDescriptions()
{
    DfCommandParser parser;
    CheckAndParse(parser);

    QFETCH(QString, miniDescription);
    QCOMPARE(QString::fromStdString(parser.GetMiniDescription()), miniDescription);

    QFETCH(QString, fullDescription);
    QCOMPARE(QString::fromStdString(parser.GetFullDescription()), fullDescription);
}

void DfCommandParserTest::CheckAndParse(DfCommandParser &parser)
{
    QFETCH(QString, file);
    string content = FileTools::GetTextFileContent(suiteFolder + file.toStdString());

    bool ok = parser.ParseBuffer(content);
    QCOMPARE(ok, true);
}

void DfCommandParserTest::CheckDataIsTheSame(const vector<Drive> &data,
                                             const QStringList &expectedData,
                                             function<string(Drive)> propertyGetter)
{
    QCOMPARE(data.size(), static_cast<unsigned long>(expectedData.size()));

    for (unsigned int i=0; i<data.size(); ++i)
        QCOMPARE(propertyGetter(data[i]), expectedData.at(i).toStdString());
}
