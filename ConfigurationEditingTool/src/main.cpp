#ifdef _WIN32
   #include <Windows.h>
#endif   

#include <QApplication>

#include "editorversion.h"
#include "mainwindow.h"
#include "ostools.h"

namespace  {
   bool HasDevelopmentTools()
   {
#ifdef NO_DEV_TOOLS
      return false;
#else
      return true;
#endif
   }

   bool IsStandaloneOnly()
   {
#ifdef STANDALONE_ONLY
      return true;
#else
      return false;
#endif
   }

   void InitializeVersion()
   {
      EditorVersion* version = EditorVersion::Create();
      version->SetAsWindowsVersion(OsTools::IsOnWindows());
      version->SetAsDevelopmentVersion(HasDevelopmentTools());
      version->SetAsStandaloneOnly(IsStandaloneOnly());
   }
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
   ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif

   QApplication a(argc, argv);
   QCoreApplication::setOrganizationName("Task Manager");
   QCoreApplication::setApplicationName("Configuration Editor");

   InitializeVersion();
   MainWindow w;
   w.CreateDefaultFile();
   w.show();
   return a.exec();
}
