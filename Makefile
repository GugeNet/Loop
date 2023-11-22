# Project Name
TARGET = Loop

# Sources
CPP_SOURCES = \
Loop.cpp \
button.cpp \
dualLedButton.cpp \
track.cpp

# Library Locations
LIBDAISY_DIR = ../DaisyExamples/libDaisy
DAISYSP_DIR = ../DaisyExamples/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

