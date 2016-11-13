#include <linux/kernel.h>

#include "printa.h"

int a = 5;

void printa(void)
{
	pr_info("a = %d, print defined in printa.c\n", a);
}
