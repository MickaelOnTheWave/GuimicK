#ifndef TASKTESTLIB_H
#define TASKTESTLIB_H

#include <QObject>
#include <vector>

void GetTaskLibTests(std::vector<QObject*>& tests, const std::string &dataPrefix);

#endif // TASKTESTLIB_H
