# Dependency rules for non-file targets
all: testFT
clobber: clean
	rm -f *~ \#*\#
clean:
	rm -f testsymtablelist *.o

# Dependency rules for file targets
testFT: ft_client.o ft.o dynarray.o path.o nodeFT.o
	gcc217 ft_client.o ft.o dynarray.o path.o nodeFT.o -o testFT
ft_client.o: ft_client.c ft.h a4def.h dynarray.h path.h
	gcc217 -c ft_client.c
ft.o: ft.c ft.h a4def.h dynarray.h path.h
	gcc217 -c ft.c
dynarray.o: dynarray.c dynarray.h
	gcc217 -c dynarray.c
path.o: path.c path.h
	gcc217 -c path.c
nodeFT.o: nodeFT.c nodeFT.h
	gcc217 -c nodeFT.c