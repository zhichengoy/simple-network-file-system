#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "FileSys.h"
using namespace std;

int main(int argc, char* argv[]) {
	const int BACKLOG = 5;
	int sockfd, newsockfd, port, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	if (argc < 2) {
		cout << "Usage: ./nfsserver port#\n";
      return -1;
    }
    port = atoi(argv[1]);
		cout << "Connecting to port " << port << endl;

		// create socket
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
    	perror("ERROR opening socket");
			exit(0);
		}

		// bind socket to address and port number
		bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    	perror("ERROR on bind");
			exit(0);
		}

		// listen for client to make a connection
		if (::listen(sockfd, BACKLOG) < 0){
			perror("ERROR on list");
			exit(0);
		}

		// accept a request from client
		newsockfd = ::accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
		if (newsockfd < 0) {
			perror("ERROR on accept");
			exit(0);
		}
		bzero(buffer, 256);

		cout << "SOCK: " << sockfd << endl;
		cout << "NEW SOCKET: " << newsockfd << endl;

    // mount the file system
    FileSys fs;
    fs.mount(newsockfd); //assume that sock is the new socket created
                    //for a TCP connection between the client and the server.

    //loop: get the command from the client and invoke the file
    //system operation which returns the results or error messages back to the clinet
    //until the client closes the TCP connection.


    //close the listening socket

    //unmout the file system
    fs.unmount();

    return 0;
}
