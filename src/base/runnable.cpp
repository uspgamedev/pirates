
#include "base/runnable.h"
#include "base/game.h"

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

namespace pirates {

namespace base {

RunnableManager* RunnableManager::reference_ = NULL;

void RunnableManager::Initialize() {
    GenericAsyncTask *task = new GenericAsyncTask(string("RunnableManager"), (TaskFunc)&TaskUpdate, NULL);
    GAME()->taskMgr().add(task);
    last_time_ = 0.0;
}

void RunnableManager::Add(Runnable *runnable) {
    runnables_.push_back(runnable);
}

void RunnableManager::Remove(Runnable *runnable) {
    runnables_.remove(runnable);
}

AsyncTask::DoneStatus RunnableManager::Update(double elapsed_time) {
    std::list<Runnable*>::iterator it;
    float dt = elapsed_time - last_time_;
    for(it = runnables_.begin(); it != runnables_.end(); ++it)
        (*it)->Update(dt);
    last_time_ = elapsed_time;
    return AsyncTask::DS_cont;
}

AsyncTask::DoneStatus RunnableManager::TaskUpdate ( GenericAsyncTask* task, void* data ) { 
    RunnableManager* manager = reference();
    if(manager != NULL)
        return manager->Update(task->get_elapsed_time());
    else
        return AsyncTask::DS_done;
}

} // namespace base
} // namespace pirates
