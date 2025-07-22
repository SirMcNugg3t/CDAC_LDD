#include <linux/module.h>
#include <linux/kernel.h>

extern int add(int a, int b);  // Declare external function

static int __init avg_init(void)
{
    int a = 10, b = 20;
    int sum = add(a, b); // Use add() from add_module
    int avg = sum / 2;

    pr_info("avg_module loaded\n");
    pr_info("Average of %d and %d is %d\n", a, b, avg);
    return 0;
}

static void __exit avg_exit(void)
{
    pr_info("avg_module unloaded\n");
}

module_init(avg_init);
module_exit(avg_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");
MODULE_DESCRIPTION("Uses add() from add_module to compute average");

