#include "destinationpathvalidator.h"

DestinationPathValidator::DestinationPathValidator(const QString& _sourcePath)
   : sourcePath(_sourcePath)
{
}

QValidator::State DestinationPathValidator::validate(QString& input, int&) const
{
   const bool isSourcePath = (input == sourcePath);
   const bool isInSourcePath = (isSourcePath) ? false : IsInSourcePath(input);

   if (isInSourcePath)
      emit Error("Destination is inside source");
   else
      emit ValidDestination(input);

   if (isInSourcePath)
       return QValidator::Invalid;
   else if (isSourcePath)
      return QValidator::Intermediate;
   else
      return QValidator::Acceptable;
}

void DestinationPathValidator::UpdateSourcePath(const QString& value)
{
   sourcePath = value;
}

bool DestinationPathValidator::IsInSourcePath(const QString& value) const
{
   const bool mightBeInSourcePath = value.startsWith(sourcePath);
   if (mightBeInSourcePath)
   {
      const QChar nextChar = value.at(sourcePath.length());
      return (nextChar == '/' || nextChar == '\\');
   }
   else
      return false;
}
