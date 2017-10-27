#include "wakejobdelegate.h"

WakeJobDelegate::WakeJobDelegate(QObject* parent)
   : QStyledItemDelegate(parent),
     widget(new WakeListWidget())
{
}

WakeJobDelegate::~WakeJobDelegate()
{
   delete widget;
}

QWidget* WakeJobDelegate::createEditor(
      QWidget* parent, const QStyleOptionViewItem& option,
      const QModelIndex& index) const
{
   auto widget = new WakeListWidget();
   widget->Initialize("blabla", 3, 10);
   return widget;
}
