kill -9 $(ps | grep -i server | tr ' ' ',' | cut -f2 -d,)
gcc -o server server.c
gcc -o c client.c
./server 127.0.0.1 5000 /home/mariana/Documents/DIE &
./c 127.0.0.1 5000
