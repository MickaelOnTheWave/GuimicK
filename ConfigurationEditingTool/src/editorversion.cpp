#include "editorversion.h"

EditorVersion* EditorVersion::instance = nullptr;

void EditorVersion::CreateAsClientVersion(const bool _isWindows)
{
   instance = new EditorVersion();
   instance->hasDevelopmentFeatures = false;
   instance->isWindows = _isWindows;
   instance->isStandaloneOnly = true;
}

void EditorVersion::CreateAsDevelopmentVersion(const bool _isWindows)
{
   instance = new EditorVersion();
   instance->hasDevelopmentFeatures = true;
   instance->isWindows = _isWindows;
   instance->isStandaloneOnly = false;
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

std::wstring EditorVersion::GetVersionTag()
{
   return instance->version;
}
