CC=gcc
CFLAGS=-Wall -Wextra -O2
LDFLAGS=-pthread

all: imserver imclient

imserver: imserver.c
	$(CC) $(CFLAGS) -o imserver imserver.c $(LDFLAGS)

imclient: imclient.c
	$(CC) $(CFLAGS) -o imclient imclient.c $(LDFLAGS)

clean:
	rm -f imserver imclient *.o
