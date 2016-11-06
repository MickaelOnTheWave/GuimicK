#include "htmlreportcreator.h"

#include <tools.h>

#include <fstream>

using namespace std;

// @todo Make this configurable by config file!
static const string CSS_FILE = "report.css";

HtmlReportCreator::HtmlReportCreator()
{
}

string HtmlReportCreator::Generate(WorkResultData *data, const string &versionString)
{
    report << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << endl;
    report << "<html>" << endl;
    report << "  <head>" << endl;
    report << "    <META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" << endl;
    report << GetFileContents(CSS_FILE) << endl;
    report << "  </head>" << endl;
    report << "<body>" << endl;

    vector<pair<string, ClientJobResults*> >::iterator itClient=data->allClientsResults.begin();
    vector<pair<string, ClientJobResults*> >::iterator endClient=data->allClientsResults.end();
    for (; itClient!=endClient; itClient++)
    {
        pair<string, ClientJobResults*> clientData = *itClient;

        report << "<h1>" << clientData.first << "</h1>" << endl;
        report << "<table>" << endl;

        report << "  <tr>" << endl;
        report << "    <th>Task</th>" << endl;
        report << "    <th>Status</th>" << endl;
        report << "    <th>Time spent</th>" << endl;
        report << "  </tr>" << endl;

        int totalCode = JobStatus::NOT_EXECUTED;
        time_t totalDuration = 0;

        ClientJobResults::iterator itJob=clientData.second->begin();
        ClientJobResults::iterator endJob=clientData.second->end();
        for (; itJob!=endJob; itJob++)
        {
            pair<string, JobStatus*> jobData = *itJob;

            jobData.second->GetFilenames(jobsFilenames);

            if (jobData.second->IsWorseThan(totalCode))
                totalCode = jobData.second->GetCode();
            totalDuration += jobData.second->GetDuration();

            AddJobData(jobData.first, jobData.second->GetDescription(), jobData.second->GetCodeDescription(), Tools::FormatTimeString(jobData.second->GetDuration()));
        }
        AddJobData("Total", "", JobStatus::GetCodeDescription(totalCode), Tools::FormatTimeString(totalDuration));

        report << "</table>" << endl;
    }

    report << "<small>Task Manager version " << versionString << "</small>" << endl;
    report << "</body>" << endl;
    report << "</html>" << endl;

    return report.str();
}

void HtmlReportCreator::GetAssociatedFiles(list<string> &fileList)
{
    fileList.clear();
    vector<string>::iterator it=jobsFilenames.begin();
    vector<string>::iterator end=jobsFilenames.end();
    for (; it!=end; it++)
        fileList.push_back(*it);
}
void HtmlReportCreator::AddJobData(const string &jobName, const string &jobDescription, const string &jobStatusCode, const string &jobDuration)
{
    report << "  <tr>" << endl;
    report << "    <td>" << endl;
    report << "      <div class=name>" << jobName << "</div>" << endl;
    report << "      <div class=comment>" << jobDescription << "</div>" << endl;
    report << "    </td>" << endl;
    report << "    <td>" << jobStatusCode << "</td>" << endl;
    report << "    <td>" << jobDuration << "</td>" << endl;
    report << "  </tr>" << endl;
}

string HtmlReportCreator::GetFileContents(const string &file)
{
    ifstream fileStream;
    fileStream.open(file.c_str());
    if (!fileStream)
        return string("");

    std::string contents;
    fileStream.seekg(0, std::ios::end);
    contents.resize(fileStream.tellg());
    fileStream.seekg(0, std::ios::beg);
    fileStream.read(&contents[0], contents.size());


    fileStream.close();
    return contents;
}
