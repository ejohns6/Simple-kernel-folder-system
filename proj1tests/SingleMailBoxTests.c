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
	//Test with encryption
	//Test 1a: Test create normal
	//Result: Pass
	int test1a;
	test1a = syscall(__NR_create_mbox_421, 1959, 1);
	printf("Test1(A 0 is pass): %d\n", test1a);
	//Test 1b: Test create with same id
	//Result: pass
	test1a = syscall(__NR_create_mbox_421, 1959, 1);
	printf("Test1(Not a 0 is pass): %d\n", test1a);
	//Test count when should be none
	int CwN;
	CwN = syscall(__NR_count_msg_421, 1959);
	printf("Count after createion(A 0 is pass): %d\n", CwN);
	//
	//Test 2a: Create message normal
	//Status: Pass
	int test2a;
	unsigned char *user_message2 = (unsigned char *) malloc(13 * sizeof(unsigned char));
	strcpy(user_message2, "A man chooses");
	test2a = syscall(__NR_send_msg_421, 1959, user_message2, 13, 10);
	printf("Test2a(A 13 is pass): %d\n", test2a);
	printf("%s\n",user_message2);
	free(user_message2);
	//Test 2b: Create message with negative length
	int test2b;
	char *user_message2b = (char *) malloc(13 * sizeof(char));
	strcpy(user_message2b, "A man chooses");
	test2b = syscall(__NR_send_msg_421, 1959, user_message2b, (long)-13, 8);
	printf("Test2b( A -1 is pass): %d\n", test2b);
	free(user_message2b);
	//Test 2c: Create message with zero length
	int test2c;
	char *user_message2c = (char *) malloc(13 * sizeof(char));
	strcpy(user_message2c, "");
	test2c = syscall(__NR_send_msg_421, 1959, user_message2c, 0, 8);
	printf("Test2c(A 0 is pass): %d\n", test2c);
	free(user_message2c);
	//Test 2e: Create another message
	int test2e;
	char *user_message2e = (char *) malloc(13 * sizeof(char));
	strcpy(user_message2e, "A slave obeys");
	test2e = syscall(__NR_send_msg_421, 1959, user_message2e, 13, 10);
	printf("Test2e(A 13 is pass: %d\n", test2e);
	free(user_message2e);
	//Test 3:  Count message
	int test3;
	test3 = syscall(__NR_count_msg_421, 1959);
	printf("Test3(A 3 is pass: %d\n", test3);
	//Test 4:  Counts length of message when there is a message
	int test4;
	test4 = syscall(__NR_len_msg_421, 1959);
	printf("Test4(A 13 is pass: %d\n", test4);
	//Test 5:  peek
	int test5;
	unsigned char *user_message5 = (unsigned char *) malloc(13 * sizeof(unsigned char));
	test5 = syscall(__NR_peek_msg_421, 1959, user_message5, 13, 10);
	printf("Test5(A 13 is pass): %d\n", test5);
	printf("%s\n", user_message5);
	free(user_message5);
	//Test 6:  Count mbox
	//Result: Passed
	int test6;
	test6  = syscall(__NR_count_mbox_421);
	printf("Test6(Pass when 1): %d\n", test6);
	//Test 7:  recv message correctly
	int test7;
	unsigned char *user_message7 = (unsigned char *) malloc(13 * sizeof(unsigned char));
	test7 = syscall(__NR_recv_msg_421, 1959, user_message7, 13, 10);
	printf("Test7(A 13 is pass): %d\n", test7);
	printf("%s\n", user_message7);
	free(user_message7);
	//Count after Recv
	int CaR1;
	CaR1 = syscall(__NR_count_msg_421, 1959);
	printf("Count after Recv1( 3 is pass: %d\n", CaR1);
	//Test 9a: Test remove when there should be a message
	int test9a;
	test9a = syscall(__NR_remove_mbox_421, 1959);
	printf("Test9a(A -1 is pass): %d\n", test9a);
	//Test 8:  Do it till there is no message and then one more
	char *user_message8a = (char *) malloc(12* sizeof(char));
	int test8a;	
	test8a = syscall(__NR_recv_msg_421, 1959, user_message8a, 2, 8);
	printf("Test8a(A 0 is pass): %d\n", test8a);
	printf("%s\n", user_message8a);
	free(user_message8a);
	//Count after Recv
	int CaR2;
	CaR2 = syscall(__NR_count_msg_421, 1959);
	printf("Count after Recv1( 1 is pass): %d\n", CaR2);
	//Counts length of message when there is a message
	int msglen2;
	msglen2 = syscall(__NR_len_msg_421, 1959);
	printf("Test4(A 13 is pass: %d\n", msglen2);
	int test8b;	
	char *user_message8b = (char *) malloc(13 * sizeof(char));
	test8b = syscall(__NR_recv_msg_421, 1959, user_message8b, 13, 10);
	printf("Test8b(A 13 is pass: %d\n", test8b);
	printf("%s\n", user_message8b);
	free(user_message8b);
	int test8c;	
	char *user_message8c = (char *) malloc(13 * sizeof(char));
	test8c = syscall(__NR_recv_msg_421, 1959, user_message8c, 13, 8);
	printf("Test8c(A -1 is pass: %d\n", test8c);
	printf("%s\n", user_message8c);
	free(user_message8c);
	//Test 10: List all the mbox up to x
	//Status: Failed
	int test10int;	
	long unsigned int *test10 = (long unsigned int *) malloc(1 * sizeof(long unsigned int));
	test10int  = syscall(__NR_list_mbox_421, test10, 1);
	printf("Test10(Pass when 1959): %d\n", test10[0]);
	free(test10);
	//Test 9b: Test remove when there is nothing there (aka normal)
	int test9b;
	test9b = syscall(__NR_remove_mbox_421, 1959);
	printf("Test9b(A 0 is pass): %d\n\n\n\n\n", test9b);


	//Test withoutencryption
	//Test 21a: Test create normal
	//Result: Pass
	int test21a;
	test21a = syscall(__NR_create_mbox_421, 1959, 0);
	printf("Test21(A 0 is pass): %d\n", test21a);
	//Test 21b: Test create with same id
	//Result: pass
	test21a = syscall(__NR_create_mbox_421, 1959, 1);
	printf("Test21(Not a 0 is pass): %d\n", test21a);
	//Test count when should be none
	int weCwN;
	weCwN = syscall(__NR_count_msg_421, 1959);
	printf("Count after createion(A 0 is pass): %d\n", weCwN);
	//
	//Test 22a: Create message normal
	//Status: Pass
	int test22a;
	unsigned char *user_message2we = (unsigned char *) malloc(13 * sizeof(unsigned char));
	strcpy(user_message2we, "A man chooses");
	test22a = syscall(__NR_send_msg_421, 1959, user_message2we, 13, 10);
	printf("Test22a(A 13 is pass): %d\n", test22a);
	printf("%s\n",user_message2we);
	free(user_message2we);
	//Test 22b: Create message with negative length
	int test22b;
	char *user_message2bwe = (char *) malloc(13 * sizeof(char));
	strcpy(user_message2bwe, "A man chooses");
	test22b = syscall(__NR_send_msg_421, 1959, user_message2bwe, (long)-13, 8);
	printf("Test22b( A -1 is pass): %d\n", test22b);
	free(user_message2bwe);
	//Test 22c: Create message with zero length
	int test22c;
	char *user_message2cwe = (char *) malloc(13 * sizeof(char));
	strcpy(user_message2cwe, "");
	test22c = syscall(__NR_send_msg_421, 1959, user_message2cwe, 0, 8);
	printf("Test22c(A 0 is pass): %d\n", test22c);
	free(user_message2cwe);
	//Test 22e: Create another message
	int test22e;
	char *user_message2ewe = (char *) malloc(13 * sizeof(char));
	strcpy(user_message2ewe, "A slave obeys");
	test22e = syscall(__NR_send_msg_421, 1959, user_message2ewe, 13, 10);
	printf("Test22e(A 13 is pass): %d\n", test22e);
	free(user_message2ewe);
	//Test 23:  Count message
	int test23;
	test23 = syscall(__NR_count_msg_421, 1959);
	printf("Test23(A 2 is pass): %d\n", test23);
	//Test 24:  Counts length of message when there is a message
	int test24;
	test24 = syscall(__NR_len_msg_421, 1959);
	printf("Test24(A 13 is pass: %d\n", test24);
	//Test 25:  peek
	int test25;
	unsigned char *user_message5we = (unsigned char *) malloc(13 * sizeof(unsigned char));
	test25 = syscall(__NR_peek_msg_421, 1959, user_message5we, 13, 10);
	printf("Test25(A 13 is pass): %d\n", test25);
	printf("%s\n", user_message5we);
	free(user_message5we);
	//Test 26:  Count mbox
	//Result: Passed
	int test26;
	test26  = syscall(__NR_count_mbox_421);
	printf("Test26(Pass when 1): %d\n", test26);
	//Test 27:  recv message correctly
	int test27;
	unsigned char *user_message7we = (unsigned char *) malloc(13 * sizeof(unsigned char));
	test27 = syscall(__NR_recv_msg_421, 1959, user_message7we, 13, 10);
	printf("Test27(A 13 is pass): %d\n", test7);
	printf("%s\n", user_message7we);
	free(user_message7we);
	//Count after Recv
	int CaR1we;
	CaR1we = syscall(__NR_count_msg_421, 1959);
	printf("Count after Recv1( 3 is pass: %d\n", CaR1we);
	//Test 29a: Test remove when there should be a message
	int test29a;
	test29a = syscall(__NR_remove_mbox_421, 1959);
	printf("Test9a(A -1 is pass): %d\n", test29a);
	//Test 28:  Do it till there is no message and then one more
	char *user_message8awe = (char *) malloc(12* sizeof(char));
	int test28a;	
	test28a = syscall(__NR_recv_msg_421, 1959, user_message8awe, 2, 8);
	printf("Test28a(A 0 is pass:) %d\n", test28a);
	printf("%s\n", user_message8awe);
	free(user_message8awe);
	//Count after Recv
	int CaR2we;
	CaR2we = syscall(__NR_count_msg_421, 1959);
	printf("Count after Recv1( 3 is pass: %d\n", CaR2we);
	//Counts length of message when there is a message
	int msglen2we;
	msglen2we = syscall(__NR_len_msg_421, 1959);
	printf("Test4(A 13 is pass: %d\n", msglen2we);
	int test28b;	
	char *user_message8bwe = (char *) malloc(13 * sizeof(char));
	test28b = syscall(__NR_recv_msg_421, 1959, user_message8bwe, 13, 10);
	printf("Test28b(A 13 is pass): %d\n", test28b);
	printf("%s\n", user_message8bwe);
	free(user_message8bwe);
	int test28c;	
	char *user_message8cwe = (char *) malloc(13 * sizeof(char));
	test28c = syscall(__NR_recv_msg_421, 1959, user_message8cwe, 13, 8);
	printf("Test28c(A -1 is pass): %d\n", test28c);
	printf("%s\n", user_message8cwe);
	free(user_message8cwe);
	//Test 210: List all the mbox up to x
	//Status: Pass
	int test210int;	
	long unsigned int *test10we = (long unsigned int *) malloc(1 * sizeof(long unsigned int));
	test210int  = syscall(__NR_list_mbox_421, test10we, 1);
	printf("Test210(Pass when 1959): %d\n", test10we[0]);
	free(test10we);
	//Test 29b: Test remove when there is nothing there (aka normal)
	int test29b;
	test29b = syscall(__NR_remove_mbox_421, 1959);
	printf("Test29b(A 0 is pass): %d\n", test29b);


	return 0;
}
