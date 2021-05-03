/*
This code was made using information found on stack overflow

This was used to loop through all process's
https://stackoverflow.com/questions/19208487/kernel-module-that-iterates-over-all-tasks-using-depth-first-tree

This was used to build the code to count threads
https://stackoverflow.com/questions/8457890/kernel-how-to-find-all-threads-from-a-processs-task-struct

Retrieve parent ID
https://stackoverflow.com/questions/32040114/retrieve-parent-pid

*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>

struct task_struct *found_task;

void DFS(struct task_struct *task)
{   
    struct task_struct *child;
    struct list_head *list;

    if(strcmp(task->comm, "profemulator") == 0) //when we find a process called profemulator we save a pointer to it in found_task
    {
      found_task = task;
      return;
    }

    list_for_each(list, &task->children) { //recursively loop through all tasks depth first
        child = list_entry(list, struct task_struct, sibling);
        DFS(child);
    }
}

int task_lister_init(void) //init function
{
    printk(KERN_INFO "Loading profemulator info module...\n");
    DFS(&init_task);

    int threadCount = 0;

    if(strcmp(found_task->comm, "profemulator") == 0) //Checking to make sure the proccess was found
    {
        printk("KERN:\n process name: %s\n pid: [%d]\n parent pid: [%d]\n", found_task->comm, found_task->pid, found_task->parent->pid); //info print

        struct task_struct *me = found_task;
        struct task_stuct *t = me;
        do {
            threadCount += 1;
        }while_each_thread(me, t); //counting the number of threads

        printk(" thread count: %d\n", threadCount);
    }

    return 0;
}

void task_lister_exit(void) //exit
{
    printk(KERN_INFO "Exiting profemulator info module\n");
}

module_init(task_lister_init);
module_exit(task_lister_exit);