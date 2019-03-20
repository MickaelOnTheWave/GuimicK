#include "consolejobdisplay.h"
#include "ui_consolejobdisplay.h"

#include "userconsolejob.h"
#include "sshconsolejob.h"

ConsoleJobDisplay::ConsoleJobDisplay(QWidget *parent) :
   AbstractDisplay(parent),
   ui(new Ui::ConsoleJobDisplay)
{
   ui->setupUi(this);
}

ConsoleJobDisplay::~ConsoleJobDisplay()
{
   delete ui;
}

void ConsoleJobDisplay::Initialize(AbstractJob* job)
{
   ui->nameLabel->setText(QString::fromStdWString(job->GetName()));
   auto consoleJob = dynamic_cast<AbstractConsoleJob*>(job);
   if (consoleJob)
   {
      InitializeTypeLabel(consoleJob);
      InitializeCommandLabel(consoleJob);
      InitializeParserLabel(consoleJob);
   }
}

void ConsoleJobDisplay::InitializeTypeLabel(AbstractConsoleJob* job)
{
   if (dynamic_cast<UserConsoleJob*>(job))
      ui->typeLabel->setText("Custom command (Server)");
   else if (dynamic_cast<SshConsoleJob*>(job))
      ui->typeLabel->setText("Custom command (Client)");
   else
      ui->typeLabel->setText("Custom command (Internal)");
}

void ConsoleJobDisplay::InitializeCommandLabel(AbstractConsoleJob* job)
{
   const QString command = QString("%1 %2")
                           .arg(job->GetCommand().c_str())
                           .arg(job->GetCommandParameters().c_str());
   ui->commandLabel->setText(command);
}

void ConsoleJobDisplay::InitializeParserLabel(AbstractConsoleJob* job)
{
   UserConsoleJob* userJob = dynamic_cast<UserConsoleJob*>(job);
   if (!userJob)
   {
      auto sshJob = dynamic_cast<SshConsoleJob*>(job);
      if (sshJob)
         userJob = dynamic_cast<UserConsoleJob*>(sshJob->GetRemoteJob());
   }

   if (userJob)
   {
      const QString parserCommand = QString::fromStdWString(userJob->GetMiniDescriptionParserCommand());
      if (parserCommand != "")
         ui->parserLabel->setText(parserCommand);
      else
         ui->parserLabel->setText("None");
   }
   else
      ui->parserLabel->setText("Not Available");
}
