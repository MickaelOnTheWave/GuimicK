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
   explicit DiskSpaceJobDisplay(QWidget *parent = 0);
   virtual ~DiskSpaceJobDisplay();

   virtual void Initialize(AbstractJob* job) override;

private:
   Ui::DiskSpaceJobDisplay *ui;
};

#endif // DISKSPACEJOBDISPLAY_H
