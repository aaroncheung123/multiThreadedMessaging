#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include "message.h"
#include <pthread.h>

using namespace std;

class Map {
public:
	Map();
	~Map();
	map<string, vector<Message> > ::iterator find(string);
	void insert(pair<string,vector<Message> >);
	bool contains(string);
	void clear();
	void push(Message);
	string get(Message);
	string list(Message);
	string put(Message message);

private:
	static mutex m;
	map<string,vector<Message> > myMap;


};