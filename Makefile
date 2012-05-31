export CCFLAGS=clang -g -Wall 
OBJ=splaytree.o bstree.o

all: $(OBJ)

bstree.o:bstree.h include/bst.h \
		include/data.h include/treenode.h
	$(CCFLAGS) -c bstree.c
splaytree.o:splaytree.h include/bst.h \
		include/data.h include/treenode.h
	$(CCFLAGS) -c splaytree.c
	cd test && make
clean:
		rm *.o && cd test && rm *.o test
