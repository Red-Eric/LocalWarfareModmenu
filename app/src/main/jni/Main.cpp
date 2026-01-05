#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <string>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.hpp"
#include "Menu/Menu.hpp"
#include "Menu/Jni.hpp"
#include "Includes/Macros.h"
#include "Menu/Setup.h"


jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("10_ButtonOnOff_Enable ESP"),
            OBFUSCATE("20_ButtonOnOff_God Mod"),
            OBFUSCATE("30_ButtonOnOff_Infinite Ammo"),
            OBFUSCATE("40_ButtonOnOff_Aimbot")
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

bool btnPressed = false;

void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {

    switch (featNum) {
        case 10:
            EnableESP = boolean;
            break;
        case 20:
            godMod = boolean;
            break;
        case 30:
            infAmmo = boolean;
            break;
        case 40:
            aimbot = boolean;
            break;

    }
}

#define targetLibName OBFUSCATE("libil2cpp.so")

ElfScanner g_il2cppELF;

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));


    do {
        sleep(1);

        g_il2cppELF = ElfScanner::createWithPath(targetLibName);
    } while (!g_il2cppELF.isValid());

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__)

#elif defined(__arm__) // 32bit
    HOOK(targetLibName, str2Offset(OBFUSCATE("0x58BB98")), onStartLocalPlayer, old_onStartLocalPlayer);
    HOOK(targetLibName, str2Offset(OBFUSCATE("0x59709C")), updateGun, old_updateGun);
    HOOK(targetLibName, str2Offset(OBFUSCATE("0x58C148")), updateBaseCharacter, old_updateBaseCharacter);
    HOOK(targetLibName, str2Offset(OBFUSCATE("0x58C368")), onDestroy, old_onDestroy);
    HOOK(targetLibName, str2Offset(OBFUSCATE("0x597838")), fire, old_fire);

    get_transform = (void* (*)(void *)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1BBE5D0")));
    get_position = (Vector3 (*)(void *)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1BC6D84")));
    get_mainCamera = (void* (*)()) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1BB9878")));
    get_worldToScreenPoint = (Vector3 (*)(void *, Vector3)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1BB92B4")));
    get_height = (int (*)()) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1BB5120")));
    get_width = (int (*)()) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1BB50D8")));
    Euler = (Quaternion (*)(Vector3)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1B963A0")));
    setRotation = (void (*)(void*, Quaternion, Quaternion)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x59D604")));
    set_rotation_camera = (void (*)(void*, Quaternion)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1BC728C")));

#endif

    LOGI(OBFUSCATE("DOne"));
    return nullptr;
}

__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}
