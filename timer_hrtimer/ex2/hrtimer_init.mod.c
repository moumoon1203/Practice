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
	{ 0xa611f0a8, __VMLINUX_SYMBOL_STR(hrtimer_forward) },
	{ 0x98fb9fea, __VMLINUX_SYMBOL_STR(hrtimer_start_range_ns) },
	{ 0xb1deb238, __VMLINUX_SYMBOL_STR(hrtimer_init) },
	{ 0x48d4c96e, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0xe67759ca, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x3a927111, __VMLINUX_SYMBOL_STR(hrtimer_cancel) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("platform:xxxx-test");

MODULE_INFO(srcversion, "44B681BC2CE71A7C702A840");
