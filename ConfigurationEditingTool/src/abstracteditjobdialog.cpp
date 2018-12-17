#include "abstracteditjobdialog.h"

AbstractEditJobDialog::AbstractEditJobDialog(AbstractJob* _job)
   : QDialog(nullptr), job(_job)
{
}

void AbstractEditJobDialog::SetupFor(const ConfigurationType&)
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
