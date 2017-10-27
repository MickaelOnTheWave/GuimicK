#include "wakelistwidget.h"
#include "ui_wakelistwidget.h"

WakeListWidget::WakeListWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::WakeListWidget)
{
   ui->setupUi(this);
}

WakeListWidget::~WakeListWidget()
{
   delete ui;
}

void WakeListWidget::Initialize(const QString& name, const int retries, const int timeout)
{
   ui->nameLabel->setText(name);
   if (name == "Wake")
      ui->typeLabel->setVisible(false);

   ui->maxRetriesLabel->setText(QString::number(retries));
   ui->timeoutLabel->setText(QString::number(timeout));
}
