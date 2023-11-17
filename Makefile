CIPH_BIN = ciph
DECIPH_BIN = deciph

CIPH_SRC = aes_ecb_ciph.c
DECIPH_SRC = aes_ecb_deciph.c

CC = gcc

FLAGS = -Wall -Ofast -lpthread

EXAMPLE_KEY = "rg5;P}H23Nkh?{&g"

EXAMPLE_CIPH_INPUT1 = gato.jpg
CIPH_FILE1 = gato_ciph.jpg
EXAMPLE_DECIPH_OUTPUT1 = gato_deciph.jpg

EXAMPLE_CIPH_INPUT2 = avestruz.mp4
CIPH_FILE2 = avestruz_ciph.mp4
EXAMPLE_DECIPH_OUTPUT2 = avestruz_deciph.mp4

all:
	$(CC) $(FLAGS) src/impl/$(CIPH_SRC) -o bin/$(CIPH_BIN)
	$(CC) $(FLAGS) src/impl/$(DECIPH_SRC) -o bin/$(DECIPH_BIN)

ex1_ciph:
	bin/$(CIPH_BIN) examples/input/$(EXAMPLE_CIPH_INPUT1) examples/ciph/$(CIPH_FILE1) $(EXAMPLE_KEY)

ex1_deciph:
	bin/$(DECIPH_BIN) examples/ciph/$(CIPH_FILE1) examples/output/$(EXAMPLE_DECIPH_OUTPUT1) $(EXAMPLE_KEY)

ex2_ciph:
	bin/$(CIPH_BIN) examples/input/$(EXAMPLE_CIPH_INPUT2) examples/ciph/$(CIPH_FILE2) $(EXAMPLE_KEY)

ex2_deciph:
	bin/$(DECIPH_BIN) examples/ciph/$(CIPH_FILE2) examples/output/$(EXAMPLE_DECIPH_OUTPUT2) $(EXAMPLE_KEY)

clean:
	rm -f bin/* examples/ciph/* examples/output/*
