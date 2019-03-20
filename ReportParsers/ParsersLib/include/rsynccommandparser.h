#ifndef RSYNCCOMMANDPARSER_H
#define RSYNCCOMMANDPARSER_H

#include "abstractfilebackupparser.h"

class RsyncCommandParser : public AbstractFileBackupParser
{
public:
    RsyncCommandParser();
    virtual ~RsyncCommandParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual void GetReport(FileBackupReport& _reportData);

private:
    typedef std::vector<std::pair<std::wstring, std::wstring> > RawFileChangeList;

    void FillChangeList(const std::vector<std::wstring>& reportLines,
                        RawFileChangeList& changeList);
    void FillReport(const RawFileChangeList &changeList);
    bool IsFileStatusChar(const char charToTest) const;
    bool IsTimeChangeOnly(const std::wstring& changeString) const;
};

#endif // RSYNCCOMMANDPARSER_H
