// serv.c
#include <stdbool.h>          // For bool type
#include <netinet/in.h>       // Structure for storing address information
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>       // For socket APIs
#include <sys/types.h>
#include <unistd.h>           // For the close() function
#include "list.h"

#define PORT 9001
#define ACK "ACK"

// Function prototype for converting list to string
// Implemented in list.c
int list_to_string(list_t *list, char *buffer, size_t buffer_size);

int main(int argc, char const* argv[]) 
{ 
    (void)argc; // Mark argc as unused
    (void)argv; // Mark argv as unused

    int n, val, idx;
    
    // Create server socket
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Server socket created successfully.\n");
  
    // Define server address 
    struct sockaddr_in servAddr; 
    memset(&servAddr, 0, sizeof(servAddr)); // Zero out the structure
  
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 
  
    // Bind socket to the specified IP and port 
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }
    printf("Socket bound to port %d.\n", PORT);
  
    // Listen for connections 
    if (listen(servSockD, 5) < 0) { // Increased backlog to 5
        perror("Listen failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening for connections...\n");
  
    // Accept a client connection
    int clientSocket = accept(servSockD, NULL, NULL); 
    if (clientSocket < 0) {
        perror("Accept failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }
    printf("Client connected successfully.\n");

    // Initialize the list
    list_t *mylist = list_alloc();  // Create the list
    if (mylist == NULL) {
        fprintf(stderr, "Failed to allocate memory for the list.\n");
        close(clientSocket);
        close(servSockD);
        exit(EXIT_FAILURE);
    }
    printf("List initialized.\n");
  
    // Buffers for receiving and sending data
    char buf[1024];
    char sbuf[1024];
    char* token;
  
    while(1) {
        // Receive messages from client socket
        n = recv(clientSocket, buf, sizeof(buf) - 1, 0); // Leave space for null terminator
        if (n < 0) {
            perror("Receive failed");
            break;
        } else if (n == 0) {
            printf("Client disconnected.\n");
            break;
        }

        buf[n] = '\0';  // Null-terminate the received string
        printf("Received command: %s\n", buf); // Log received command

        // Tokenize the received command
        token = strtok(buf, " ");

        // Clear the response buffer before each response
        memset(sbuf, 0, sizeof(sbuf));

        if (token == NULL) {
            snprintf(sbuf, sizeof(sbuf), "Invalid command");
        }
        else if (strcmp(token, "exit") == 0) {
            snprintf(sbuf, sizeof(sbuf), "Server shutting down.");
            send(clientSocket, sbuf, strlen(sbuf), 0);
            printf("Exit command received. Shutting down server.\n");
            break; // Exit the loop to shutdown
        } 
        else if (strcmp(token, "get_length") == 0) {
            val = list_length(mylist);
            snprintf(sbuf, sizeof(sbuf), "Length = %d", val);
        } 
        else if (strcmp(token, "add_front") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                val = atoi(token);
                list_add_to_front(mylist, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            } else {
                snprintf(sbuf, sizeof(sbuf), "Invalid input for add_front");
            }
        } 
        else if (strcmp(token, "add_back") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                val = atoi(token);
                list_add_to_back(mylist, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            } else {
                snprintf(sbuf, sizeof(sbuf), "Invalid input for add_back");
            }
        } 
        else if (strcmp(token, "remove_front") == 0) {
            val = list_remove_from_front(mylist);
            if (val != -1) { // Assuming -1 indicates failure
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            } else {
                snprintf(sbuf, sizeof(sbuf), "List is empty. Cannot remove front.");
            }
        } 
        else if (strcmp(token, "remove_back") == 0) {
            val = list_remove_from_back(mylist);
            if (val != -1) { // Assuming -1 indicates failure
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            } else {
                snprintf(sbuf, sizeof(sbuf), "List is empty. Cannot remove back.");
            }
        } 
        else if (strcmp(token, "remove_at_index") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                idx = atoi(token);
                val = list_remove_at_index(mylist, idx);
                if (val != -1) { // Assuming -1 indicates failure
                    snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
                } else {
                    snprintf(sbuf, sizeof(sbuf), "Invalid index or list is empty.");
                }
            } else {
                snprintf(sbuf, sizeof(sbuf), "Invalid input for remove_at_index");
            }
        } 
        else if (strcmp(token, "is_in") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                val = atoi(token);
                bool is_in = list_is_in(mylist, val);
                snprintf(sbuf, sizeof(sbuf), "Is in list = %d", is_in);
            } else {
                snprintf(sbuf, sizeof(sbuf), "Invalid input for is_in");
            }
        } 
        else if (strcmp(token, "get_elem_at") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                idx = atoi(token);
                val = list_get_elem_at(mylist, idx);
                if (val != -1) { // Assuming -1 indicates failure
                    snprintf(sbuf, sizeof(sbuf), "Elem at index = %d", val);
                } else {
                    snprintf(sbuf, sizeof(sbuf), "Invalid index.");
                }
            } else {
                snprintf(sbuf, sizeof(sbuf), "Invalid input for get_elem_at");
            }
        } 
        else if (strcmp(token, "get_index_of") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                val = atoi(token);
                idx = list_get_index_of(mylist, val);
                if (idx != -1) { // Assuming -1 indicates not found
                    snprintf(sbuf, sizeof(sbuf), "Index of elem = %d", idx);
                } else {
                    snprintf(sbuf, sizeof(sbuf), "Element not found in list.");
                }
            } else {
                snprintf(sbuf, sizeof(sbuf), "Invalid input for get_index_of");
            }
        }
        else if (strcmp(token, "print") == 0) {
            // Handle the 'print' command
            if (list_to_string(mylist, sbuf, sizeof(sbuf)) == 0) {
                // Successfully converted list to string
                // Prepend a descriptive message
                char temp[1024];
                snprintf(temp, sizeof(temp), "List Contents: %s", sbuf);
                strncpy(sbuf, temp, sizeof(sbuf));
                sbuf[sizeof(sbuf) - 1] = '\0'; // Ensure null termination
            } else {
                snprintf(sbuf, sizeof(sbuf), "Failed to retrieve list contents.");
            }
        }
        else {
            snprintf(sbuf, sizeof(sbuf), "Invalid operation");
        }

        // Send message back to client, ensure sending the correct string length
        if (send(clientSocket, sbuf, strlen(sbuf), 0) < 0) {
            perror("Send failed");
            break;
        }
        printf("Sent response: %s\n", sbuf); // Log sent response
    }

    // Cleanup resources
    list_free(mylist);          // Free the list memory
    close(clientSocket);        // Close the client socket
    close(servSockD);           // Close the server socket
    printf("Server shut down gracefully.\n");
    return 0;
}





