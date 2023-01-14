CXXFLAGS = -O3

termlingus: obj/main.o obj/ytdl.o
	g++ -o termlingus obj/main.o obj/ytdl.o $(CXXFLAGS)

obj/ytdl.o: src/ytdl.cpp
	mkdir -p obj
	g++ -I include/ -c -o obj/ytdl.o src/ytdl.cpp $(CXXFLAGS)

obj/main.o: src/main.cpp
	mkdir -p obj
	g++ -I include/ -c -o obj/main.o src/main.cpp $(CXXFLAGS)

.PHONY: clean
clean:
	rm -rf obj
	rm -f termlingus
