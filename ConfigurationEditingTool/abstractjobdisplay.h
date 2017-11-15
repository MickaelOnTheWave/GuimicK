#ifndef ABSTRACTJOBDISPLAY_H
#define ABSTRACTJOBDISPLAY_H

#include "abstractdisplay.h"

namespace Ui {
   class AbstractJobDisplay;
}

class AbstractJobDisplay : public AbstractDisplay
{
   Q_OBJECT

public:
   explicit AbstractJobDisplay(QWidget *parent = 0);
   virtual ~AbstractJobDisplay();

   virtual void Initialize(AbstractJob* job) override;

private:
   Ui::AbstractJobDisplay *ui;
};

#endif // ABSTRACTJOBDISPLAY_H
