#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");
MODULE_DESCRIPTION("Prints process info at module load time");

static int __init process_info_init(void)
{
    struct task_struct *task = current;

    pr_info("Process Info at Module Load:\n");
    pr_info("PID      : %d\n", task->pid);
    pr_info("TGID     : %d\n", task->tgid);
    pr_info("PPID     : %d\n", task->real_parent->pid);
    pr_info("COMM     : %s\n", task->comm);
    pr_info("PRIORITY : %d\n", task->prio);
    pr_info("STATIC_PRIO : %d\n", task->static_prio);
    pr_info("NORMAL_PRIO : %d\n", task->normal_prio);
    pr_info("CPU      : %d\n", task_cpu(task));

    return 0;
}

static void __exit process_info_exit(void)
{
    pr_info("Process info module exiting\n");
}

module_init(process_info_init);
module_exit(process_info_exit);

