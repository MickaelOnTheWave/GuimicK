#ifndef EDITCONSOLEJOBDIALOG_H
#define EDITCONSOLEJOBDIALOG_H

#include "abstracteditjobdialog.h"
#include "userconsolejob.h"

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

   void on_successTypeBox_currentIndexChanged(int index);

   void on_outputTypeBox_activated(const QString &arg1);

   void on_useParserBox_clicked();

   void on_useCurrentClientBox_clicked();

   void on_addAttachmentButton_clicked();

   void on_removeAttachmentButton_clicked();

protected:
   virtual void UpdateUiFromJob() override;
   virtual void UpdateJobFromUi() override;

private:
   void UpdateUiFromJob_Basic();
   void UpdateUiFromJob_User();
   void UpdateUiFromUserConsoleJob(UserConsoleJob* userJob);

   void UpdateJobFromUi_Basic();
   void UpdateJobFromUi_User();

   UserConsoleJob* GetUserConsoleJob();

   void UpdateParserUi();

   Ui::EditConsoleJobDialog *ui;
};

#endif // EDITCONSOLEJOBDIALOG_H
