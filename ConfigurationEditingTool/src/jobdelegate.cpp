#include "jobdelegate.h"

#include <QPainter>
#include "qtmetatypes.h"

JobDelegate::JobDelegate(AbstractDisplay* displayWidget)
   : QStyledItemDelegate(Q_NULLPTR),
     widget(displayWidget)

{
}

JobDelegate::~JobDelegate()
{
   delete widget;
}

void JobDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const
{
   auto job = index.data().value<AbstractJob*>();
   if (!job)
      return;

   widget->Initialize(job);

   painter->save();
   painter->translate(option.rect.topLeft());
   if (option.state & QStyle::State_Selected)
      painter->fillRect(widget->rect(), option.palette.highlight());
   widget->render(painter, QPoint(), QRegion(), QWidget::DrawChildren);
   painter->restore();
}

QSize JobDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)
   return widget->size();
}

QWidget* JobDelegate::createEditor(
      QWidget* , const QStyleOptionViewItem&, const QModelIndex&) const
{
   return nullptr;
}
