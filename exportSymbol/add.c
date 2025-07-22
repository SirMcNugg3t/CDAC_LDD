
#include <linux/module.h>
#include <linux/kernel.h>

int add(int a, int b)
{
    return a + b;
}
EXPORT_SYMBOL(add); 

static int __init add_init(void)
{
    pr_info("add_module loaded\n");
    return 0;
}

static void __exit add_exit(void)
{
    pr_info("add_module unloaded\n");
}

module_init(add_init);
module_exit(add_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");
MODULE_DESCRIPTION("Provides add() function");

