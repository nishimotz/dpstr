# makefile for 'dpstr'
# 

#CC      = cc -Wall -g -mno-cygwin
CC      = cc -Wall
CPATH   =
CFLAGS	=
TARGET  = dpstr.linux
#TARGET  = dpstr
SRC	= main.c dpstr.c dpmatch.c estimate.c distance.c \
	  disp.c
OBJS	= $(SRC:.c=.o)

#LIBS = 	-L/usr/X11R6/lib -lglut -lX11 -lm
LIBS = -lm

# rules

.SUFFIXES: .o .c .h

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

# targets

$(TARGET): $(OBJS) Makefile
	$(CC) $(OBJS) $(DEBUG) $(LIBS) -o $(TARGET)

clean :
	\rm -rf *~
	\rm -rf *.o
	\rm -rf $(TARGET)


# depends

disp.o : disp.h
dpstr.o : dpstr.h
dpmatch.o : dpstr.h
estimate.o : dpstr.h
distance.o : dpstr.h

# End of makefile


