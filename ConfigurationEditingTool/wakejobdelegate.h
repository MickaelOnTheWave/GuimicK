#ifndef WAKEJOBDELEGATE_H
#define WAKEJOBDELEGATE_H

#include <QStyledItemDelegate>

#include "wakelistwidget.h"

class WakeJobDelegate : public QStyledItemDelegate
{
public:
   explicit WakeJobDelegate(QObject* parent = Q_NULLPTR);
   virtual ~WakeJobDelegate();

   virtual QWidget* createEditor(
         QWidget *parent, const QStyleOptionViewItem &option,
         const QModelIndex &index) const override;

private:
   WakeListWidget* widget;
};

#endif // WAKEJOBDELEGATE_H
