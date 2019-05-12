#ifndef WakeJobDisplay_H
#define WakeJobDisplay_H

#include "abstractdisplay.h"

namespace Ui {
   class WakeJobDisplay;
}

class WakeJobDisplay : public AbstractDisplay
{
   Q_OBJECT

public:
   explicit WakeJobDisplay(QWidget *parent = 0);
   virtual ~WakeJobDisplay();

   virtual void Initialize(AbstractJob* job) override;

private:
   Ui::WakeJobDisplay *ui;
};

#endif // WakeJobDisplay_H
