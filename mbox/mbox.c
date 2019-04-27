#include <linux/kernel.h>
#include <linux/cred.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/thread_info.h>
#include <linux/mutex.h>

struct MailBox{
	unsigned long id;

	bool enabled;

	struct Message* MessageHead;

	struct MailBox* next; 
};

struct Message{
	struct Message* next;
	unsigned char* Mess;
	int MessageLength;
};

struct MailBox* MBHead;
static DEFINE_MUTEX(mboxC);
/*
struct MailBoxHead *MBHead;
EXPORT_SYMBOL(MBHead);
extern MBHead = kmalloc(sizeof (struct MailBoxHead), GFP_KERNEL);*/

SYSCALL_DEFINE2(create_mbox_421, unsigned long, id, int, enable_crypt){
	mutex_lock(&mboxC);	
	// int user_id_cred;
	// user_id_cred = get_current_cred();	
	if(current_uid().val != 0){
		printk("Error = EPERM");
		mutex_unlock(&mboxC);
		return(-EPERM);
	}
	printk("Pass1");
	if(MBHead != NULL){
		struct MailBox *pos;
		pos = MBHead; 
		printk("Pass1a");		
		while(true){
			printk("Pass1aa");
			if(pos->id == id){
				printk("Error = EEXIST\n");
				mutex_unlock(&mboxC);
				return(-EEXIST); 
				printk("Should not be possible");
			}
			printk("Pass1ab");
			if(pos->next == NULL){
				break;
			}
			pos = pos->next;
			
		}
	}
	printk("Pass2");
	
	struct MailBox* newBox;
	newBox = kmalloc(sizeof(struct MailBox), GFP_KERNEL);
	newBox->id = id;
	printk("Pass3");
	bool _e_c;
	if(enable_crypt != 0){
		_e_c = true;
	}else{
		_e_c = false;
	}
	newBox->enabled = _e_c;
	printk("Pass4");
	if(MBHead == NULL){
		newBox->next = NULL;
	}else{
		newBox->next = MBHead;
	}
	newBox->MessageHead = NULL;
	printk("Pass5");
	MBHead = newBox;
	printk("Pass6");
	mutex_unlock(&mboxC);
	return(0);
}

SYSCALL_DEFINE1(remove_mbox_421, unsigned long, id){
	mutex_lock(&mboxC);
	if(current_uid().val != 0){
		printk("Error = EPERM");
		mutex_unlock(&mboxC);
		return(-EPERM);
	}
	printk("Rem Pass1");
	if(MBHead != NULL){
		struct MailBox* pos;
		pos = MBHead;
		//If it is the first object
		if(pos->id == id){
			//printk("Rem Pass2\n");
			
			if(pos->MessageHead != NULL){
				printk("Rem Pass2b");
				printk("Error = ENOTEMPTY");
				mutex_unlock(&mboxC);
				return(-ENOTEMPTY);
			}else if(pos->MessageHead == NULL){ // }else{

				printk("Rem Pass3\n");
				MBHead = pos->next;
				kfree(pos);
				mutex_unlock(&mboxC);
				return(0);
			}
		}
		printk("Rem Pass4");
		struct MailBox* prevPos;
		if(pos->next == NULL){
			printk("Error = ENOENT\n");
			mutex_unlock(&mboxC);
			return(-ENOENT);
		}
		prevPos = pos;
		pos = pos->next;			
		while(true){
			printk("Rem Pass5");
			if(pos->id == id){ 
				if(pos->MessageHead != NULL){
					printk("Error = ENOTEMPTY\n");
					mutex_unlock(&mboxC);
					return(-ENOTEMPTY);
				}else if(pos->MessageHead == NULL){
					printk("Rem PassDelMid");
					printk("Prev:%d\n", prevPos->id);
					printk("Pos:%d\n", pos->id);
					if(pos->next != NULL){
						printk("Pos:%d\n", pos->next->id);							
					}
					prevPos->next = pos->next;
					kfree(pos);
					mutex_unlock(&mboxC);
					return(0);
				}
			}
			prevPos = pos;
			if(pos->next == NULL){
				printk("Error = ENOENT\n");
				mutex_unlock(&mboxC);
				return(-ENOENT);
			}
			pos = pos->next;
			
		}
	}else{
		printk("Error = ENOENT\n");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}	
	printk("Pass6");
	mutex_unlock(&mboxC);
	return(0);
}

