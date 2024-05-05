RAYLIBLINKS = -lraylib -lGL -lpthread -lm -ldl

main: main.c
	gcc main.c -o main $(RAYLIBLINKS)

run: main
	./main