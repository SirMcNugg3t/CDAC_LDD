#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> 
#include <linux/slab.h>  

#define DEVICE_NAME "char_dyn"
#define MEM_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC");
MODULE_DESCRIPTION("Simple Character Driver with Dynamic Major");

static char *device_buffer;
static dev_t dev_num;
static struct cdev my_cdev;

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("Device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("Device closed\n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    if (*ppos >= MEM_SIZE)
        return 0;

    if (*ppos + count > MEM_SIZE)
        count = MEM_SIZE - *ppos;

    if (copy_to_user(buf, device_buffer + *ppos, count))
        return -EFAULT;

    *ppos += count;
    pr_info("Read %zu bytes\n", count);
    return count;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (*ppos >= MEM_SIZE)
        return -ENOSPC;

    if (*ppos + count > MEM_SIZE)
        count = MEM_SIZE - *ppos;

    if (copy_from_user(device_buffer + *ppos, buf, count))
        return -EFAULT;

    *ppos += count;
    pr_info("Wrote %zu bytes\n", count);
    return count;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write
};

static int __init mem_init(void)
{
    if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Failed to allocate major number\n");
        return -1;
    }

    pr_info("Allocated Major: %d, Minor: %d\n", MAJOR(dev_num), MINOR(dev_num));

    cdev_init(&my_cdev, &fops);
    if (cdev_add(&my_cdev, dev_num, 1) < 0) {
        pr_err("Failed to add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    device_buffer = kmalloc(MEM_SIZE, GFP_KERNEL);
    if (!device_buffer) {
        pr_err("Failed to allocate memory\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return -ENOMEM;
    }

    pr_info("mem_char driver loaded\n");
    return 0;
}

static void __exit mem_exit(void)
{
    kfree(device_buffer);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("mem_char driver unloaded\n");
}

module_init(mem_init);
module_exit(mem_exit);

