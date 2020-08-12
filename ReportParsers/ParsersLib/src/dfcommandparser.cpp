#include "dfcommandparser.h"

#include <sstream>
#include "stringtools.h"

using namespace std;

wstring CreateFormattedSize(const wstring &rawSize)
{
    const size_t lastCharPos = rawSize.size()-1;
    wstring formattedSize = rawSize.substr(0, lastCharPos);
    formattedSize += wstring(L" ") + rawSize[lastCharPos] + L"b";
    return formattedSize;
}

LogicalDrive BuildDriveFromTokens(const vector<wstring> &properties)
{
   LogicalDrive drive;
   drive.name = properties[0];
   drive.totalSpace = CreateFormattedSize(properties[1]);
   drive.usedSpace = CreateFormattedSize(properties[2]);
   drive.freeSpace = CreateFormattedSize(properties[3]);
   drive.ratio = properties[4];
   return drive;
}

int GuessErrorType(const vector<wstring> &properties)
{
   int errorType = ErrorDrive::UNDEFINED;
   if (properties[2] == L"can't" && properties[3] == L"find")
      errorType = ErrorDrive::NOT_FOUND;
   return errorType;
}

ErrorDrive CreateDriveError(const vector<wstring> &properties)
{
   ErrorDrive drive;
   drive.name = properties[1].substr(0, properties[1].length()-1);
   drive.errorType = GuessErrorType(properties);
   return drive;
}

wstring GetErrorDescription(const int errorType)
{
   if (errorType == ErrorDrive::NOT_FOUND)
      return L"Drive not found";
   else
      return L"Unknown error";
}


DfCommandParser::DfCommandParser()
{
}

bool DfCommandParser::ParseBuffer(const wstring& buffer)
{
    driveList.clear();

    vector<wstring> lines;
    StringTools::Tokenize(buffer, '\n', lines);

    if (lines.size() > 1)
        return FillDriveData(lines);
    else
        return false;
}

wstring DfCommandParser::GetMiniDescription()
{
   const size_t totalDriveCount = driveList.size() + errorDriveList.size();
    if (totalDriveCount == 1)
    {
       if (errorDriveList.empty())
         return CreateResumedMiniDescription(driveList.front());
       else
         return CreateResumedMiniDescription(errorDriveList.front());
    }
    else
        return CreateDriveListDescription();
}

wstring DfCommandParser::GetFullDescription()
{
    if (driveList.size() < 2)
        return L"";
    else
        return CreateFullDescription();
}

bool DfCommandParser::FillDriveData(const std::vector<wstring> &lines)
{
    std::vector<wstring>::const_iterator it=lines.begin()+1;
    for (; it!=lines.end(); ++it)
    {
        vector<wstring> tokens;
        TokenizeUsingWhitespaces(*it, tokens);

        if (IsDesirableDriveName(tokens[0]))
            driveList.push_back(BuildDriveFromTokens(tokens));
        else if (IsDfError(tokens[0]))
           errorDriveList.push_back(CreateDriveError(tokens));
    }
    return (!driveList.empty() && errorDriveList.empty());
}

void DfCommandParser::TokenizeUsingWhitespaces(const wstring &buffer,
                                               vector<wstring> &tokens) const
{
    tokens.clear();
    wstring currentToken(L"");
    for (unsigned int i=0; i<buffer.length(); ++i)
    {
        if (isspace(buffer[i]) != 0)
        {
            if (currentToken != L"")
            {
                tokens.push_back(currentToken);
                currentToken = L"";
            }
        }
        else
            currentToken += buffer[i];
    }
}

bool DfCommandParser::IsDesirableDriveName(const wstring &name) const
{
   return (name.length() > 0 && name[0] == '/');
}

bool DfCommandParser::IsDfError(const wstring& name) const
{
   return (name == L"df:");
}

wstring DfCommandParser::CreateResumedMiniDescription(const LogicalDrive& drive) const
{
   return drive.freeSpace + L" available (" + drive.ratio + L" used)";
}

wstring DfCommandParser::CreateResumedMiniDescription(const ErrorDrive& drive) const
{
   if (drive.errorType == ErrorDrive::NOT_FOUND)
      return L"Drive " + drive.name + L" not found";
   else
      return L"Unknown error on " + drive.name;
}

wstring DfCommandParser::CreateDriveListDescription() const
{
    wstringstream stream;
    stream << driveList.size() << " drives checked, see report";
    return stream.str();
}

wstring DfCommandParser::CreateFullDescription() const
{
   wstringstream stream;
   AddHealthyDrivesData(stream);
   AddErrorDrivesData(stream);
   return stream.str();
}

void DfCommandParser::AddHealthyDrivesData(wstringstream& descriptionStream) const
{
   const wstring tab = L"\t";
   descriptionStream << "Drive List" << endl;

   vector<LogicalDrive>::const_iterator it = driveList.begin();
   for (;it != driveList.end(); ++it)
   {
     descriptionStream << tab << it->name << " : " << it->totalSpace << " total, ";
     descriptionStream << CreateResumedMiniDescription(*it) << endl;
   }
   descriptionStream << endl;
}

void DfCommandParser::AddErrorDrivesData(wstringstream& descriptionStream) const
{
   if (!errorDriveList.empty())
   {
      const wstring tab = L"\t";
      descriptionStream << endl << endl << L"Drives with errors";

      vector<ErrorDrive>::const_iterator it = errorDriveList.begin();
      for (;it != errorDriveList.end(); ++it)
      {
        descriptionStream << tab << it->name << " : ";
        descriptionStream << GetErrorDescription(it->errorType) << endl;
      }

      descriptionStream << endl;
   }
}

