# EDM Makefile
# Copyright (C) 2016 Yan Li <yanli@ascar.io>
#
# Based on Redis Makefile
# Copyright (C) 2009 Salvatore Sanfilippo <antirez at gmail dot com>
# This file is released under the BSD license, see the LICENSE file
#
# Dependencies are stored in the Makefile.dep file. To rebuild this file
# Just use 'make dep', but this is only needed by developers.

OPTIMIZATION?=-O2

# Default settings
STD=-std=c++0x -pedantic
WARN=-Wall -W -Werror
OPT=$(OPTIMIZATION)

# Override default settings if possible
-include .make-settings

FINAL_CFLAGS=$(STD) $(WARN) $(OPT) $(DEBUG) $(CFLAGS)
FINAL_LDFLAGS=$(LDFLAGS) $(DEBUG)
FINAL_LIBS=-lm
DEBUG=-g -ggdb -fsanitize=address

MY_CXX=$(QUIET_CXX)$(CXX) $(FINAL_CFLAGS)
MY_LD=$(QUIET_LINK)$(CXX) $(FINAL_LDFLAGS)
MY_INSTALL=$(QUIET_INSTALL)$(INSTALL)

CCCOLOR="\033[34m"
LINKCOLOR="\033[34;1m"
SRCCOLOR="\033[33m"
BINCOLOR="\033[37;1m"
MAKECOLOR="\033[32;1m"
ENDCOLOR="\033[0m"

ifndef V
QUIET_CC = @printf '    %b %b\n' $(CCCOLOR)CC$(ENDCOLOR) $(SRCCOLOR)$@$(ENDCOLOR) 1>&2;
QUIET_CXX = @printf '    %b %b\n' $(CCCOLOR)CXX$(ENDCOLOR) $(SRCCOLOR)$@$(ENDCOLOR) 1>&2;
QUIET_LINK = @printf '    %b %b\n' $(LINKCOLOR)LINK$(ENDCOLOR) $(BINCOLOR)$@$(ENDCOLOR) 1>&2;
QUIET_INSTALL = @printf '    %b %b\n' $(LINKCOLOR)INSTALL$(ENDCOLOR) $(BINCOLOR)$@$(ENDCOLOR) 1>&2;
endif

all: edm-test
	@echo ""
	@echo "Hint: It's a good idea to run 'make test' ;)"
	@echo ""

.PHONY: all

edm-test: IntervalTree.o edm-test.o
	$(MY_LD) -o $@ $^ $(FINAL_LIBS)

# Deps (use make dep to generate this)
include Makefile.dep

dep:
	$(MY_CXX) -MM *.cpp > Makefile.dep

.PHONY: dep

persist-settings: distclean
	echo STD=$(STD) >> .make-settings
	echo WARN=$(WARN) >> .make-settings
	echo OPT=$(OPT) >> .make-settings
	echo MALLOC=$(MALLOC) >> .make-settings
	echo CFLAGS=$(CFLAGS) >> .make-settings
	echo LDFLAGS=$(LDFLAGS) >> .make-settings
	echo PREV_FINAL_CFLAGS=$(FINAL_CFLAGS) >> .make-settings
	echo PREV_FINAL_LDFLAGS=$(FINAL_LDFLAGS) >> .make-settings

.PHONY: persist-settings

# Prerequisites target
.make-prerequisites:
	@touch $@

# Clean everything, persist settings and build dependencies if anything changed
ifneq ($(strip $(PREV_FINAL_CFLAGS)), $(strip $(FINAL_CFLAGS)))
.make-prerequisites: persist-settings
endif

ifneq ($(strip $(PREV_FINAL_LDFLAGS)), $(strip $(FINAL_LDFLAGS)))
.make-prerequisites: persist-settings
endif

%.o: %.cpp .make-prerequisites
	$(MY_CXX) -c $<

clean:
	rm -rf edm-test *.o

.PHONY: clean

distclean: clean
	-(rm -f .make-*)

.PHONY: distclean

test: edm-test
	@./edm-test small_size_sample_sets.csv
	@./edm-test large_size_sample_sets.csv

.PHONY: test

check: test

.PHONY: check
