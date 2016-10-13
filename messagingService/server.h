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
#include <thread>
#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "queue.h"
#include "Map.h"

using namespace std;

class Server {
public:
    Server(int port);
    ~Server();
    void create_first();
    void run();
    
private:
    void create();
    void close_socket();
    void serve();
    bool handle(int);
    string get_request(int);
    void get_value(int, Message&);
    bool send_response(int, string);
    Message parse_request(string);
    bool handle_message(int, Message&, string);
    Queue myQueue;
    Map myMap; 

    int port_;
    int server_;
    int buflen_;
    //char* buf_;
};
