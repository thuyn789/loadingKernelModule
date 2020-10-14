
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

//This method helps initialize and add elements to the linked list
void add_birthday_kernel(struct birthday * person, int month, int day, int year, char name[10]){
	person = kmalloc(sizeof(struct birthday), GFP_KERNEL);
	person->month = month;
	person->day = day;
	person->year = year;
	strcpy(person->name, name);
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);
}

int simple_init(void) {
	printk(KERN_INFO "Loading Module\n");

	//***Initialize and add elements to the linked list***
	struct birthday brian;
	add_birthday_kernel(&brian, 2, 15, 1993, "Brian");

	struct birthday royce;
	add_birthday_kernel(&royce, 6, 1, 1997, "Royce");

	struct birthday michael;
	add_birthday_kernel(&michael, 4, 18, 1992, "Michael");

	struct birthday mary;
	add_birthday_kernel(&mary, 12, 27, 1998, "Mary");

	struct birthday antony;
	add_birthday_kernel(&antony, 7, 8, 1996,"Antony");
	//***done adding elements***


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
