gcc  malloc_test.c my_malloc.c -w -o mine
./mine > test_output.txt
diff test_output.txt desired_test_output.txt


# gcc  my_malloc-driver.c my_malloc.c -w -o mine
# ./mine
