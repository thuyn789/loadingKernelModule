
/*
*  Author: Tin Huynh
*  Purpose: To load a simple module to linux kernel. The module will create a linked list of 
*           five people, print output to the kernel log, remove the person with longest name,
*           and then remove everyone on the list.
*  Created on Budgie 20.04
*  Tested on Ubuntu 14.04
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday {	
	int month;
	int day;
	int year;
	char name[10];
	struct list_head list;
	
};

/**
 * The following defines and initializes a list_head object named birthday_list
 */
static LIST_HEAD(birthday_list);

int simple_init(void) {
	printk(KERN_INFO "Loading Module\n");

	//Initialize and add elements to the linked list
	struct birthday * list_pointer;

	struct birthday brian;
	list_pointer = &brian;
	list_pointer = kmalloc(sizeof(struct birthday), GFP_KERNEL);
	list_pointer->month = 2;
	list_pointer->day = 15;
	list_pointer->year = 1993;
	strcpy(list_pointer->name, "Brian");
	INIT_LIST_HEAD(&list_pointer->list);
	list_add_tail(&list_pointer->list, &birthday_list);

	struct birthday royce;
	list_pointer = &royce;
	list_pointer = kmalloc(sizeof(struct birthday), GFP_KERNEL);
	list_pointer->month = 6;
	list_pointer->day = 1;
	list_pointer->year = 1997;
	strcpy(list_pointer->name, "Royce");
	INIT_LIST_HEAD(&list_pointer->list);
	list_add_tail(&list_pointer->list, &birthday_list);

	struct birthday michael;
	list_pointer = &michael;
	list_pointer = kmalloc(sizeof(struct birthday), GFP_KERNEL);
	list_pointer->month = 4;
	list_pointer->day = 18;
	list_pointer->year = 1992;
	strcpy(list_pointer->name, "Michael");
	INIT_LIST_HEAD(&list_pointer->list);
	list_add_tail(&list_pointer->list, &birthday_list);

	struct birthday mary;
	list_pointer = &mary;
	list_pointer = kmalloc(sizeof(struct birthday), GFP_KERNEL);
	list_pointer->month = 12;
	list_pointer->day = 27;
	list_pointer->year = 1998;
	strcpy(list_pointer->name, "Mary");
	INIT_LIST_HEAD(&list_pointer->list);
	list_add_tail(&list_pointer->list, &birthday_list);

	struct birthday * antony;
	list_pointer = &antony;
	list_pointer = kmalloc(sizeof(struct birthday), GFP_KERNEL);
	list_pointer->month = 7;
	list_pointer->day = 8;
	list_pointer->year = 1996;
	strcpy(list_pointer->name, "Antony");
	INIT_LIST_HEAD(&list_pointer->list);
	list_add_tail(&list_pointer->list, &birthday_list);
	//done adding elements


	//Traverse the linked list, print all element, and find the longest name
	struct birthday * pointer = NULL;
	char temp[10]; //initializing temporary string for comparison

	list_for_each_entry(pointer, &birthday_list, list) {
		printk(KERN_INFO "BIRTHDAY: Name: %s Month: %d Day: %d Year: %d\n", 
			pointer->name, pointer->month, pointer->day, pointer->year);

		//Find the longest name and store a copy to temp
		if(strlen(temp) < strlen(pointer->name)){
			strcpy(temp, pointer->name);
		}
	}

	//Remove the longest name
	pointer = NULL;
	struct birthday * next = NULL;

	list_for_each_entry_safe(pointer, next, &birthday_list, list) {
		//compare the name in the list with the temporary string 
		if(strcmp(temp, pointer->name) == 0){
			printk(KERN_INFO "Removing Longest Name: %s \n", pointer->name);
			list_del(&pointer->list);
			kfree(pointer);
		}
	}

	//Output after deletion
	pointer = NULL;

	list_for_each_entry(pointer, &birthday_list, list) {
		printk(KERN_INFO "BIRTHDAY: Name: %s Month: %d Day: %d Year: %d\n", 
			pointer->name, pointer->month, pointer->day, pointer->year);
	}
	
	
	printk(KERN_INFO "Done Loading\n");
    return 0;
}

void simple_exit(void) {	
	printk(KERN_INFO "Removing Module\n");

	//Initializing pointers to the linked list
	struct birthday * pointer = NULL; 
	struct birthday * next = NULL;

	//Traverse and remove all element from the list
	list_for_each_entry_safe(pointer, next, &birthday_list, list) {
		printk(KERN_INFO "REMOVING: Name: %s Month: %d Day: %d Year: %d\n",
			pointer->name, pointer->month, pointer->day, pointer->year);

		list_del(&pointer->list); //deletion
		kfree(pointer); //free kernel memory after deletion
	}

	printk(KERN_INFO "Done Removing\n"); //output existing message
}

module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Data Structures");
MODULE_AUTHOR("SGG");
