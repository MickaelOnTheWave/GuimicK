#ifndef EDITABSTRACTJOBDIALOG_H
#define EDITABSTRACTJOBDIALOG_H

#include "abstracteditjobdialog.h"

namespace Ui {
class EditAbstractJobDialog;
}

class EditAbstractJobDialog : public AbstractEditJobDialog
{
    Q_OBJECT

public:
    explicit EditAbstractJobDialog(AbstractJob* _job);
    virtual ~EditAbstractJobDialog();

protected:
   void UpdateUiFromJob() override;
   void UpdateJobFromUi() override;

private:
    Ui::EditAbstractJobDialog *ui;
};

#endif // EDITABSTRACTJOBDIALOG_H
