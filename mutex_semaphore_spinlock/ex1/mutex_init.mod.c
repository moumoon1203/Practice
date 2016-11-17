#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x96cec1da, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xdc5b557, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x3741f64c, __VMLINUX_SYMBOL_STR(platform_device_unregister) },
	{ 0x9ffcb8b1, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x2e32a83a, __VMLINUX_SYMBOL_STR(platform_device_register_full) },
	{ 0xf1f5d77d, __VMLINUX_SYMBOL_STR(devm_kfree) },
	{ 0xf7c2cca1, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x992d89c6, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x3fcc7bf4, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0xb9901c6e, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x48d4c96e, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x7819011c, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xf8509e7d, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xe67759ca, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("platform:xxxx-test");

MODULE_INFO(srcversion, "6AF45EF407FC9B006FD3BB1");