JP2A_SRCS = options.c terminal.c image.c html.c curl.c aspect_ratio.c
JP2A_OBJS = $(addprefix obj/jp2a/, $(JP2A_SRCS:.c=.o))

JP2A_PACKAGE_STRING = \"jp2a-termlingus\"
JP2A_PACKAGE_BUGREPORT = \"https://github.com/syntheit/termlingus/issues\"
JP2A_PACKAGE_URL = \"https://github.com/syntheit/termlingus\"
CXXFLAGS =  -D PACKAGE_STRING="$(JP2A_PACKAGE_STRING)"
CXXFLAGS += -D PACKAGE_BUGREPORT="$(JP2A_PACKAGE_BUGREPORT)"
CXXFLAGS += -D PACKAGE_URL="$(JP2A_PACKAGE_URL)"

termlingus: obj/main.o $(JP2A_OBJS)
	gcc -o termlingus obj/main.o $(JP2A_OBJS) -lcurl -ljpeg -lz -lncurses -lpng

obj/jp2a/%.o: lib/jp2a/src/%.c
	mkdir -p obj/jp2a
	touch lib/jp2a/include/config.h
	gcc $(CXXFLAGS) -c -o $@ $< -I lib/jp2a/include/ 

obj/main.o: src/main.c
	mkdir -p obj
	gcc $(CXXFLAGS) -I include/ -I lib/jp2a/include/ -c -o obj/main.o src/main.c

.PHONY: clean
clean:
	rm -rf obj
	rm -f termlingus
