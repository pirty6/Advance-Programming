kill -9 $(ps | grep -i server | tr ' ' ',' | cut -f1 -d,)
gcc -o server server.c -lpthread
gcc -o c client.c
./server 127.0.0.1 5000 hewwo &
./c 127.0.0.1 5000