SYSCALL_DEFINE0(count_mbox_421){
	mutex_lock(&mboxC);
	printk("Count Pass1");
	int count;
	count = 0;
	if(MBHead != NULL){
		struct MailBox* pos;
		pos = MBHead;
		while(true){
			printk("Count Pass2");
			count = count + 1;
			if(pos->next == NULL){
				printk("Count Pass3");
				break;
			}
			pos = pos->next;
			
		}
	}
	mutex_unlock(&mboxC);
	return(count);
}

SYSCALL_DEFINE2(list_mbox_421, unsigned __user *, mbxes, long, k){
	mutex_lock(&mboxC);
	printk("List Mbox Pass 1");	
	int count_list;
	count_list = 0;
	if(MBHead != NULL){
		struct MailBox* pos;
		pos = MBHead;
		printk("List Mbox Pass 2");			
		while(true){
			printk("List Mbox Pass 3a");
			if(count_list >= k){
				break;
			}
			mbxes[count_list] = pos->id;
			printk("List Mbox Pass 3b");
			count_list += 1;
			if(pos->next == NULL){
				break;
			}
			printk("List Mbox Pass 3c");
			pos = pos->next;
		}
	}
	mutex_unlock(&mboxC);
	return(count_list);
}
//copy_to_from and copy_to_user
SYSCALL_DEFINE4(send_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, unsigned long, key){
	mutex_lock(&mboxC);	
	printk("send pass 1");
	if(n < 0){
		printk("Error = EINVAL");
		mutex_unlock(&mboxC);
		return(-EINVAL);
	}
	if(msg == NULL){
		printk("Error = EFAULT");
		mutex_unlock(&mboxC);
		return(-EFAULT);
	}
	printk("send pass 2");	
	if(MBHead != NULL){
		struct MailBox* pos;
		pos = MBHead;			
		while(true){
			printk("send pass 3");
			if(pos->id == id){ 
				if(pos->enabled == false){
					printk("send pass 4a");
					struct Message* mes = kmalloc(sizeof(struct Message), GFP_KERNEL);
					int send_mis;
					unsigned char *un_encrypted = kmalloc(sizeof(unsigned char) * n, GFP_KERNEL);
					int count_send;					
					if(msg != NULL && n != 0){	
						
						send_mis = copy_from_user(un_encrypted, msg, n);
						count_send = n - send_mis;
					}else if(msg != NULL){
						count_send = 0;
					}else{
						printk("Error = EFAULT\n");
						mutex_unlock(&mboxC);
						return(-EFAULT);
					}

					

					printk("send pass 4a");

					mes->Mess = un_encrypted;
					mes->MessageLength = count_send;
					struct Message* last;
					mes->next = NULL;
					if(pos->MessageHead == NULL){
						printk("send 101");						
						pos->MessageHead = mes;
					}else if(pos->MessageHead != NULL){
						printk("send 102");
						last = pos->MessageHead;
						while(true){
							if(last->next == NULL){
								printk("Send 10");
								break;
							}
							last = last->next;
						}
						last->next  =  mes; 
					}
					mutex_unlock(&mboxC);
					return(count_send);
				}else{
					printk("send pass 5a");
					//unsigned char is 8 bits and unsigned long is 32
					int len;
					len = n;
					printk("%n\n", n);
					while(len%4 != 0){
						len = len + 1;
					}
					unsigned char *encrypted_temp = kmalloc(sizeof(unsigned char) * n, GFP_KERNEL);
					int not_copied;	
					printk("send pass 5aa");				
					if(msg != NULL && n != 0){	
						not_copied =copy_from_user(encrypted_temp, msg, n);
					}else if(msg != NULL){
					}else{
						printk("Error = EFAULT\n");
						mutex_unlock(&mboxC);
						return(-EFAULT);
					}
					unsigned char *encrypted_final = kmalloc(sizeof(unsigned char) * n, GFP_KERNEL);
					/*					
					if(n == 0){
						strcpy(encrypted_final, "");
					}*/
					int count_send;
					count_send = 0;
					printk("send pass 5b");
					while(count_send < len && count_send < n){
						//unsigned char * tempString2 = (unsigned char*)&tempLong;
						if(count_send < n){
							encrypted_final[count_send] = encrypted_temp[count_send] ^ ((key >> (8 * 0)) & 0xff); 
							printk("send pass 6a");
						}else{
							count_send += 1;
							break;
						}
						if(count_send + 1 < n){
							encrypted_final[count_send + 1] = encrypted_temp[count_send + 1] ^ ((key >> (8 * 1)) & 0xff);
							printk("send pass 6b");
						}else{
							count_send += 2;
							break;
						}
						if(count_send + 2 < n){
							encrypted_final[count_send + 2] = encrypted_temp[count_send + 2] ^ ((key >> (8 * 2)) & 0xff);
							printk("send pass 6c");
						}else{
							count_send += 2;
							break;
						}
						if(count_send + 3 < n){
							encrypted_final[count_send + 3] = encrypted_temp[count_send + 3] ^ ((key >> (8 * 3)) & 0xff);
							printk("send pass 6d");
						}else{
							count_send += 3;
							break;
						}
						count_send = count_send + 4;
					}
					printk("send pass 7");
					encrypted_temp = NULL;
					kfree(encrypted_temp);
					struct Message* mes = kmalloc(sizeof(struct Message), GFP_KERNEL);
					mes->Mess = encrypted_final;
					mes->MessageLength = n;
					struct Message* last;
					mes->next = NULL;
					if(pos->MessageHead == NULL){						
						pos->MessageHead = mes;
					}else if(pos->MessageHead != NULL){
						last = pos->MessageHead;
						while(true){
							if(last->next == NULL){
								printk("Send 10");
								break;
							}
							last = last->next;
						}
						last->next  =  mes; 
					}
					mutex_unlock(&mboxC);
					return(count_send);
				}
			}
			if(pos->next == NULL){
				printk("Error = ENOENT\n");
				mutex_unlock(&mboxC);
				return(-ENOENT);
			}
			pos = pos->next;
			
		}
	}else{
		printk("Error = ENOENT\n");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}
	mutex_unlock(&mboxC);
	return(0);
}

