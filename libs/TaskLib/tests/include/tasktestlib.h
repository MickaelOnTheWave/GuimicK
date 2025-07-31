#ifndef TASKTESTLIB_H
#define TASKTESTLIB_H

#include <QObject>
#include <vector>

void GetTaskLibTests(std::vector<QObject*>& tests,
                     const std::wstring& dataPrefix,
                     const std::wstring& errorPrefix);

#endif // TASKTESTLIB_H
