exec = cats
sources=$(wildcard src/*.c)
objects=$(sources:.c=.o)
flags = -g # -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec) icon.res

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-del src/*.o