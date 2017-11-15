#ifndef ABSTRACTDISPLAY_H
#define ABSTRACTDISPLAY_H

#include <QWidget>
#include "abstractjob.h"

class AbstractDisplay : public QWidget
{
public:
   explicit AbstractDisplay(QWidget *parent = nullptr);
   virtual ~AbstractDisplay() = default;

   virtual void Initialize(AbstractJob* job) = 0;
};

#endif // ABSTRACTDISPLAY_H
