#ifndef REPORTDISPATCHER_H
#define REPORTDISPATCHER_H

/**
  Interface for all classes that manages dispatching of
  maintenance report to user (email sending etc...).
  */
class IReportDispatcher
{
public :
	virtual Dispatch() = 0;
};


#endif // REPORTDISPATCHER_H
