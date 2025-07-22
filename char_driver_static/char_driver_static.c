#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychardev"
#define MEM_SIZE 1024
#define MY_MAJOR 80

static char device_buffer[MEM_SIZE];
static int device_open = 0;

static int my_open(struct inode *inode, struct file *file)
{
    if (device_open)
        return -EBUSY;
    
    device_open++;
    pr_info("Device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    device_open--;
    pr_info("Device released\n");
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
    pr_info("Read %zu bytes from device\n", count);
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
    pr_info("Wrote %zu bytes to device\n", count);
    return count;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

static int __init mychardev_init(void)
{
    int ret = register_chrdev(MY_MAJOR, DEVICE_NAME, &fops);
    if (ret < 0) {
        pr_err("Failed to register device with major %d\n", MY_MAJOR);
        return ret;
    }
    pr_info("Char device registered with major %d\n", MY_MAJOR);
    return 0;
}

static void __exit mychardev_exit(void)
{
    unregister_chrdev(MY_MAJOR, DEVICE_NAME);
    pr_info("Char device unregistered\n");
}

module_init(mychardev_init);
module_exit(mychardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");
MODULE_DESCRIPTION("Simple Character Device with Static Major Number");

