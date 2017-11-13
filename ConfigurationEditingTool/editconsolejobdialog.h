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

   void on_successTypeBox_currentIndexChanged(int index);

   void on_successTypeBox_2_activated(const QString &arg1);

   void on_useParserBox_clicked();

   void on_useCurrentClientBox_clicked();

   void on_addAttachmentButton_clicked();

   void on_removeAttachmentButton_clicked();

private:
   void UpdateUiFromJob();
   void UpdateUiFromJob_Basic();
   void UpdateUiFromJob_User();
   void UpdateUiFromJob_Ssh();
   void UpdateJobFromUi();

   void UpdateParserUi();

   Ui::EditConsoleJobDialog *ui;
};

#endif // EDITCONSOLEJOBDIALOG_H
