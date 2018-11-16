kill -9 $(ps | grep -i server | tr ' ' ',' | cut -f2 -d,)
gcc -o server server.c
gcc -o t test.c
gcc -o c client.c
./server 127.0.0.1 5000 /home/mariana/Documents/DIE &
./t 127.0.0.1 5000