SYSCALL_DEFINE4(recv_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, unsigned long, key){
	mutex_lock(&mboxC);	
	if(n < 0){
		printk("Error = ENOENT");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}
	if(msg == NULL){
		printk("Error1 = EFAULT");
		mutex_unlock(&mboxC);
		return(-EFAULT);
	}
	/*
	if(likely(check_copy_size(msg, n, false))){
		printk("Error2 = EFAULT");
		mutex_unlock(&mboxC);
		return(-EFAULT);
	}
	*/
	if(__compiletime_object_size(msg) <= n){
		printk("Error2 = EFAULT");
		mutex_unlock(&mboxC);
		return(-EFAULT);
	}
	int len_check;
	if(MBHead != NULL){
		struct MailBox *pos;
		pos = MBHead;			
		while(true){
			if(pos->id == id){ 
				struct Message *rec_mes;
				rec_mes = pos->MessageHead;
				if(rec_mes == NULL){
					printk("Nothing there");
					mutex_unlock(&mboxC);
					return(-1);
				}
				if(pos->enabled == false){
					struct Message *rec_mes;
					rec_mes = pos->MessageHead;
					
					unsigned char *un_encrypted_rec;
					un_encrypted_rec = rec_mes->Mess;
					int not_copied;
					int count_recv;
					if(msg != NULL && rec_mes->MessageLength != 0){	
						not_copied = copy_to_user(msg, un_encrypted_rec, n);
						count_recv = n - not_copied;
					}else if(msg != NULL && rec_mes->MessageLength == 0){
						count_recv = 0;
					}else{
						printk("Error = EFAULT\n");
						mutex_unlock(&mboxC);
						return(-EFAULT);
					}
					
					
					pos-> MessageHead = rec_mes->next;
					kfree(rec_mes);
					mutex_unlock(&mboxC);
					return(count_recv);
				}else{
					//unsigned char is 8 bits and unsigned long is 32
					int len;
					len = rec_mes->MessageLength;
					while(len%4 != 0){
						len = len + 1;
					}
					unsigned char *encrypted_temp;
					encrypted_temp = rec_mes->Mess;
					unsigned char *un_encrypted = kmalloc(sizeof(unsigned char) * len, GFP_KERNEL);
					int count_recv;
					count_recv = 0;
	
					while(count_recv < len && count_recv < n){
						if(count_recv < n){
							un_encrypted[count_recv] = encrypted_temp[count_recv] ^ ((key >> (8 * 0)) & 0xff);
						}
						if(count_recv + 1 < n){
							un_encrypted[count_recv + 1] = encrypted_temp[count_recv + 1] ^ ((key >> (8 * 1)) & 0xff);
						}else{
							count_recv = count_recv + 1;
							break;
						}
						if(count_recv + 2 < n){
							un_encrypted[count_recv + 2] = encrypted_temp[count_recv + 2] ^ ((key >> (8 * 2)) & 0xff);
						}else{
							count_recv = count_recv + 2;
							break;
						}
						if(count_recv + 3 < n){
							un_encrypted[count_recv + 3] = encrypted_temp[count_recv + 3] ^ ((key >> (8 * 3)) & 0xff);
						}else{
							count_recv = count_recv + 3;
							break;
						}
						count_recv = count_recv + 4;
					};
					pos->MessageHead = rec_mes->next;
					kfree(encrypted_temp);
					kfree(rec_mes);
					int not_copied;
					if(msg != NULL && rec_mes->MessageLength != 0){	
						not_copied = copy_to_user(msg, un_encrypted, n);
					}else if(msg != NULL && rec_mes->MessageLength == 0){
						mutex_unlock(&mboxC);
						return(0);
					}else{
						printk("Error = EFAULT\n");
						mutex_unlock(&mboxC);
						return(-EFAULT);
					}
					count_recv = count_recv - not_copied;
					mutex_unlock(&mboxC);
					return(count_recv);
				}
			}
			if(pos->next == NULL){
				printk("Error = ENOENT\n");
				mutex_unlock(&mboxC);
				return(-ENOENT);
			}
			pos = pos->next;
			
		}
	}else{
		printk("Error = ENOENT\n");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}
}

