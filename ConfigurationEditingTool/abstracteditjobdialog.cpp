#include "abstracteditjobdialog.h"

AbstractEditJobDialog::AbstractEditJobDialog(AbstractJob* _job)
   : QDialog(nullptr), job(_job)
{
}

void AbstractEditJobDialog::OnCloseAccepting()
{
   UpdateJobFromUi();
   accept();
}

void AbstractEditJobDialog::OnCloseRejecting()
{
   reject();
}
