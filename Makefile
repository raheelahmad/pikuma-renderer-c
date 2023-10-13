INC=-l/opt/homebrew/include
LINK=-L/opt/homebrew/lib
# compile:
# 	clang -c src/main.c -o main -lSDL2
build:
	gcc -Wall -std=c99 ./src/*.c -o renderer -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2
run:
	./renderer
clean:
	rm renderer
