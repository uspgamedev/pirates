////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: runnable.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PIRATES_BASE_RUNNABLE_H_
#define PIRATES_BASE_RUNNABLE_H_

#include "pandaFramework.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"
#include <list>

namespace pirates {

namespace base {
class Runnable {
  public:
    virtual void Update(float dt) = 0;
  protected:
    Runnable() { }
};

class RunnableManager {
  public:
    ~RunnableManager() {
        reference_ = NULL;
    }
    static RunnableManager* reference() {
        if (reference_ == NULL)
            return (reference_ = new RunnableManager);
        return reference_;
    }

    void Initialize();
    void Add(Runnable* runnable);
    void Remove(Runnable* runnable);

  private:
    RunnableManager() {}
    AsyncTask::DoneStatus Update(double elapsed_time);
    static AsyncTask::DoneStatus TaskUpdate( GenericAsyncTask* task, void* data );

    static RunnableManager *reference_;

    double last_time_;
    std::list<Runnable*> runnables_;
};

} // namespace base
} // namespace pirates

#endif

