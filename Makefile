weather:main.o picture.o cJSON.o
        gcc -o weather main.o picture.o cJSON.o -lcurl
main.o:main.c
        gcc -c main.c
picture.o:picture.c
        gcc -c picture.c
cJSON.o:cJSON.c cJSON.h
        gcc -c cJSON.c
clean:
        -rm main.o cJSON.o picture.o weather
