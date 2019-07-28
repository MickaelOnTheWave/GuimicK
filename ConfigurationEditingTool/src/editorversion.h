#ifndef EDITORVERSION_H
#define EDITORVERSION_H


class EditorVersion
{
public:
   static void CreateInstance();

   static bool HasDevelopmentFeatures();
   static bool IsWindowsVersion();
   static bool IsLinuxVersion();

private:
   EditorVersion() = default;

   static EditorVersion* instance;
   bool hasDevelopmentFeatures;
   bool isWindows;
};

#endif // EDITORVERSION_H
