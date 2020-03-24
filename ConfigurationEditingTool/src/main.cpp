#ifdef _WIN32
   #include <Windows.h>
#endif   

#include <QApplication>

#include "editorversion.h"
#include "mainwindow.h"
#include "ostools.h"

namespace  {
   int RunEditorMain(int argc, char *argv[])
   {
   #ifdef _WIN32
      ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
   #endif

      QApplication a(argc, argv);
      QCoreApplication::setOrganizationName("Task Manager");
      QCoreApplication::setApplicationName("Configuration Editor");

      EditorVersion::Create();
      MainWindow w;
      w.CreateDefaultFile();
      w.show();
      return a.exec();
   }

   bool NeedsAdminElevation()
   {
      return OsTools::IsOnWindows() && !OsTools::IsRunningAsAdministrator();
   }

   int RunElevated()
   {
      char szPath[MAX_PATH];
      if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
      {
         SHELLEXECUTEINFO sei = { sizeof(sei) };

         sei.lpVerb = "runas";
         sei.lpFile = szPath;
         sei.hwnd = NULL;
         sei.nShow = SW_NORMAL;

         ShellExecuteEx(&sei);
      }
      return 0;
   }
}

int main(int argc, char *argv[])
{
   if (NeedsAdminElevation())
      return RunElevated();
   else
      return RunEditorMain(argc, argv);
}
