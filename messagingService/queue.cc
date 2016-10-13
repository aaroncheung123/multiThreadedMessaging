#include <iostream>
#include "queue.h"

Queue::Queue(){}
Queue::~Queue(){}

mutex Queue::m;
condition_variable Queue:: notfull;
condition_variable Queue:: notempty;

void Queue::add(int i)
{
	// static mutex m;
	// static condition_variable notfull;
	
	// static queue<int> myQueue;
	unique_lock<mutex> lock(m);
	//cout << "q1" << endl;
	while(myQueue.size() == 100)
	{
		//cout << "q2" << endl;
		notfull.wait(lock);
	}
	myQueue.push(i);
	//cout << "q3" << endl;
	notempty.notify_one();
	//cout << "q4" << endl;
}

int Queue::remove()
{
	//static condition_variable notempty;
	unique_lock<mutex> lock(m);
	//cout << "HANNDDDDLLEEE -------------------****" << endl;
	//cout << "r1" << endl;
	while(myQueue.empty())
	{
		//cout << "r2" << endl;
		notempty.wait(lock);
	}
	//save front first
	int temp = myQueue.front();
	//cout << "r3" << endl;
	myQueue.pop();
	//lock.unlock();
	notfull.notify_one();
	//cout << "r4" << endl;
	return temp;
}