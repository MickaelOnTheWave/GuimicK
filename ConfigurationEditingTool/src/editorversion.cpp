#include "editorversion.h"

EditorVersion* EditorVersion::instance = nullptr;

void EditorVersion::CreateInstance()
{
    instance = new EditorVersion();
    instance->hasDevelopmentFeatures = true;
    instance->isWindows = false;
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
