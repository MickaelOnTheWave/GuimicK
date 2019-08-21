#ifndef CONSOLEJOBDISPLAY_H
#define CONSOLEJOBDISPLAY_H

#include "abstractdisplay.h"

#include "AbstractConsoleJob.h"
#include "sshconsolejob.h"
#include "userconsolejob.h"

namespace Ui {
   class ConsoleJobDisplay;
}

class ConsoleJobDisplay : public AbstractDisplay
{
   Q_OBJECT

public:
   explicit ConsoleJobDisplay(QWidget *parent = nullptr);
   ~ConsoleJobDisplay() override;

   virtual void Initialize(AbstractJob* job) override;

private:
   void InitializeTypeLabel(AbstractConsoleJob* job);
   void InitializeCommandLabel(AbstractConsoleJob* job);
   void InitializeParserLabel(AbstractConsoleJob* job);

   static UserConsoleJob* GetUserJob(AbstractConsoleJob* job);

   Ui::ConsoleJobDisplay *ui;
};

#endif // CONSOLEJOBDISPLAY_H
