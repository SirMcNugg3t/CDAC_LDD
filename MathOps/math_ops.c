#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");
MODULE_DESCRIPTION("Math Operations Provider Module");

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_op(int a, int b) { return b ? a / b : 0; }
int avg(int a, int b) { return (a + b) / 2; }

EXPORT_SYMBOL(add);
EXPORT_SYMBOL(sub);
EXPORT_SYMBOL(mul);
EXPORT_SYMBOL(div_op);
EXPORT_SYMBOL(avg);

static int __init math_ops_init(void)
{
    pr_info("math_ops module loaded\n");
    return 0;
}

static void __exit math_ops_exit(void)
{
    pr_info("math_ops module unloaded\n");
}

module_init(math_ops_init);
module_exit(math_ops_exit);

