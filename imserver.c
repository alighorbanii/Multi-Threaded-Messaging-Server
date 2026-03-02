#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <pthread.h>

#define MAXWORDS 10000
#define MAXLEN   1000
#define MAXCLIENTS 100

char usernames[MAXCLIENTS][MAXLEN];
FILE *clientfps[MAXCLIENTS];  
int numClients = 0;

void *server_child(void *ptr) {
    int clientIndex = (int)(long)ptr;

        char serverfifo[MAXLEN];
	    sprintf(serverfifo, "/tmp/%s-s-%lu", getenv("USER"),
	                (unsigned long)pthread_self());
			    mkfifo(serverfifo, 0600);
			        chmod(serverfifo, 0622);

				    fprintf(clientfps[clientIndex], "%s\n", serverfifo);
				        fflush(clientfps[clientIndex]);

					    FILE *serverfp = fopen(serverfifo, "r");
					        if (!serverfp) {
						        perror("open serverfifo for reading");
							        unlink(serverfifo);
								        return NULL;
									    }

									        char line[MAXLEN];

										    while (fgets(line, MAXLEN, serverfp)) {
										            size_t len = strlen(line);
											            if (len > 0 && line[len - 1] == '\n')
												                line[len - 1] = '\0';

														        if (strcmp(line, "list") == 0) {
															            char out[MAXLEN];
																                out[0] = '\0';
																		            for (int i = 0; i < numClients; i++) {
																			                    if (i > 0) strcat(out, " ");
																					                    strcat(out, usernames[i]);
																							                }
																									            strcat(out, "\n");
																										                fprintf(clientfps[clientIndex], "%s", out);
																												            fflush(clientfps[clientIndex]);
																													            }
																														            else if (strncmp(line, "send ", 5) == 0) {
																															                char *p = line + 5;

																																	            while (*p == ' ') p++;
																																		                char target[MAXLEN];
																																				            int tlen = 0;
																																					                while (*p && *p != ' ') {
																																							                if (tlen < MAXLEN - 1)
																																									                    target[tlen++] = *p;
																																											                    p++;
																																													                }
																																															            target[tlen] = '\0';

																																																                while (*p == ' ') p++;
																																																		            char *message = p;   

																																																			                int tIndex = -1;
																																																					            for (int i = 0; i < numClients; i++) {
																																																						                    if (strcmp(usernames[i], target) == 0) {
																																																								                        tIndex = i;
																																																											                    break;
																																																													                    }
																																																															                }

																																																																	            if (tIndex == -1) {
																																																																		                    fprintf(clientfps[clientIndex],
																																																																				                            "Sorry, %s has not joined yet.\n", target);
																																																																							                    fflush(clientfps[clientIndex]);
																																																																									                } else {
																																																																											                char out[MAXLEN];
																																																																													                snprintf(out, MAXLEN, "%s says %s\n",
																																																																															                         usernames[clientIndex], message);
																																																																																		                 fprintf(clientfps[tIndex], "%s", out);
																																																																																				                 fflush(clientfps[tIndex]);
																																																																																						                 fprintf(clientfps[clientIndex], "Message sent!\n");
																																																																																								                 fflush(clientfps[clientIndex]);
																																																																																										             }
																																																																																											             }
																																																																																												         }

																																																																																													     fclose(serverfp);
																																																																																													         unlink(serverfifo);

																																																																																														     return NULL;
																																																																																														     }

																																																																																														     int main(int argc, char *argv[]) {
																																																																																														         char line[MAXLEN];

																																																																																															     if (argc != 2) {
																																																																																															             fputs("Usage: imserver <server-fifo>\n", stderr);
																																																																																																             exit(1);
																																																																																																	         }

																																																																																																		     char filename[MAXLEN];
																																																																																																		         strncpy(filename, argv[1], MAXLEN - 1);
																																																																																																			     filename[MAXLEN - 1] = '\0';

																																																																																																			         mkfifo(filename, 0600);
																																																																																																				     chmod(filename, 0622);

																																																																																																				         printf("Connect to %s to use IM!\n", filename);

																																																																																																					     while (1) {
																																																																																																					             FILE *fp = fopen(filename, "r");
																																																																																																						             if (!fp) {
																																																																																																							                 printf("FIFO %s cannot be opened for reading.\n", filename);
																																																																																																									             exit(2);
																																																																																																										             }

																																																																																																											             while (fgets(line, MAXLEN, fp)) {
																																																																																																												                 char *clientfifo = strtok(line, " \n");
																																																																																																														             char *userName   = strtok(NULL, " \n");
																																																																																																															                 if (!clientfifo || !userName) {
																																																																																																																	                 continue;
																																																																																																																			             }

																																																																																																																				                 if (numClients >= MAXCLIENTS) {
																																																																																																																						                 continue;   
																																																																																																																								             }

																																																																																																																									                 clientfps[numClients] = fopen(clientfifo, "w");
																																																																																																																											             if (!clientfps[numClients]) {
																																																																																																																												                     continue;
																																																																																																																														                 }

																																																																																																																																             strncpy(usernames[numClients], userName, MAXLEN - 1);
																																																																																																																																	                 usernames[numClients][MAXLEN - 1] = '\0';

																																																																																																																																			             printf("%s joined!\n", usernames[numClients]);
																																																																																																																																				                 fflush(stdout);

																																																																																																																																						             pthread_t tid;
																																																																																																																																							                 pthread_create(&tid, NULL, server_child,
																																																																																																																																									                            (void *)(long)numClients);
																																																																																																																																												                pthread_detach(tid);

																																																																																																																																														            numClients++;
																																																																																																																																															            }
																																																																																																																																																            fclose(fp);
																																																																																																																																																	        }

																																																																																																																																																		    return 0;
																																																																																																																																																		    }
