#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 12000

int main(){
    int sockfd, n;
    socklen_t len;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;

    // Create a UDP socket
    // Notice the use of SOCK_DGRAM for UDP packets
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Fill server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
    servaddr.sin_port = htons(PORT); // port number

    clock_t start, end;
    double duration;
    //timeout will be measured in milliseconds, so 1s
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    //Set timeout to 1 second
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));

    //Could have sent the empty buffer but figured an actual message would be more fun
    const char *message = "ping";

    for(int i = 0; i < 10; i++){

        //Log start time
        start = clock();

        //Send packet to server
        n = sendto(sockfd, (char *) message, strlen(message), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr));

        //Make sure the message sending does not incur an error
        if(n < 0) std::cout << "Message could not send" << std::endl;

        //Receive response from server
        //If the packet is not received in one second, time out and the function returns -1
        //If recvfrom returns -1, we assume the packet was lost
        //Otherwise, the packet was received so we track the round trip time
        if (recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, ( struct sockaddr *) &servaddr, &len) < 0){
            std::cout << "Packet " << (i+1) << " was lost." << std::endl;
        }
        else{
            //get end time
            end = clock();

            //calculate duration
            duration = (double) (end - start)/CLOCKS_PER_SEC;

            std::cout << "Round trip time of packet " << (i+1) << " is " << duration << " seconds." << std::endl;
        }
    }
    //close the socket
    close(sockfd);
}
