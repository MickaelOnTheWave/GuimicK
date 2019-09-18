#ifndef DESTINATIONPATHVALIDATOR_H
#define DESTINATIONPATHVALIDATOR_H

#include <QValidator>

class DestinationPathValidator : public QValidator
{
   Q_OBJECT

public:
   DestinationPathValidator(const QString& _sourcePath);

   QValidator::State validate(QString &input, int &pos) const override;

signals:
   void Error(const QString& msg) const;
   void ValidDestination(const QString& msg) const;

public slots:
   void UpdateSourcePath(const QString& value);

private:
   bool IsInSourcePath(const QString& value) const;

   QString sourcePath;
};

#endif // DESTINATIONPATHVALIDATOR_H
