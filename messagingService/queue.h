#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <pthread.h>



using namespace std;

class Queue {
public:
	Queue();
	~Queue();
	void add(int);
	int remove();

private:
	static mutex m;
	static condition_variable notfull;
	static condition_variable notempty;
	queue<int> myQueue;
};