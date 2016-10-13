#include <iostream>
#include "Map.h"

Map::Map(){}
Map::~Map(){}

mutex Map::m;

// map<string, vector<Message> > ::iterator Map::find(string s)
// {
// 	map<string, vector<Message> > ::iterator it;
// 	unique_lock<mutex> lock(m);
// 	it = myMap.find(s);
// 	return it;
// }
// void Map::insert(pair<string,vector<Message> > w)
// {
// 	unique_lock<mutex> lock(m);
// 	myMap.insert(w);
// }
// bool Map::contains(string s)
// {
// 	unique_lock<mutex> lock(m);
// 	map<string, vector<Message> > ::iterator it;
//     it = myMap.find(s);

//     if(it != myMap.end())
//     {
//     	return true;
//     }
//     else
//     {
//     	return false;
//     }
// }

void Map::clear()
{
	unique_lock<mutex> lock(m);
	myMap.clear();
}

void Map::push(Message x)
{
	unique_lock<mutex> lock(m);
	map<string, vector<Message> > ::iterator it;
    it = myMap.find(x.getUser());
    it->second.push_back(x);
}

string Map::put(Message message)
{
	unique_lock<mutex> lock(m);
	map<string, vector<Message> > ::iterator it;
	it = myMap.find(message.getUser());

	if(it != myMap.end())
	{
	    //cout << "TEST 8" << endl;
	    it->second.push_back(message);
	    //myMap.push(message);
	    return "OK\n";
	}
	else
	{
	    //cout << "TEST 9" << endl;
	    vector<Message> v;
	    v.push_back(message);
	    //cout << "TEST 9.1" << endl;
	    myMap.insert(pair<string,vector<Message> >(message.getUser(), v));
	    return "OK\n";
	    //cout << "TEST 9.2" << endl;
	    
	   
	    //cout << "TEST 9.4" << endl;
	}
}

string Map::get(Message message)
{
	unique_lock<mutex> lock(m);
	map<string, vector<Message> > ::iterator it;
	it = myMap.find(message.getUser());
	vector<Message> myVect;
	
	if(it != myMap.end())
	{
	     //cout << "GETTER2" << endl;
	     myVect = it->second;

	     Message letter = myVect[message.getIndex() - 1];
	     string sub = letter.getSubject();
	     string words = letter.getCache();
	     string wordLength = to_string(letter.getLength());
	     // cout << "GETTER4" << endl;
	     // cout << "sub: " << sub << endl;
	     // cout << "length: " << wordLength << endl;
	     // cout << "words: " << words << endl;

	     string totalString = "message " + sub + " " + wordLength + "\n" + words;

	     //cout << "TOTAL STRING IS:" << totalString;
	     return totalString;
	     //cout << "TOTAL STRING IS ()()()()()" << totalString << endl; 
	}
	else
	{
	    //cout << "GETTER3" << endl;
	    return "error invalid get\n";
	}
}

string Map::list(Message message)
{
	unique_lock<mutex> lock(m);
	map<string, vector<Message> > ::iterator it;
	it = myMap.find(message.getUser());
	vector<Message> myVect = it->second;
	

	string totalString = "list " + to_string(myVect.size()) + "\n";

	for(int i = 0; i < myVect.size(); i++)
	{
	    totalString += to_string(i + 1) + " " + myVect[i].getSubject() + "\n";
	}

	//cout << "TOTAL STRING FOR LIST IS" << endl << totalString << endl;
	return totalString;
	
}


