#include "jobdatamodel.h"

#include <QSize>

using namespace std;

JobDataModel::JobDataModel(QObject *parent)
   : QAbstractListModel(parent)
{
}

QVariant JobDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   return QVariant("Jobs");
}

int JobDataModel::rowCount(const QModelIndex &parent) const
{
   // For list models only the root node (an invalid parent) should return the list's size. For all
   // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
   if (parent.isValid())
      return 0;

   return static_cast<int>(jobs.size());
}

QVariant JobDataModel::data(const QModelIndex &index, int role) const
{
   if (!index.isValid())
      return QVariant();
   else if (role == Qt::DisplayRole)
   {
      AbstractJob* job = jobs[index.row()];
      return QString(job->GetName().c_str());
   }
   else if (role == Qt::SizeHintRole)
      return QVariant();
   else
      return QVariant();
}

void JobDataModel::Clear()
{
   for (auto it : jobs)
      delete it;
   jobs.clear();
}

void JobDataModel::AddJobs(const std::list<AbstractJob*>& _jobs)
{
   copy(_jobs.begin(), _jobs.end(), back_inserter(jobs));
}

void JobDataModel::Insert(const int index, AbstractJob* job)
{
   jobs.insert(jobs.begin()+index, job);
}

void JobDataModel::MoveJob(const int oldIndex, const int newIndex)
{
   AbstractJob* oldJob = jobs[oldIndex];
   AbstractJob* newJob = jobs[newIndex];;
   jobs[oldIndex] = newJob;
   jobs[newIndex] = oldJob;
}

bool JobDataModel::removeRows(int row, int count, const QModelIndex& parent)
{
   auto it = jobs.begin()+row;
   jobs.erase(it, it+count);
   return true;
}

AbstractJob* JobDataModel::GetJob(const QModelIndex& index)
{
   return jobs[index.row()];
}

