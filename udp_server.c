#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
    
#define PORT 8080
#define BUFF_SIZE 1024 
     
int main(int argc, char* argv[]) { 
    int port = -1;

    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            printf("USAGE: ./udp_server [-p <LOCAL_PORT>]\n");
            exit(EXIT_SUCCESS);

        } else if(!strcmp(argv[i], "-p")) {
            i++;
            port = atoi(argv[i]);
        
        } else {
            printf("USAGE: ./udp_server [-h] [-p <LOCAL_PORT>]\n");
            exit(EXIT_FAILURE);
        }
    }

    if(port == -1) {
        port = PORT;
    }

    int sockfd;
    char input_buffer[BUFF_SIZE];
    char output_buffer[BUFF_SIZE];

    struct sockaddr_in servaddr, cliaddr; 
        
    // Creating socket file descriptor 
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
        
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; // Listen to all networks
    servaddr.sin_port = htons(port); // Sets required port
        
    // Bind the socket with the server address 
    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    int len, n; 
    
    len = sizeof(cliaddr);  //len is value/result 
    
    while(true) {
        n = recvfrom(sockfd, (char*)input_buffer, BUFF_SIZE,  
                    MSG_WAITALL, (struct sockaddr*)&cliaddr, 
                    &len); 
        input_buffer[n] = '\0'; 
        printf("receive from client: \"%s\"\n", input_buffer); 
        
        int out_len = snprintf(output_buffer, BUFF_SIZE, "server got: \"%s\"", input_buffer);
        output_buffer[out_len] = '\0';
        sendto(sockfd, (const char*)output_buffer, strlen(output_buffer),  
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
                len); 
        printf("sent to client: \"%s\"\n", output_buffer);  
    }

    return EXIT_SUCCESS; 
}