SYSCALL_DEFINE4(peek_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, unsigned long, key){
	mutex_lock(&mboxC);	
	if(n < 0){
		printk("Error = ENOENT");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}
	printk("peek pass1\n");
	if(msg == NULL){
		printk("Error = EFAULT");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}
	/*
	if(likely(check_copy_size(msg, n, false))){
		printk("Error2 = EFAULT");
		mutex_unlock(&mboxC);
		return(-EFAULT);
	}
	*/
	if(__compiletime_object_size(msg) <= n){
		printk("Error2 = EFAULT");
		mutex_unlock(&mboxC);
		return(-EFAULT);
	}
	int len_check;
	printk("peek pass2\n");
	if(MBHead != NULL){
		printk("peek pass3\n");
		struct MailBox *pos;
		pos = MBHead;			
		while(true){
			if(pos->id == id){ 
				printk("peek pass4\n");
				struct Message *rec_mes;
				rec_mes = pos->MessageHead;
				while(true){
					printk("peek pass4a\n");
					if(rec_mes->next == NULL){
						printk("peek pass4b\n");
						break;
					}
					rec_mes = rec_mes->next;
				}
				printk("peek pass5\n");
				if(pos->enabled == false){
					printk("peek pass6\n");
					unsigned char *un_encrypted_rec;
					un_encrypted_rec = rec_mes->Mess;
					int not_copied;
					int count_recv;
					if(msg != NULL && rec_mes->MessageLength != 0){	
						not_copied = copy_to_user(msg, un_encrypted_rec, n);
						count_recv = n - not_copied;
					}else if(msg != NULL && rec_mes->MessageLength == 0){
						count_recv = 0;
					}else{
						printk("Error = EFAULT\n");
						mutex_unlock(&mboxC);
						return(-EFAULT);
					}

					mutex_unlock(&mboxC);
					return(count_recv);
				}else{
					//unsigned char is 8 bits and unsigned long is 32 or is it 64
					printk("peek pass7\n");
					int len;
					len = rec_mes->MessageLength;
					while(len%4 != 0){
						len = len + 1;
					}
					unsigned char *un_encrypted_temp;
					un_encrypted_temp = rec_mes->Mess;
					unsigned char *un_encrypted;
					un_encrypted = kmalloc(sizeof(unsigned char) * rec_mes->MessageLength, GFP_KERNEL);
					int count_recv;
					count_recv = 0;
					while(count_recv < len && count_recv < n){
						if(count_recv < n){
							un_encrypted[count_recv] = un_encrypted_temp[count_recv] ^ ((key >> (8 * 0)) & 0xff);
						}
						if(count_recv + 1 < n){
							un_encrypted[count_recv + 1] = un_encrypted_temp[count_recv + 1] ^ ((key >> (8 * 1)) & 0xff);
						}else{
							count_recv = count_recv + 1;
							break;
						}
						if(count_recv + 2 < n){
							un_encrypted[count_recv + 2] = un_encrypted_temp[count_recv + 2] ^ ((key >> (8 * 2)) & 0xff);
						}else{
							count_recv = count_recv + 2;
							break;
						}
						if(count_recv + 3 < n){
							un_encrypted[count_recv + 3] = un_encrypted_temp[count_recv + 3] ^ ((key >> (8 * 3)) & 0xff);
						}else{
							count_recv = count_recv + 3;
							break;
						}
						count_recv = count_recv + 4;
					}
					int not_copied;
					if(msg != NULL && rec_mes->MessageLength != 0){	
						not_copied = copy_to_user(msg, un_encrypted, n);
						//if not 0 then error
					}else if(msg != NULL && rec_mes->MessageLength == 0){
						mutex_unlock(&mboxC);
						return(0);
					}else{
						printk("Error = EFAULT\n");
						mutex_unlock(&mboxC);
						return(-EFAULT);
					}
					not_copied = n - not_copied;
					kfree(un_encrypted_temp);
					kfree(un_encrypted);
					mutex_unlock(&mboxC);
					return(not_copied);
				}
			}
			if(pos->next == NULL){
				printk("Error = ENOENT\n");
				mutex_unlock(&mboxC);
				return(-ENOENT);
			}
			pos = pos->next;	
		}
	}else{
		printk("Error = ENOENT\n");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}
	
}

