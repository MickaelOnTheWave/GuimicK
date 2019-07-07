#ifndef ADMINRIGHTSWARNING_H
#define ADMINRIGHTSWARNING_H

#include <QFrame>

namespace Ui {
   class AdminRightsWarning;
}

class AdminRightsWarning : public QFrame
{
   Q_OBJECT

public:
   explicit AdminRightsWarning(QWidget *parent = nullptr);
   ~AdminRightsWarning();

private:
   Ui::AdminRightsWarning *ui;
};

#endif // ADMINRIGHTSWARNING_H
