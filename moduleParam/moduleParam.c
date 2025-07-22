#include <linux/init.h>
#include <linux/module.h>

static int a = 0;
static int b = 0;

module_param(a, int, 0444);
MODULE_PARM_DESC(a, "First integer parameter");

module_param(b, int, 0444);
MODULE_PARM_DESC(b, "Second integer parameter");

static int __init add_module_init(void)
{
    pr_info("add_module: a = %d, b = %d\n", a, b);
    pr_info("add_module: sum = %d\n", a + b);
    return 0;
}

static void __exit add_module_exit(void)
{
    pr_info("add_module: exiting\n");
}

module_init(add_module_init);
module_exit(add_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");
MODULE_DESCRIPTION("A simple add module with module_param()");

