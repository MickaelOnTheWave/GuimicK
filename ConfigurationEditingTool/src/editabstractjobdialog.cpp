#include "editabstractjobdialog.h"
#include "ui_editabstractjobdialog.h"

EditAbstractJobDialog::EditAbstractJobDialog(AbstractJob* _job)
    : AbstractEditJobDialog(_job),
    ui(new Ui::EditAbstractJobDialog)
{
    ui->setupUi(this);
    UpdateUiFromJob();

    connect(ui->closeButtonBox, SIGNAL(accepted()), this, SLOT(OnCloseAccepting()));
    connect(ui->closeButtonBox, SIGNAL(rejected()), this, SLOT(OnCloseRejecting()));
}

EditAbstractJobDialog::~EditAbstractJobDialog()
{
    delete ui;
}

void EditAbstractJobDialog::UpdateUiFromJob()
{
    ui->jobnameEdit->setText(QString::fromStdWString(job->GetName()));
}

void EditAbstractJobDialog::UpdateJobFromUi()
{
    job->SetName(ui->jobnameEdit->text().toStdWString());
}
