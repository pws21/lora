#gcc -O2 -std=gnu99 -Wall -pedantic -fPIC -I/home/pws/odpi/include -I/usr/include/lua5.2 -L/home/pws/odpi/lib -L/usr/lib/lua5.2 -lodpic test.c -o test.so -shared

#gcc test.o -o test -L../odpi/lib -lodpic -ldl
#gcc test.o -o test_exe -L../odpi/lib -L/usr/lib/lua5.2 -ldl -lodpic -llua52

# Working variant
gcc -c -I../odpi/include -I/usr/include/lua5.2 -std=gnu99 -L../odpi/lib -L/usr/lib/lua5.2 -lodpic  -O2 -g -Wall -fPIC -o test.o test.c
gcc test.o -L../odpi/lib -lodpic -shared -o test.so
