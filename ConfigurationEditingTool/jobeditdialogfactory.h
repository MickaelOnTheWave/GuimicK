#ifndef JOBEDITDIALOGFACTORY_H
#define JOBEDITDIALOGFACTORY_H

#include "abstracteditjobdialog.h"
#include "abstractjob.h"

class JobEditDialogFactory
{
public:

   static AbstractEditJobDialog* Create(AbstractJob* job);
};

#endif // JOBEDITDIALOGFACTORY_H
