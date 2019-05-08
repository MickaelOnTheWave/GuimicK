#ifndef EDITCHECKDISKJOBDIALOG_H
#define EDITCHECKDISKJOBDIALOG_H

#include "abstracteditjobdialog.h"

namespace Ui {
class EditCheckDiskJobDialog;
}

class EditCheckDiskJobDialog : public AbstractEditJobDialog
{
    Q_OBJECT

public:
    explicit EditCheckDiskJobDialog(AbstractJob* _job);
    virtual ~EditCheckDiskJobDialog();

private slots:
    void on_chooseDiskButton_pressed();

protected:
    void UpdateUiFromJob() override;
    void UpdateJobFromUi() override;

private:
    Ui::EditCheckDiskJobDialog *ui;
};

#endif // EDITCHECKDISKJOBDIALOG_H
