#include "server.h"

Server::Server(int port) {
    // setup variables
    port_ = port;
    buflen_ = 1024;
    //buf_ = new char[buflen_+1];
}

Server::~Server() {
    delete buf_;
}

void
Server::run() {
    // create and run the server
    create();
    serve();
}

void
Server::create() {
    struct sockaddr_in server_addr;

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // create socket
    server_ = socket(PF_INET,SOCK_STREAM,0);
    if (!server_) {
        perror("socket");
        exit(-1);
    }

    // set socket to immediately reuse port when the application closes
    int reuse = 1;
    if (setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        exit(-1);
    }

    // call bind to associate the socket with our local address and
    // port
    if (::bind(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

    // convert the socket to listen for incoming connections
    if (listen(server_,SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }
}

void
Server::close_socket() {
    close(server_);
}

void Server::create_first() 
{
    while(1) 
    {
        int temp = myQueue.remove();
        if(handle(temp))
        {
            myQueue.add(temp);
        }
        
    }
}

void Server::serve() {

    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);
      // accept clients

    //creating 10 threads
    thread t1(&Server::create_first, this);
    thread t2(&Server::create_first, this);
    thread t3(&Server::create_first, this);
    thread t4(&Server::create_first, this);
    thread t5(&Server::create_first, this);
    thread t6(&Server::create_first, this);
    thread t7(&Server::create_first, this);
    thread t8(&Server::create_first, this);
    thread t9(&Server::create_first, this);
    thread t10(&Server::create_first, this);
    vector<thread> threads;

    // for (int i=0; i < 10; i++) {
    //     // create thread
    //     threads.push_back(thread(&Server::create_first, this));
    // }
    

    while((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) 
    {
        //create a client object
        //cout << "Adding to queue **********************" << endl;
        myQueue.add(client);
        //cout << "hello1" << endl;
    }
    close_socket();
}

bool
Server::handle(int client) 
{
    // loop to handle all requests
    while (1) 
    {
        //cout << "TEST 0" << endl;
        // get a request
        string request = get_request(client);
        // break if client is done or an error occurred
        if (request.empty())
        {
            return false;
        }
            //break;
        // parse request


        //Checking the command
        //cout << "TEST 1" << endl;
        string command;
        string user;
        string subject;
        string index;
        string extra;
        int length = NULL;
        string cache;
        stringstream ss;

        //spliting up the received input
        string delimiter = "\n";
        string firstCommand = request.substr(0, request.find(" "));
        string fullCommand = request.substr(0, request.find(delimiter));
        //cout << "TEST 2" << endl;
        Message message = Message();
        cout << endl << endl << "REQUEST ***************************" << endl << request << endl;
        //*********************** STORING MESSAGES
        if(firstCommand == "put")
        {
            cache = request.substr(request.find(delimiter) + 1, request.length()-1);
            message.setCache(cache);
            //cout << "TEST 3" << endl;


            ss << fullCommand;
            ss >> command >> user >> subject >> length;

            
            cout << "PUT -------" << endl;
            cout << "command: " << command << endl;
            cout << "user: " << user << endl;
            cout << "sub: " << subject << endl;
            cout << "length: " << length << endl;


            //cout << "TEST 4" << endl;
            if(subject.empty())
            {
                cout << "ERROR ERROR ERROR1" << endl;
                bool success = send_response(client, "error no subject\n");
            }
            else if(length == NULL)


            {
                cout << "ERROR ERROR ERROR2" << endl;
                bool success = send_response(client, "error command\n");
            }
            else
            {
                message.setCommand(command);
                message.setUser(user);
                message.setSubject(subject);
                message.setLength(length);

                //if message is too long then get the rest of the message
                
                
                //cout << "CACHE ------ " << message.getCache().size() << endl;
                //cout << "length: " << message.getLength() << endl;
                if (message.getCache().size() < message.getLength())
                {
                    //cout << "TEST 6" << endl;
                    get_value(client,message);
                }

                
                if(myMap.put(message) == "OK\n")
                {
                    bool success = send_response(client, "OK\n");
                    if (!success)
                    {
                        cout << "ERROR ERROR ERROR4" << endl;
                        break;
                    }
                }
            }
        }
        else if(firstCommand == "list")
        {
            ss << fullCommand;
            ss >> command >> user;

            // cout << "LIST -------" << endl;
            // cout << "command: " << command << endl;
            // cout << "user: " << user << endl;


            if(user.empty())
            {
                bool success = send_response(client, "error invalid input\n");
            }
            // if(!extra.empty())
            // {
            //     subject = subject + " " + to_string(length);
            //     length = atoi(extra.c_str());
            // }

            message.setCommand(command);
            message.setUser(user);


            bool success = send_response(client, myMap.list(message));
            if (!success)
            {
                //cout << "ERROR ERROR 2" << endl;
                send_response(client, "error");
            }
            
        }
        else if(firstCommand == "get")
        {
            //cout << "GETTER" << endl;
            ss << request;
            ss >> command >> user >> index;

            message.setCommand(command);
            message.setUser(user);
            message.setIndex(atoi(index.c_str()));
            //cout << "GETTER1" << endl;
            

            if(myMap.get(message) == "error invalid get\n")
            {
                send_response(client, "error invalid get\n");
            }
            else
            {
                bool success = send_response(client, myMap.get(message));
                if (!success)
                {
                    //cout << "ERROR ERROR 1" << endl;
                    send_response(client, "error invalid input\n");
                }
            }


        }
        else if(firstCommand == "reset\n")
        {
            myMap.clear();
            bool success = send_response(client, "OK\n");
            if (!success)
            {
                cout << "ERROR ERROR ERROR5" << endl;
                send_response(client, "error invalid input\n");
            }
        }
        else if(firstCommand == "quit")
        {
            bool success = send_response(client, "quit");
            if (!success)
            {
                send_response(client, "error invalid input\n");
            }
        }
        else
        {
            cout << "ERROR ERROR" << endl;
            bool success = send_response(client, "error command\n");
        }

    }
    close(client);
    return true;
}

//**************************************************

void
Server::get_value(int client, Message& message) 
{
    char* buf_ = new char[buflen_+1];
    //cout << "MAYDAY1 !!!!!!!!!!!!!!!!!!!!!" << endl;
    string temp = "";
    while(message.getCache().size() < message.getLength())
    {
        //cout << "MAYDAY2" << endl;
        int nread = recv(client,buf_,1024,0);
        //cout << "MAYDAY3" << endl;
        if (nread < 0) 
        {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return;
        } 
        else if (nread == 0) 
        {
            // the socket is closed
            //cout << "socket is closed" << endl;
            return;
        }
        //cout << "MAYDAY5" << endl;
        temp = string(buf_,nread);
        message.setCache(message.getCache() + temp);
        //cout << "THE CACHE SIZE NOW:" << message.getCache().size() << endl;
        //cout << "THE LENGTH NOW:" << message.getLength() << endl;
        //cout << "0000000000000000000000000000" << endl;
        //cout << message.getCache() << endl;
    }
    message.setCache(message.getCache());
    //cout << "MAYDAY6" << endl;   
}

bool
Server::handle_message(int client, Message& message, string command) 
{
    if(command == "put")
    {
        string response = "OK\n";
        return send_response(client, response);
    }
    else if(command == "list")
    {
        string response = message.getListString();
        const char* ptr = response.c_str();
        return send(client, ptr, response.size(), 0);
    }
    return false;
  
}

bool
Server::send_response(int client, string response) {
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) 
    {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) 
        {
            if (errno == EINTR) 
            {
                // the socket call was interrupted -- try again
                continue;
            }
            else 
            {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } 
        else if (nwritten == 0) 
        {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

string
Server::get_request(int client) {
    string request = "";
    char* buf_ = new char[buflen_+1];
    // read until we get a newline
    while (request.find("\n") == string::npos) {
        int nread = recv(client,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {

            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        request.append(buf_,nread);
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    return request;
}
