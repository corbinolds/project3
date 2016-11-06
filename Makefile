#
# Makefile
# Project 4
#
#  Created by Phillip Romig on 4/3/12.
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

CXX = g++
LD = g++
CXXFLAGS = -g
LDFLAGS = -g 

#
# You should be able to add object files here without changing anything else
#
OBJ_FILES = simulator.o project3.o

project3: ${OBJ_FILES} 
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}

#
# Please remember not to submit objects or binarys.
#
clean:
	rm -f core project3 ${OBJ_FILES}

#
# This might work to create the submission tarball in the formal I asked for.
#
submit:
	rm -f core project1 ${OBJ_FILES}
	mkdir `whoami`
	cp Makefile README.txt *.h *.cc `whoami`
	tar cf `whoami`.tar `whoami`
