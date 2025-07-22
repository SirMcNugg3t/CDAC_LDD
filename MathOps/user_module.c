#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");
MODULE_DESCRIPTION("User Module for math_ops");

extern int add(int, int);
extern int sub(int, int);
extern int mul(int, int);
extern int div_op(int, int);
extern int avg(int, int);

static int __init user_module_init(void)
{
    int x = 10, y = 5;

    pr_info("add(%d, %d) = %d\n", x, y, add(x, y));
    pr_info("sub(%d, %d) = %d\n", x, y, sub(x, y));
    pr_info("mul(%d, %d) = %d\n", x, y, mul(x, y));
    pr_info("div(%d, %d) = %d\n", x, y, div_op(x, y));
    pr_info("avg(%d, %d) = %d\n", x, y, avg(x, y));

    return 0;
}

static void __exit user_module_exit(void)
{
    pr_info("user_module removed\n");
}

module_init(user_module_init);
module_exit(user_module_exit);

