#include "wakejobdelegate.h"

#include <QPainter>

WakeJobDelegate::WakeJobDelegate(QObject* parent)
   : QStyledItemDelegate(parent),
     widget(new WakeListWidget())
{
}

WakeJobDelegate::~WakeJobDelegate()
{
   delete widget;
}

void WakeJobDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const
{
   QString name = qvariant_cast<QString>(index.data());
   widget->Initialize(name, 3, 10);

   painter->save();
   widget->resize( option.rect.size() );
   painter->translate(option.rect.topLeft());
   widget->render(painter, QPoint(), QRegion(), QWidget::DrawChildren );
   painter->restore();
}

QSize WakeJobDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   return widget->sizeHint();
}

QWidget* WakeJobDelegate::createEditor(
      QWidget* , const QStyleOptionViewItem&, const QModelIndex&) const
{
   return nullptr;
}
