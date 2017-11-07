#include "abstracteditjobdialog.h"

AbstractEditJobDialog::AbstractEditJobDialog(AbstractJob* _job)
   : QDialog(nullptr), job(_job)
{
}
