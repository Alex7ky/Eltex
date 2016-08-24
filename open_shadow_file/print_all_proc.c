#include <linux/module.h>
#include <linux/sched.h>
#include <linux/export.h>
#include <linux/slab.h>
#include <linux/key.h>
#include <linux/keyctl.h>
#include <linux/init_task.h>
#include <linux/security.h>
#include <linux/binfmts.h>
#include <linux/cn_proc.h>

#define DRIVER_AUTHOR "Alexandr Stolpovsky <stolpovsky7@yandex.ru>"
#define DRIVER_DESC   "A sample driver"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);

static int my_pid;
module_param(my_pid, int, 0);

static int __init print_all_proc_init(void)
{
	struct task_struct *p;
	struct task_struct *tmp_task = 0;
	struct cred __rcu *tmp_cred;
	int flg = 0;

	/* Ищем процесс с нужным pid'ом */
	for (p = &init_task ; (p = list_entry_rcu((p)->tasks.next,
	     struct task_struct, tasks)) != &init_task ;) {
		if (p->pid == my_pid) {
			tmp_task = p;
			pr_info("======= The process was found! =======\n");
			pr_info("=== NAME - %s\n", p->comm);
			pr_info("=== UID - %d\n", p->cred->uid.val);
			pr_info("=== GID - %d\n", p->cred->gid.val);
			pr_info("=== SUID - %d\n", p->cred->suid.val);
			pr_info("=== SGID - %d\n", p->cred->sgid.val);
			pr_info("=== EUID - %d\n", p->cred->euid.val);
			pr_info("=== EGID - %d\n", p->cred->egid.val);
			pr_info("=== FSUID - %d\n", p->cred->fsuid.val);
			pr_info("=== FSGID - %d\n", p->cred->fsgid.val);
			pr_info("============================== =======\n");
			flg = 1;
		}
	}

	if (flg == 1) {
		/** Т.к. мы не можем менять
		 *  на прямую значения p->cred
		 *  поэтому склонируем структуру cred
		 *  функция clone_cred возвращает
		 *  указатель на на структуру cred
		 *  без спецификатора const!
		 */
		tmp_cred = clone_cred(tmp_task->cred);

		/** Предоставляем права
		 *  нашему процессу
		 */
		tmp_cred->fsuid.val = 0;
		tmp_cred->fsgid.val = 0;

		/** После изменения перезаписываем
		 *  значение cred нашей
		 *  структуры tmp_task.
		 */
		tmp_task->cred = tmp_cred;

		pr_info("======= Process change parameters =======\n");
		pr_info("=== NAME - %s\n",  tmp_task->comm);
		pr_info("=== UID - %d\n",   tmp_task->cred->uid.val);
		pr_info("=== GID - %d\n",   tmp_task->cred->gid.val);
		pr_info("=== SUID - %d\n",  tmp_task->cred->suid.val);
		pr_info("=== SGID - %d\n",  tmp_task->cred->sgid.val);
		pr_info("=== EUID - %d\n",  tmp_task->cred->euid.val);
		pr_info("=== EGID - %d\n",  tmp_task->cred->egid.val);
		pr_info("=== FSUID - %d\n", tmp_task->cred->fsuid.val);
		pr_info("=== FSGID - %d\n", tmp_task->cred->fsgid.val);
		pr_info("============================== =======\n");
	} else
		pr_info("Process not found!\n");

	return 0;
}

static void __exit print_all_proc_fini(void)
{
}

module_init(print_all_proc_init);
module_exit(print_all_proc_fini);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");
