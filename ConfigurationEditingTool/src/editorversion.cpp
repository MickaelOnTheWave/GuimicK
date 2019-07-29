#include "editorversion.h"

EditorVersion* EditorVersion::instance = nullptr;

void EditorVersion::CreateAsClientVersion(const bool _isWindows)
{
   Initialize(_isWindows, false, true);
}

void EditorVersion::CreateAsDevelopmentVersion(const bool _isWindows)
{
   Initialize(_isWindows, true, false);
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

void EditorVersion::Initialize(const bool _isWindows,
                               const bool _hasDevFeatures,
                               const bool _isStandaloneOnly)
{
   instance = new EditorVersion();
   instance->hasDevelopmentFeatures = _hasDevFeatures;
   instance->isWindows = _isWindows;
   instance->isStandaloneOnly = _isStandaloneOnly;
}
