CC = g++
CFLAGS = -O3 -std=c++17 -Wall -Wextra -pedantic -Wfatal-errors -fPIC -Iextern
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

SRCS = $(wildcard src/*.cc)
OBJS := $(patsubst src/%.cc, %.o, $(SRCS))

TARGET = MUL_codec

all: $(TARGET)

MUL_codec: $(OBJS)
	$(CC) -Lextern/libppm -Lextern/libdivsufsort -lm -ldivsufsort -lppm -Wl,-rpath=extern/libppm,-rpath=extern/libdivsufsort $^ -o $@

%.o: src/%.cc
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

-include $(patsubst %,%.d,$(basename $(notdir $(SRCS))))

.PHONY: clean doc zip

clean:
	rm -rf *.o *.Td *.d $(TARGET) *.zip

doc:
	cd doc && pdflatex -halt-on-error README.tex; cd ..
	cd doc && pdflatex -halt-on-error prezentace.tex; cd ..

zip: MUL_codec doc
	zip -r xdlaba02.zip src extern doc/README.pdf Makefile
