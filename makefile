RAYLIBLINKS = -lraylib -lGL -lpthread -lm -ldl
ENABLEWARNING = -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-result -Wno-unused-label -Wno-unused-local-typedefs
NOSTANDARDLIB = 
main: main.c
	gcc main.c -o main $(RAYLIBLINKS) $(ENABLEWARNING) $(NOSTANDARDLIB)

run: main
	./main