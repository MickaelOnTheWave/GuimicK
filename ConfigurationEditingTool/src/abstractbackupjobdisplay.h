#ifndef ABSTRACTBACKUPJOBDISPLAY_H
#define ABSTRACTBACKUPJOBDISPLAY_H

#include "abstractdisplay.h"

namespace Ui {
   class AbstractBackupJobDisplay;
}

class AbstractBackupJobDisplay : public AbstractDisplay
{
   Q_OBJECT

public:
   explicit AbstractBackupJobDisplay(QWidget *parent = nullptr);
   ~AbstractBackupJobDisplay() override;

   virtual void Initialize(AbstractJob* job) override;

private:
   void SetIcon(AbstractJob* job);

   Ui::AbstractBackupJobDisplay *ui;
};

#endif // ABSTRACTBACKUPJOBDISPLAY_H
