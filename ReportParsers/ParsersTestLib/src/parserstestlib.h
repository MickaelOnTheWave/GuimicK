#ifndef PARSERSTESTLIB_H
#define PARSERSTESTLIB_H

#include <QObject>
#include <vector>

void GetParserLibTests(std::vector<QObject*>& tests,
                       const std::string& dataPathPrefix = "");

#endif // PARSERSTESTLIB_H
