#ifndef SEVENZIPBACKUPWIDGET_H
#define SEVENZIPBACKUPWIDGET_H

#include "editbackupjobwidget.h"

class SevenZipBackupWidget : public EditBackupJobWidget
{
   Q_OBJECT
public:
   explicit SevenZipBackupWidget(QWidget* parent = nullptr);

protected:
   void FinishedDestinationEditing(const QString& value) override;

private:
   static QString AddZipExtension(const QString& value);
};

#endif // SEVENZIPBACKUPWIDGET_H
