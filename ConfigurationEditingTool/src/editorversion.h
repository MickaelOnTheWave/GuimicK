#ifndef EDITORVERSION_H
#define EDITORVERSION_H

#include <string>

class EditorVersion
{
public:
   /**
    * @brief Creates the global instance
    * @return A pointer to the global instance for
    * setup purposes. nullptr if the global instance
    * was already created.
    */
   static EditorVersion* Create();

   static bool IsStandaloneOnly();
   static bool HasDevelopmentFeatures();
   static bool IsWindowsVersion();
   static bool IsLinuxVersion();

   static std::wstring GetProductName();
   static std::wstring GetEditorName();
   static std::wstring GetVersionTag();

   void SetAsDevelopmentVersion(const bool value);
   void SetAsWindowsVersion(const bool value);
   void SetAsStandaloneOnly(const bool value);

private:
   EditorVersion() = default;

   static EditorVersion* instance;
   bool hasDevelopmentFeatures;
   bool isWindows;
   bool isStandaloneOnly;
   const std::wstring productName = L"GuimicK";
   const std::wstring editorName = L"GuimicK Configuration";
   const std::wstring version = L"1.0.16";
};

#endif // EDITORVERSION_H
