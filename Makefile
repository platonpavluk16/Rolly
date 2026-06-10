CC := gcc

CFLAGS := -Wall -Wextra -std=c11
INCLUDES := -Isrc/glad/include

LDLIBS := -lglfw -lGL -ldl -lm

SRCS := src/main.c src/glad/src/glad.c
OBJS := $(SRCS:.c=.o)

TARGET := app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean