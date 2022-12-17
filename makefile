all: data_helpers.o task_helpers.o gol_data gol_task

data_helpers.c: 
	gcc -std=c99 -Wall -pedantic data_helpers.o -o data_helpers
	
data_helpers.o: data_helpers.c data_helpers.h
	gcc -std=c99 -Wall -pedantic -c data_helpers.c -o data_helpers.o

task_helpers.c:
	gcc -std=c99 -Wall -pedantic task_helpers.o -o task_helpers
	
task_helpers.o: task_helpers.c task_helpers.h
	gcc -std=c99 -Wall -pedantic -c task_helpers.c -o task_helpers.o

gol_data: gol_data.o data_helpers.o
	gcc -std=c99 -Wall -pedantic gol_data.o data_helpers.o -lpthread -o gol_data

gol_data.o: gol_data.c data_helpers.h
	gcc -std=c99 -Wall -pedantic -c gol_data.c -o gol_data.o 

gol_task: gol_task.o task_helpers.o
	gcc -std=c99 -Wall -pedantic gol_task.o task_helpers.o -lpthread -o gol_task

gol_task.o: gol_task.c task_helpers.h
	gcc -std=c99 -Wall -pedantic -c gol_task.c -o gol_task.o

clean:
	rm -f *.o  data_helpers.o task_helpers.o gol_data gol_task
