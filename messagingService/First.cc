#include <iostream>
#include "First.h"

First::First(bool* ready, mutex* mutex, condition_variable* cv) {
    ready_ = ready;
    mutex_ = mutex;
    cv_ = cv;
}

First::~First() {
}


void First::work() {
	Server s;
    unique_lock<mutex> lock(*mutex_);
    while (not *ready_) 
    {
        cv_->wait(lock);
    }
    s.handle(myQueue.remove(client));
}
