CC = gcc
CFLAGS = -Wall -std=c99
CPPFLAGS =

ifeq ($(block_len),)
export block_len = 4801
endif

ifeq ($(block_wgt),)
export block_wgt = 45
endif

suffix = $(block_len)_$(block_wgt)
CPPFLAGS += -DBLOCK_LENGTH=$(block_len) -DBLOCK_WEIGHT=$(block_wgt)

ifeq ($(verbose),1)
CPPFLAGS += -DVERBOSE
endif

ifdef max_iter
CPPFLAGS += -DMAX_ITER=$(max_iter)
endif


ifeq ($(debug),1)
CFLAGS += -g -pg
else
CFLAGS += -O4
endif

all: qcmdpc$(suffix)

qcmdpc$(suffix): lib$(suffix).o qcmdpc$(suffix).o attack$(suffix).o reconstruct$(suffix).o  observe$(suffix).o main$(suffix).o graphs$(suffix).o  misc.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

%$(suffix).o: %.c qcmdpc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

main$(suffix).o decrypt$(suffix).o: decrypt.h attack.h
decrypt$(suffix).o: misc.h

misc.o: misc.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean%:
	- /bin/rm *$*.o qcmdpc$*

clean:
	- /bin/rm *.o

veryclean: clean
	- /bin/rm qcmdpc[0-9]*_*[0-9]
