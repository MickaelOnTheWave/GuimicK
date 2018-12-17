#ifndef ABSTRACTEDITJOBDIALOG_H
#define ABSTRACTEDITJOBDIALOG_H

#include <QDialog>
#include "abstractjob.h"
#include "configurationtype.h"

class AbstractEditJobDialog : public QDialog
{
   Q_OBJECT

public:
   AbstractEditJobDialog(AbstractJob* _job);
   virtual ~AbstractEditJobDialog() = default;

   virtual void SetupFor(const ConfigurationType& type);

protected slots:
   void OnCloseAccepting();
   void OnCloseRejecting();

protected:
   virtual void UpdateUiFromJob() = 0;
   virtual void UpdateJobFromUi() = 0;

   AbstractJob* job;
};

#endif // ABSTRACTEDITJOBDIALOG_H
