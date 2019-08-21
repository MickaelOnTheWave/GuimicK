#include "consolejobdisplay.h"
#include "ui_consolejobdisplay.h"

#include "editorversion.h"
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
   if (EditorVersion::HasDevelopmentFeatures())
   {
      if (dynamic_cast<UserConsoleJob*>(job))
         ui->typeLabel->setText("Custom command (Server)");
      else if (dynamic_cast<SshConsoleJob*>(job))
         ui->typeLabel->setText("Custom command (Client)");
      else
         ui->typeLabel->setText("Custom command (Internal)");
   }
   else
      ui->typeLabel->setText("Custom command");
}

void ConsoleJobDisplay::InitializeCommandLabel(AbstractConsoleJob* job)
{
   const QString jobCommand = QString::fromStdWString(job->GetCommand());
   const QString jobParameters = QString::fromStdWString(job->GetCommandParameters());
   const QString command = QString("%1 %2")
                           .arg(jobCommand)
                           .arg(jobParameters);
   ui->commandLabel->setText(command);
}

void ConsoleJobDisplay::InitializeParserLabel(AbstractConsoleJob* job)
{
   if (EditorVersion::HasDevelopmentFeatures())
   {
      UserConsoleJob* userJob = GetUserJob(job);
      if (userJob)
      {
         const QString parserCommand = QString::fromStdWString(userJob->GetMiniDescriptionParserCommand());
         if (parserCommand != "")
            ui->parserValueLabel->setText(parserCommand);
         else
            ui->parserValueLabel->setText("None");
      }
      else
         ui->parserValueLabel->setText("Not Available");
   }
   else
   {
      ui->parserLabel->setVisible(false);
      ui->parserValueLabel->setVisible(false);
   }
}

UserConsoleJob* ConsoleJobDisplay::GetUserJob(AbstractConsoleJob* job)
{
   UserConsoleJob* userJob = dynamic_cast<UserConsoleJob*>(job);
   if (!userJob)
   {
      auto sshJob = dynamic_cast<SshConsoleJob*>(job);
      if (sshJob)
         userJob = dynamic_cast<UserConsoleJob*>(sshJob->GetRemoteJob());
   }
   return userJob;
}
