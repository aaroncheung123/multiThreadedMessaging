#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include "message.h"
#include <map>
#include <vector>

using namespace std;

class Server {
public:
    Server(int port);
    ~Server();

    void run();
    
private:
    void create();
    void close_socket();
    void serve();
    void handle(int);
    string get_request(int);
    void get_value(int, Message&);
    bool send_response(int, string);
    Message parse_request(string);
    bool handle_message(int, Message&, string);

    int port_;
    int server_;
    int buflen_;
    char* buf_;

    map<string,vector<Message> > myMap;
};
