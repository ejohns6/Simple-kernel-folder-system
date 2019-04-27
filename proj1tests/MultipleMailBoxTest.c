#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define __NR_create_mbox_421	335
#define __NR_remove_mbox_421	336
#define __NR_count_mbox_421	337
#define __NR_list_mbox_421	338
#define __NR_send_msg_421	339
#define __NR_recv_msg_421	340
#define __NR_peek_msg_421	341
#define __NR_count_msg_421	342
#define __NR_len_msg_421	343


//This is done in root
int main(int argc, char *argv[]) {
	//Test 9:  Count mbox
	//Result: Passed
	int test9;
	test9  = syscall(__NR_count_mbox_421);
	printf("Test6(Pass when 4): %d\n", test9);	
	//Test 1: Test create normal
	//Result: Pass
	int test1;
	test1 = syscall(__NR_create_mbox_421, 1, 1);
	printf("Test1(A 0 is pass): %d\n", test1);
	//Test 2: Test create but there is already one created
	//Result: pass
	unsigned long id2 = 2;
	int enc2 = 1;
	int test2;
	test2 = syscall(__NR_create_mbox_421, id2, enc2);
	printf("Test2(A 0 is pass): %d\n", test2);
	//Test 3: Test create but there is already one created
	//Result: pass
	int test3;	
	test3 = syscall(__NR_create_mbox_421, 3, 1);
	printf("Test3(A 0 is pass): %d\n", test3);
	//Test 4: Test create but there is already one created
	//Result: pass
	int test4;	
	test4 = syscall(__NR_create_mbox_421, 4, 1);
	printf("Test4(A 0 is pass): %d\n", test4);
	//Test 5: Test create but there is already one created
	//Result: pass
	int test5;	
	test5 = syscall(__NR_create_mbox_421, 5, 1);
	printf("Test5(A 0 is pass): %d\n", test5);
	//Test 6: Test create but there is already one created
	//Result: pass
	int test6;	
	test6 = syscall(__NR_create_mbox_421, 6, 1);
	printf("Test6(A 0 is pass): %d\n", test6);
	//Test 7: Test create but there is already one created
	//Result: pass
	int test7;	
	test7 = syscall(__NR_create_mbox_421, 7, 1);
	printf("Test7(A 0 is pass): %d\n", test7);
	//Test 8: Test create but there is already one created
	//Result: pass
	int test8;	
	test8 = syscall(__NR_create_mbox_421, 8, 1);
	printf("Test7(A 0 is pass): %d\n", test8);
	//Test 9:  Count mbox
	//Result: Passed
	test9  = syscall(__NR_count_mbox_421);
	printf("Test6(Pass when 4): %d\n", test9);

	//Test 10: Test remove 
	int test10;
	test10 = syscall(__NR_remove_mbox_421, 1);
	printf("Test10(A 0 is pass): %d\n", test10);
	//Test 11: Test remove : ERROR
	int test11;
	test11 = syscall(__NR_remove_mbox_421, 8);
	printf("Test11(A 0 is pass): %d\n", test11);
	//Test 12: Test remove 
	int test12;
	test12 = syscall(__NR_remove_mbox_421, 2);
	printf("Test12(A 0 is pass): %d\n", test12);
	//Test 13: Test remove 
	int test13;
	test13 = syscall(__NR_remove_mbox_421, 7);
	printf("Test13(A 0 is pass): %d\n", test13);
	//Test 14: Test remove 
	int test14;
	test14 = syscall(__NR_remove_mbox_421, 5);
	printf("Test14(A 0 is pass): %d\n", test14);
	//Test 15: Test remove
	int test15;
	test15 = syscall(__NR_remove_mbox_421, 6);
	printf("Test15(A 0 is pass): %d\n", test15);
	//Test 16: Test remove
	int test16;
	test16 = syscall(__NR_remove_mbox_421, 4);
	printf("Test16(A 0 is pass): %d\n", test16);
	//Test 17: Test remove
	int test17;
	test17 = syscall(__NR_remove_mbox_421, 3);
	printf("Test17(A 0 is pass): %d\n", test17);
	//Test 18: Test remove
	int test18;
	test18 = syscall(__NR_remove_mbox_421, 1);
	printf("Test18(A -1 is pass): %d\n", test18);
	//Test 9:  Count mbox
	//Result: Passed
	test9  = syscall(__NR_count_mbox_421);
	printf("Test6(Pass when 4): %d\n", test9);	
	return 0;
	
}
