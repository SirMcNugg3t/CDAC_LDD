#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/wait.h>

#define DEVICE_NAME "simplechar"
#define NUM_DEVICES 2
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashank");

static dev_t dev_num;
static struct cdev my_cdev;

struct my_device {
    char *buffer;
    int head, tail;
    struct mutex lock;
    wait_queue_head_t read_queue;
    int device_open;
};

static struct my_device devices[NUM_DEVICES];

static int my_open(struct inode *inode, struct file *file)
{
    int minor = iminor(inode);

    if (minor >= NUM_DEVICES)
        return -ENODEV;

    struct my_device *dev = &devices[minor];

    if (dev->device_open)
        return -EBUSY;

    dev->device_open = 1;

    if (!dev->buffer)
        dev->buffer = kzalloc(BUFFER_SIZE, GFP_KERNEL);

    if (!dev->buffer)
        return -ENOMEM;

    file->private_data = dev;

    pr_info("Device %d opened\n", minor);
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    struct my_device *dev = file->private_data;

    dev->device_open = 0;
    pr_info("Device %d closed\n", iminor(inode));
    return 0;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    struct my_device *dev = file->private_data;
    size_t i = 0;

    mutex_lock(&dev->lock);
    while (i < count && ((dev->head + 1) % BUFFER_SIZE != dev->tail)) {
        if (get_user(dev->buffer[dev->head], &buf[i])) {
            mutex_unlock(&dev->lock);
            return -EFAULT;
        }
        dev->head = (dev->head + 1) % BUFFER_SIZE;
        i++;
    }
    mutex_unlock(&dev->lock);

    wake_up_interruptible(&dev->read_queue);
    return i;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    struct my_device *dev = file->private_data;
    size_t i = 0;

    // Block if no data available
    if (dev->head == dev->tail) {
        pr_info("Read blocked, waiting...\n");
        if (wait_event_interruptible(dev->read_queue, dev->head != dev->tail))
            return -ERESTARTSYS;
    }

    mutex_lock(&dev->lock);
    while (i < count && dev->head != dev->tail) {
        if (put_user(dev->buffer[dev->tail], &buf[i])) {
            mutex_unlock(&dev->lock);
            return -EFAULT;
        }
        dev->tail = (dev->tail + 1) % BUFFER_SIZE;
        i++;
    }
    mutex_unlock(&dev->lock);

    return i;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write
};

static int __init simplechar_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, NUM_DEVICES, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate char device region\n");
        return ret;
    }

    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

    ret = cdev_add(&my_cdev, dev_num, NUM_DEVICES);
    if (ret) {
        unregister_chrdev_region(dev_num, NUM_DEVICES);
        pr_err("Failed to add cdev\n");
        return ret;
    }

    for (int i = 0; i < NUM_DEVICES; i++) {
        mutex_init(&devices[i].lock);
        init_waitqueue_head(&devices[i].read_queue);
        devices[i].head = 0;
        devices[i].tail = 0;
        devices[i].device_open = 0;
        devices[i].buffer = NULL;
    }

    pr_info("simplechar driver loaded with major %d\n", MAJOR(dev_num));
    return 0;
}

static void __exit simplechar_exit(void)
{
    for (int i = 0; i < NUM_DEVICES; i++) {
        kfree(devices[i].buffer);
    }

    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, NUM_DEVICES);
    pr_info("simplechar driver unloaded\n");
}

module_init(simplechar_init);
module_exit(simplechar_exit);

