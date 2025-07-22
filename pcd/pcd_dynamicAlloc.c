#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/kdev_t.h>

// #define MAJOR_NUMBER 255   
#define MEM_SIZE 256       

char buffer[MEM_SIZE];  
dev_t pcd_num;

static const struct file_operations pcd_fops;
struct cdev pcd;

static void pcd_open(struct inode *inode, struct file *file){
    pr_info("pcd: Device opened\n");
} 

static void pcd_release(struct inode *inode, struct file *file) {
    pr_info("pcd: Device closed\n");
}







static const struct file_operations pcd_fops(){
    // .owner 
    .open = pcd_open,
    .release = pcd_release,
    .write = pcd_write,
    .read = pcd_read,
}

static int __init pcd_init(void){

    // Dynamically Allocate Device Number
    alloc_chrdev_region(&pcd_num,0,1,"pcd_nugget");

    // Initialize 
    //pcd.owner = THIS_MODULE;
    cdev_init(&pcd,&pcd_fops);

    // Registering with kernal VFS
    cdev_add(&pcd,pcd_num,1);

    pr_info("pseudo char driver initiated...");
    return 0;
}

static void __exit pcd_init(void){
    pr_info("pseudo char driver exiting...");s
}




module_init(xxx);
module_exit();

MODULE_LICENSE("GPL");
MODULE_AUTHOR("nugget");
MODULE_DESCRIPTION("Pseudo Char Driver Implementation - by dynamically allocating device number");