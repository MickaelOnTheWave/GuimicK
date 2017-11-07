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
   ui->typeLabel->setVisible((name != "Wake"));
   ui->maxRetriesLabel->setText(QString::number(retries));
   ui->timeoutLabel->setText(QString::number(timeout));
}
