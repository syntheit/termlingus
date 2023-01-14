termlingus: obj/main.o obj/ytdl.o
	g++ -o termlingus obj/main.o obj/ytdl.o

obj/ytdl.o: src/ytdl.cpp
	mkdir -p obj
	g++ -I include/ -c -o obj/ytdl.o src/ytdl.cpp

obj/main.o: src/main.cpp
	mkdir -p obj
	g++ -I include/ -c -o obj/main.o src/main.cpp

.PHONY: clean
clean:
	rm -rf obj
	rm -f termlingus
