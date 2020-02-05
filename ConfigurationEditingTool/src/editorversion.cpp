#include "editorversion.h"

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
   return instance->isStandaloneOnly;
}

bool EditorVersion::HasDevelopmentFeatures()
{
    return instance->hasDevelopmentFeatures;
}

bool EditorVersion::IsWindowsVersion()
{
    return instance->isWindows;
}

bool EditorVersion::IsLinuxVersion()
{
   return !instance->isWindows;
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
   return instance->version;
}

void EditorVersion::SetAsDevelopmentVersion(const bool value)
{
   hasDevelopmentFeatures = value;
}

void EditorVersion::SetAsWindowsVersion(const bool value)
{
   isWindows = value;
}

void EditorVersion::SetAsStandaloneOnly(const bool value)
{
   isStandaloneOnly = value;
}
