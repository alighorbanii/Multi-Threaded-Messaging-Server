#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>

#define MAXLEN 1000

int main(int argc, char *argv[]) {
    if (argc != 3) {
            fputs("Usage: imclient <server-fifo> <user-id>\n", stderr);
	            exit(1);
		        }

			    char clientfifo[MAXLEN];
			        sprintf(clientfifo, "/tmp/%s-%d", getenv("USER"), getpid());
				    mkfifo(clientfifo, 0600);
				        chmod(clientfifo, 0622);

					    FILE *fp = fopen(argv[1], "w");
					        if (!fp) {
						        perror("open server fifo");
							        unlink(clientfifo);
								        exit(1);
									    }

									        fprintf(fp, "%s %s\n", clientfifo, argv[2]);
										    fclose(fp);

										        FILE *clientfp = fopen(clientfifo, "r");
											    if (!clientfp) {
											            perror("open clientfifo for reading");
												            unlink(clientfifo);
													            exit(1);
														        }

															    char serverfifo[MAXLEN];
															        fscanf(clientfp, "%s", serverfifo);     
																    char line[MAXLEN];
																        fgets(line, MAXLEN, clientfp);         

																	    FILE *serverfp = fopen(serverfifo, "w");
																	        if (!serverfp) {
																		        perror("open serverfifo for writing");
																			        fclose(clientfp);
																				        unlink(clientfifo);
																					        exit(1);
																						    }

																						        puts("Keep writing your inputs... (use 'list' or 'send <user> <message>')");

																							    int childPid;
																							        if (!(childPid = fork())) {
																								        while (1) {
																									            if (!fgets(line, MAXLEN, stdin))
																										                    break;

																												                fprintf(serverfp, "%s", line);
																														            fflush(serverfp);
																															            }
																																        } else {
																																	        while (1) {
																																		            if (!fgets(line, MAXLEN, clientfp))
																																			                    break;
																																					                printf("%s", line);
																																							        }
																																								        kill(childPid, SIGKILL);
																																									    }

																																									        fclose(clientfp);
																																										    fclose(serverfp);
																																										        unlink(clientfifo);

																																											    return 0;
																																											    }
