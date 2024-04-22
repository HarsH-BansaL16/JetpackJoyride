CC := g++
CFLAGS := -std=c++11
INCLUDE := -Idependencies/include
LIB_DIR := -Ldependencies/library
LIBS := -lglfw.3.3
SRCS := app.cpp glad.c
OUT := app

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIB_DIR) $(LIBS)

clean:
	rm -f $(OUT)
