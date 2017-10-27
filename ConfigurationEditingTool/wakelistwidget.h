#ifndef WAKELISTWIDGET_H
#define WAKELISTWIDGET_H

#include <QWidget>

namespace Ui {
   class WakeListWidget;
}

class WakeListWidget : public QWidget
{
   Q_OBJECT

public:
   explicit WakeListWidget(QWidget *parent = 0);
   ~WakeListWidget();

   void Initialize(const QString& name,
                   const int retries, const int timeout);

private:
   Ui::WakeListWidget *ui;
};

#endif // WAKELISTWIDGET_H
