#ifndef CONSOLEJOBDISPLAY_H
#define CONSOLEJOBDISPLAY_H

#include "abstractdisplay.h"

#include "AbstractConsoleJob.h"

namespace Ui {
   class ConsoleJobDisplay;
}

class ConsoleJobDisplay : public AbstractDisplay
{
   Q_OBJECT

public:
   explicit ConsoleJobDisplay(QWidget *parent = 0);
   virtual ~ConsoleJobDisplay();

   virtual void Initialize(AbstractJob* job) override;

private:
   void InitializeTypeLabel(AbstractConsoleJob* job);
   void InitializeCommandLabel(AbstractConsoleJob* job);
   void InitializeParserLabel(AbstractConsoleJob* job);

   Ui::ConsoleJobDisplay *ui;
};

#endif // CONSOLEJOBDISPLAY_H
