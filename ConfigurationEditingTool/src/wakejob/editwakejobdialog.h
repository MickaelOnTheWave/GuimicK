#ifndef EDITWAKEJOBDIALOG_H
#define EDITWAKEJOBDIALOG_H

#include "abstracteditjobdialog.h"

namespace Ui {
   class EditWakeJobDialog;
}

class EditWakeJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditWakeJobDialog(AbstractJob* _job);
   virtual ~EditWakeJobDialog();

protected:
   virtual void UpdateUiFromJob() override;
   virtual void UpdateJobFromUi() override;

private:
   Ui::EditWakeJobDialog *ui;
};

#endif // EDITWAKEJOBDIALOG_H
