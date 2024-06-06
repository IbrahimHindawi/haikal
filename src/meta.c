/*
 * Metaprogramming Data Structures & Algorithms in C by
 * creating an external program `meta` that parses C source
 * and replaces the `TYPE` token with a desired type.
 * for core types -> `metacore()` -> `#include "hkType_core.h"
 * for new types -> `metagen()` -> 
 *      `#include "custom.h"`
 *      `#include "hkType_custom.h"`
 */
#ifdef _MSC_VER
#   define _CRT_SECURE_NO_WARNINGS
#endif
#include "meta.h"

int main() {
    printf("Haikal code gen initiated...\n");
    // metacore("hkNode");
    // metacore("hkList");
    // metacore("hkQueue");
    metainit("hkArray");
    metacore("hkArray");
    metagen("hkArray", "VkLayerProperties");
    metagen("hkArray", "VkExtensionProperties");
    metagen("hkArray", "VkExtensionProperties");
    metagen("hkArray", "VkPhysicalDevice");
    metagen("hkArray", "VkImage");
    metagen("hkArray", "VkImageView");
    metagen("hkArray", "VkQueueFamilyProperties");
    metagen("hkArray", "VkDeviceQueueCreateInfo");
    metagen("hkArray", "VkExtensionProperties");
    metagen("hkArray", "VkSurfaceFormatKHR");
    metagen("hkArray", "VkPresentModeKHR");

    return 0;
}
