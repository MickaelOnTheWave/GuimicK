#ifndef LINUXTASKTOOLRUNNER_H
#define LINUXTASKTOOLRUNNER_H

#include "abstracttasktoolrunner.h"

class LinuxTaskToolRunner : public AbstractTaskToolRunner
{
    Q_OBJECT
public:
   void Run() override;

   ErrorMessageCreator* CreateMessageCreator() override;
};

#endif // LINUXTASKTOOLRUNNER_H
