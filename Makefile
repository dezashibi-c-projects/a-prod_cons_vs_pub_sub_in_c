# ***************************************************************************************
#    Project: Understanding Producer-Consumer and Publish-Subscribe Patterns in C
#    File: Makefile
#    Date: 2024-08-08
#    Author: Navid Dezashibi
#    Contact: navid@dezashibi.com
#    Website: https://www.dezashibi.com | https://github.com/dezashibi
#    License:
#     Please refer to the LICENSE file in the repository or website for more information
#     about the licensing of this work. If you have any questions or concerns,
#     please feel free to contact me at the email address provided above.
# ***************************************************************************************
# *  Description: Read the readme file for more information
# ***************************************************************************************

# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -DDTHREAD_DEBUG -Wall -g -O0
TARGET_NAME1 = producer_consumer
TARGET_NAME2 = publisher_subscriber

# Platform Specific Settings
ifeq ($(OS),Windows_NT)
	TARGET_EXT = .exe
	CFLAGS +=
else
	TARGET_EXT =
	CFLAGS += -lpthread
endif

BUILD = $(CC) $(CFLAGS)

# Executable name
TARGET1 = $(TARGET_NAME1)$(TARGET_EXT)
TARGET2 = $(TARGET_NAME2)$(TARGET_EXT)

# Source files
SRC1 = producer_consumer.c
SRC2 = publisher_subscriber.c

# Default target
all: prod_cons pub_sub

# Run the targets
run1: prod_cons
	./$(TARGET1)

run2: pub_sub
	./$(TARGET2)

# Rule to link the object files and create the executable (debug)
prod_cons: $(SRC1)
	$(BUILD) $(SRC1) -o $(TARGET1)

pub_sub: $(SRC2)
	$(BUILD) $(SRC2) -o $(TARGET2)

# Clean up
clean:
	rm -f *.pdb *.o $(TARGET1) $(TARGET2)

# Phony targets
.PHONY: all run1 run2 clean