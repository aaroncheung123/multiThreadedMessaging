#include "message.h"

Message::Message()
{
	command = "";
	user = "";
	subject = "";
	letter = "";
}
Message::~Message(){}


void Message::setCommand(string x)
{
	command = x;
}
void Message::setUser(string x)
{
	user = x;
}
void Message::setSubject(string x)
{
	subject = x;
}
void Message::setLetter(string x)
{
	letter = x;
}
void Message::setLength(int x)
{
	length = x;
}
void Message::setCache(string x)
{
	cache = x;
}
void Message::setIndex(int x)
{
	index = x;
}
void Message::setListString(string x)
{
	listString = x;
}



string Message::getCommand()
{
	return command;
}
string Message::getUser()
{
	return user;
}
string Message::getSubject()
{
	return subject;
}
string Message::getLetter()
{
	return letter;
}
int Message::getLength()
{
	return length;
}
string Message::getCache()
{
	return cache;
}
int Message::getIndex()
{
	return index;
}
string Message::getListString()
{
	return listString;
}


string Message::putFormat()
{
	return "put " + user + " " + subject + " " +
	 to_string(letter.length()) + "\n" + letter;
}
string Message::listFormat()
{
	return "list " + user + "\n";
}
string Message::readFormat()
{
	return "get " + user + " " + to_string(index) + "\n";
}
