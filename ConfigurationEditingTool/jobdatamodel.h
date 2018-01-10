#ifndef JOBDATAMODEL_H
#define JOBDATAMODEL_H

#include "abstractjob.h"
#include <QAbstractListModel>

class JobDataModel : public QAbstractListModel
{
   Q_OBJECT

public:
   explicit JobDataModel(QObject *parent = nullptr);

   // Header:
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

   // Basic functionality:
   int rowCount(const QModelIndex &parent = QModelIndex()) const override;

   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

   void Clear();
   void AddJobs(const std::list<AbstractJob*>& _jobs);
   void Insert(const int index, AbstractJob* job);
   void MoveJob(const int oldIndex, const int newIndex);
   virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

   AbstractJob* GetJob(const QModelIndex &index);
   std::vector<AbstractJob*> GetJobs();

private:
   std::vector<AbstractJob*> jobs;
};


#endif // JOBDATAMODEL_H
