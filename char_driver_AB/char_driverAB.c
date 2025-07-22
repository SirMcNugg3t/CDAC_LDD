#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "char_case"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");
MODULE_DESCRIPTION("Character Case Transformer Driver");

static dev_t dev_num;
static struct cdev my_cdev;
static char stored_char = 0;

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

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    char kbuf;

    if (copy_from_user(&kbuf, buf, 1))
        return -EFAULT;

    pr_info("Received from user: %c\n", kbuf);

    if (kbuf == 'a')
        stored_char = 'b';
    else if (kbuf == 'A')
        stored_char = 'B';
    else
        stored_char = '?';

    return 1;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    if (*off > 0) return 0; // EOF

    if (copy_to_user(buf, &stored_char, 1))
        return -EFAULT;

    *off += 1;
    return 1;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .write = my_write,
    .read = my_read,
};

static int __init case_init(void)
{
    if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Failed to allocate major number\n");
        return -1;
    }

    cdev_init(&my_cdev, &fops);
    if (cdev_add(&my_cdev, dev_num, 1) < 0) {
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    pr_info("char_case driver loaded, Major: %d\n", MAJOR(dev_num));
    return 0;
}

static void __exit case_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("char_case driver unloaded\n");
}

module_init(case_init);
module_exit(case_exit);

