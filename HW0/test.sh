gcc hw0.c test.c -o hw0
./hw0 > hw0-result.txt
diff hw0-result.txt desired-result.txt