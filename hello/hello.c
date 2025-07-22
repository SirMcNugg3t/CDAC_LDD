#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>


static int __init hello_init(void){
	pr_info("Hello");
	return 0;
}

static void __exit hello_exit(void){
	pr_info("Exiting Hello");	
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("nugget");
