#include "destinationpathvalidator.h"

DestinationPathValidator::DestinationPathValidator(const QString& _sourcePath)
   : sourcePath(_sourcePath)
{
}

QValidator::State DestinationPathValidator::validate(QString& input, int&) const
{
   const bool isInSourcePath = input.startsWith(sourcePath);
   if (isInSourcePath)
      emit Error("Destination is inside source");

   return (isInSourcePath) ? QValidator::Invalid : QValidator::Acceptable;
}

void DestinationPathValidator::UpdateSourcePath(const QString& value)
{
   sourcePath = value;
}
