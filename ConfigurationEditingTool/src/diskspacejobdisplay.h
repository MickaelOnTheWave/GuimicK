#ifndef DISKSPACEJOBDISPLAY_H
#define DISKSPACEJOBDISPLAY_H

#include "abstractdisplay.h"

namespace Ui {
   class DiskSpaceJobDisplay;
}

class DiskSpaceJobDisplay : public AbstractDisplay
{
   Q_OBJECT

public:
   explicit DiskSpaceJobDisplay(QWidget *parent = nullptr);
   virtual ~DiskSpaceJobDisplay() override;

   virtual void Initialize(AbstractJob* job) override;

private:
   static std::wstring GetDefaultTypeName(AbstractJob* job);

   static QString CreateDriveLabel(const std::wstring& driveName);

   Ui::DiskSpaceJobDisplay *ui;
};

#endif // DISKSPACEJOBDISPLAY_H
