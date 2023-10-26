INC=-l/opt/homebrew/include
LINK=-L/opt/homebrew/lib
build:
	gcc -Wall -std=c99 ./src/*.c -o renderer -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lm
run:
	./renderer
clean:
	rm renderer
