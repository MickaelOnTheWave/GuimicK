#ifndef EDITCONSOLEJOBDIALOG_H
#define EDITCONSOLEJOBDIALOG_H

#include "abstracteditjobdialog.h"

namespace Ui {
   class EditConsoleJobDialog;
}

class EditConsoleJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditConsoleJobDialog(AbstractJob* _job);
   virtual ~EditConsoleJobDialog();

private slots:
   void on_okButton_clicked();

   void on_cancelButton_clicked();

   void on_successTypeBox_activated(const QString &arg1);

   void on_successTypeBox_currentIndexChanged(int index);

private:
   void UpdateUiFromJob();
   void UpdateJobFromUi();

   Ui::EditConsoleJobDialog *ui;
};

#endif // EDITCONSOLEJOBDIALOG_H
