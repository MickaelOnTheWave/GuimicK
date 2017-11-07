#ifndef ABSTRACTEDITJOBDIALOG_H
#define ABSTRACTEDITJOBDIALOG_H

#include <QDialog>
#include "abstractjob.h"

class AbstractEditJobDialog : public QDialog
{
public:
   AbstractEditJobDialog(AbstractJob* _job);
   virtual ~AbstractEditJobDialog() = default;

protected:
   AbstractJob* job;
};

#endif // ABSTRACTEDITJOBDIALOG_H
