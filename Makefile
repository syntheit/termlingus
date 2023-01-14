termlingus: obj/main.o
	g++ -o termlingus obj/main.o

obj/main.o: src/main.cpp
	mkdir -p obj
	g++ -I include/ -c -o obj/main.o src/main.cpp

.PHONY: clean
clean:
	rm -rf obj
	rm -f termlingus
