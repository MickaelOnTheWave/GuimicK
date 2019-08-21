#include "sevenzipbackupwidget.h"

SevenZipBackupWidget::SevenZipBackupWidget(QWidget *parent)
   : EditBackupJobWidget(parent)
{
}

void SevenZipBackupWidget::FinishedDestinationEditing(const QString& value)
{
   const QString valueWithZipExtension = AddZipExtension(value);
   SetDestinationWidgetPath(valueWithZipExtension);
   EditBackupJobWidget::FinishedDestinationEditing(valueWithZipExtension);
}

QString SevenZipBackupWidget::AddZipExtension(const QString& value)
{
   const QString zipExtension = ".zip";
   return (value.endsWith(zipExtension)) ? value : value + zipExtension;
}