SYSCALL_DEFINE1(count_msg_421, unsigned long, id){
	mutex_lock(&mboxC);	
	int count = 0;
	if(MBHead != NULL){
		struct MailBox *pos;
		pos = MBHead;			
		while(true){
			if(pos->id == id){ 
				struct Message *mes;
				mes = pos->MessageHead;
				while(true){
					if(mes == NULL){
						mutex_unlock(&mboxC);
						return(count);
					}
					count = count + 1;
					mes = mes -> next;
				}				
			}
			if(pos->next == NULL){
				printk("Error = ENOENT");
				mutex_unlock(&mboxC);
				return(-ENOENT);
			}
			pos = pos->next;
			
		}
	}else{
		printk("Error = ENOENT\n");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}
	mutex_unlock(&mboxC);
	return(0);
}

SYSCALL_DEFINE1(len_msg_421, unsigned long, id){
	mutex_lock(&mboxC);	
	int count = 0;
	if(MBHead != NULL){
		struct MailBox *pos;
		pos = MBHead;			
		while(true){
			if(pos->id == id){ 
				if(pos->MessageHead == 0){
					printk("Error = ENOENT");
					mutex_unlock(&mboxC);
					return(-ENOENT);
				}
				/*
				char *mes;
				mes = pos->Mess;
				*/
				struct Message *mes;
				mes = pos->MessageHead;
				count = mes->MessageLength;
				mutex_unlock(&mboxC);
				return(count);
			}
			if(pos->next == NULL){
				printk("Error = ENOENT");
				mutex_unlock(&mboxC);
				return(-ENOENT);
			}
			pos = pos->next;
			
		}
	}else{
		printk("Error = ENOENT\n");
		mutex_unlock(&mboxC);
		return(-ENOENT);
	}
//adasfsaf
	mutex_unlock(&mboxC);
	return(0);
}
