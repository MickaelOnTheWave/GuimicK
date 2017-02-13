#ifndef TASKTESTLIB_H
#define TASKTESTLIB_H

#include <QObject>
#include <vector>

void GetAllTests(std::vector<QObject*>& tests);
void FreeTests(std::vector<QObject*>& tests);

#endif // TASKTESTLIB_H
