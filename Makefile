NAME := bitmap

CC := cc
CFLAGS := -Wall -Wextra
CINCLUDES := -I./includes
CLIBRARIES := -lm

DIST := dist
SRCS := $(wildcard src/*.c)

build:
	mkdir -p $(DIST)

	$(CC) $(CFLAGS) $(CINCLUDES) $(SRCS) $(CLIBRARIES)  -o $(DIST)/$(NAME)
