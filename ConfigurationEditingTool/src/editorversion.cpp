#include "editorversion.h"

#include "ostools.h"
#include "stringtools.h"

EditorVersion* EditorVersion::instance = nullptr;

EditorVersion* EditorVersion::Create()
{
   if (instance == nullptr)
   {
      instance = new EditorVersion();
      return instance;
   }
   else
      return nullptr;
}

bool EditorVersion::IsStandaloneOnly()
{
#ifdef STANDALONE_ONLY
   return true;
#else
   return false;
#endif
}

bool EditorVersion::HasDevelopmentFeatures()
{
#ifdef NO_DEV_TOOLS
   return false;
#else
   return true;
#endif
}

bool EditorVersion::IsWindowsVersion()
{
   return OsTools::IsOnWindows();
}

bool EditorVersion::IsLinuxVersion()
{
   return !IsWindowsVersion();
}

std::wstring EditorVersion::GetProductName()
{
   return instance->productName;
}

std::wstring EditorVersion::GetEditorName()
{
   return instance->editorName;
}

std::wstring EditorVersion::GetVersionTag()
{
#ifdef VERSION
   return StringTools::Utf8ToUnicode(VERSION);
#else
   return L"1.0.20-static";
#endif
}
