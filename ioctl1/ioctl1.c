#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <stdlib.h>
#include <stdio.h>

#define DEVICE_FILE_NAME "/dev/mydev"

#define IOC_MAGIC 'g'
#define OP_READ   _IOR(IOC_MAGIC, 0, char *)
#define OP_WRITE  _IOW(IOC_MAGIC, 1, char *)

/**
 * Функции работы чтения/записи с драйвером через ioctl
 */
void ioctl_set_msg(int file_desc, char *message)
{
	int ret_val;

	ret_val = ioctl(file_desc, OP_READ, message);

	if (ret_val < 0) {
		printf("Ошибка при вызове ioctl_set_msg: %d\n", ret_val);
		exit(-1);
	}
}

void ioctl_get_msg(int file_desc)
{
	int ret_val;
	char message[100];

	ret_val = ioctl(file_desc, OP_WRITE, message);

	if (ret_val < 0) {
		printf("Ошибка при вызове ioctl_get_msg: %d\n", ret_val);
		exit(-1);
	}

	printf("Получено сообщение (get_msg): %s\n", message);
}


int main(void)
{
	int file_desc, ret_val;
	char *msg = "Hello kernel, I'm user\n";

	file_desc = open(DEVICE_FILE_NAME, O_RDWR);

	if (file_desc < 0) {
		printf("Невозможно открыть файл устройства: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}
	
	/* Читаем сообщения с ядра */
	ioctl_get_msg(file_desc);
	/* Отправляем сообщение ядру */
	ioctl_set_msg(file_desc, msg);

	close(file_desc);

	return 0;
}
     
