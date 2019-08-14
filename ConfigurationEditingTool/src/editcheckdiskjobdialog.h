#ifndef EDITCHECKDISKJOBDIALOG_H
#define EDITCHECKDISKJOBDIALOG_H

#include "abstracteditjobdialog.h"

#include <map>
#include <QButtonGroup>
#include <QRadioButton>

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

protected:
    void UpdateUiFromJob() override;
    void UpdateJobFromUi() override;

private:
   void SetupUiWithDrives();

   QStringList DetectDrives();
   void AddDriveButtons(const QStringList& drives);

   void SelectDriveOnUi(const QString& drive);

    Ui::EditCheckDiskJobDialog *ui;

    QButtonGroup* driveGroup;
    std::map<QString, QAbstractButton*> driveButtonMap;
};

#endif // EDITCHECKDISKJOBDIALOG_H
