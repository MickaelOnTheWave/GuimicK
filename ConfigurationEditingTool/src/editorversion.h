#ifndef EDITORVERSION_H
#define EDITORVERSION_H

#include <string>

class EditorVersion
{
public:
   static void CreateAsClientVersion(const bool _isWindows);
   static void CreateAsDevelopmentVersion(const bool _isWindows);

   static bool IsStandaloneOnly();
   static bool HasDevelopmentFeatures();
   static bool IsWindowsVersion();
   static bool IsLinuxVersion();

   static std::wstring GetVersionTag();

private:
   EditorVersion() = default;

   static EditorVersion* instance;
   bool hasDevelopmentFeatures;
   bool isWindows;
   bool isStandaloneOnly;
   const std::wstring version = L"1.07";
};

#endif // EDITORVERSION_H
