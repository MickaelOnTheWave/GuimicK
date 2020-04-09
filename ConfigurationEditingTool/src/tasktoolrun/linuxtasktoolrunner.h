#ifndef LINUXTASKTOOLRUNNER_H
#define LINUXTASKTOOLRUNNER_H

#include "abstracttasktoolrunner.h"

class LinuxTaskToolRunner : public AbstractTaskToolRunner
{
    Q_OBJECT
public:
   ErrorMessageCreator* CreateMessageCreator() override;

private:
   void RunFunction() override;
};

#endif // LINUXTASKTOOLRUNNER_H
