#ifndef JOBDELEGATE_H
#define JOBDELEGATE_H

#include <QStyledItemDelegate>
#include "abstractdisplay.h"

class JobDelegate : public QStyledItemDelegate
{
public:
   JobDelegate(AbstractDisplay* displayWidget);
   virtual ~JobDelegate();

   void paint(QPainter *painter,
              const QStyleOptionViewItem &option,
              const QModelIndex &index) const override;

   QSize sizeHint(const QStyleOptionViewItem &option,
                  const QModelIndex &index) const override;

   virtual QWidget* createEditor(
         QWidget *parent, const QStyleOptionViewItem &option,
         const QModelIndex &index) const override;

private:
   AbstractDisplay* widget;
};

#endif // JOBDELEGATE_H
