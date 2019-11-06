#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "editorversion.h"

AboutDialog::AboutDialog(const ConfigurationType configurationType,
                         QWidget *parent) :
   QDialog(parent),
   ui(new Ui::AboutDialog)
{
   ui->setupUi(this);
   SetWindowTitle();
   SetExeInfo(configurationType);
   SetPartOfInfo();
}

AboutDialog::~AboutDialog()
{
   delete ui;
}

void AboutDialog::SetWindowTitle()
{
   const std::wstring title = L"About " + EditorVersion::GetEditorName();
   setWindowTitle(QString::fromStdWString(title));
}

void AboutDialog::SetExeInfo(const ConfigurationType configurationType)
{
   const std::wstring editorInfo = EditorVersion::GetEditorName() +
                                   L" v" + EditorVersion::GetVersionTag();
   QString fullExeInfo(QString::fromStdWString(editorInfo));
   if (EditorVersion::IsStandaloneOnly() == false)
   {
      fullExeInfo.append("\t (");
      fullExeInfo.append(CreateModeString(configurationType)).append(")");
   }
   ui->labelExeInfo->setText(fullExeInfo);
}

void AboutDialog::SetPartOfInfo()
{
   std::wstring partOfInfo = L"This editor is part of ";
   partOfInfo += EditorVersion::GetProductName();
   ui->labelPartOfInfo->setText(QString::fromStdWString(partOfInfo));
}

QString AboutDialog::CreateModeString(const ConfigurationType configurationType) const
{
   switch (configurationType)
   {
      case ServerConfigurationType : return "Server";
      case ClientConfigurationType : return "Client";
      case StandaloneConfigurationType : return "Standalone";
      default : return "Unsupported";
   }
}
