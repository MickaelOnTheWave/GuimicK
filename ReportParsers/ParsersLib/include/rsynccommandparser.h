#ifndef RSYNCCOMMANDPARSER_H
#define RSYNCCOMMANDPARSER_H

#include "abstractfilebackupparser.h"

class RsyncCommandParser : public AbstractFileBackupParser
{
public:
    RsyncCommandParser();
    virtual ~RsyncCommandParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual void GetReport(FileBackupReport& _reportData);

private:
    typedef std::vector<std::pair<std::string, std::string> > RawFileChangeList;

    void FillChangeList(const std::vector<std::string>& reportLines,
                        RawFileChangeList& changeList);
    void FillReport(const RawFileChangeList &changeList);
    bool IsFileStatusChar(const char charToTest) const;
    bool IsTimeChangeOnly(const std::string& changeString) const;
};

#endif // RSYNCCOMMANDPARSER_H
