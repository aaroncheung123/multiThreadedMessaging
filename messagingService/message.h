#pragma once
#include <string>  
#include <sstream> 
#include <iostream>

using namespace std;

 class Message 
 {
 public:
 	Message();
 	~Message();
 	void setCommand(string);
 	void setUser(string);
 	void setSubject(string);
 	void setLetter(string);
 	void setLength(int);
 	void setCache(string);
 	void setIndex(int);
 	void setListString(string);

 	string getCommand();
 	string getUser();
 	string getSubject();
 	string getLetter();
 	int getLength();
 	string getCache();
 	int getIndex();
 	string getListString();

 	string putFormat();
 	string listFormat();
 	string readFormat();


private:
	string command;
	string user;
	string subject;
	string letter;
	int length;
	string cache;
	int index;
	string listString;
  };