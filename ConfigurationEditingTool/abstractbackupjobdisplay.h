#ifndef ABSTRACTBACKUPJOBDISPLAY_H
#define ABSTRACTBACKUPJOBDISPLAY_H

#include <QWidget>

namespace Ui {
   class AbstractBackupJobDisplay;
}

class AbstractBackupJobDisplay : public QWidget
{
   Q_OBJECT

public:
   explicit AbstractBackupJobDisplay(QWidget *parent = 0);
   ~AbstractBackupJobDisplay();

private:
   Ui::AbstractBackupJobDisplay *ui;
};

#endif // ABSTRACTBACKUPJOBDISPLAY_H
