#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall -g -std=c++14

# define any directories containing header files other than /usr/include
#
INCLUDES = -I../Box2D-71a6079/Box2D/ -I../rttr/src/ -I../SFML-2.4.2_SRC/include/
# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:

#   if I wa# define any libraries to link into executable:
nt to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:

SUBDIRS := $(wildcard */.)  # e.g. "foo/. bar/."

# foo/.all bar/.all foo/.clean bar/.clean
SUBDIRS_TARGETS := \
    $(foreach t,$(TARGETS),$(addsuffix $t,$(SUBDIRS)))


#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: $(TARGETS) $(SUBDIRS_TARGETS)

# static pattern rule, expands into:
# all clean : % : foo/.% bar/.%
$(TARGETS) : % : $(addsuffix %,$(SUBDIRS))
	@echo 'Done "$*" target'

# here, for foo/.all:
#   $(@D) is foo
#   $(@F) is .all, with leading period
#   $(@F:.%=%) is just all
$(SUBDIRS_TARGETS) :
	$(MAKE) -C $(@D) $(@F:.%=%)


# DO NOT DELETE THIS LINE -- make depend needs it
