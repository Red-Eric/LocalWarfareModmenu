
#include "Includes/obfuscate.h"
#include "Menu/Menu.hpp"

bool iconValid = false;
bool settingsValid = false;
bool featuresValid = false;
bool initValid = false;

//Big letter cause crash
void setText(JNIEnv *env, jobject obj, const char* text){
    //https://stackoverflow.com/a/33627640/3763113
    //A little JNI calls here. You really really need a great knowledge if you want to play with JNI stuff
    //Html.fromHtml("");
    jclass html = (*env).FindClass(OBFUSCATE("android/text/Html"));
    jmethodID fromHtml = (*env).GetStaticMethodID(html, OBFUSCATE("fromHtml"), OBFUSCATE("(Ljava/lang/String;)Landroid/text/Spanned;"));

    //setText("");
    jclass textView = (*env).FindClass(OBFUSCATE("android/widget/TextView"));
    jmethodID setText = (*env).GetMethodID(textView, OBFUSCATE("setText"), OBFUSCATE("(Ljava/lang/CharSequence;)V"));

    //Java string
    jstring jstr = (*env).NewStringUTF(text);
    (*env).CallVoidMethod(obj, setText,  (*env).CallStaticObjectMethod(html, fromHtml, jstr));
}

jstring Icon(JNIEnv *env, jobject thiz) {

    //Use https://www.base64encode.org/ to encode your image to base64
    return env->NewStringUTF(
            OBFUSCATE("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"));
}

jstring IconWebViewData(JNIEnv *env, jobject thiz) {

    //WebView support GIF animation. Upload your image or GIF on imgur.com or other sites

    // From internet (Requires android.permission.INTERNET)
    // return env->NewStringUTF(OBFUSCATE("https://i.imgur.com/SujJ85j.gif"));

    // Base64 html:
    // return env->NewStringUTF("data:image/png;base64, <encoded base64 here>");

    // To disable it, return nullptr. It will use normal image above:
    // return nullptr

    //return env->NewStringUTF(OBFUSCATE_KEY("https://i.imgur.com/SujJ85j.gif", 'u'));
    return nullptr;
}

jobjectArray SettingsList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_Settings"),
            OBFUSCATE("-1_Toggle_Save feature preferences"), //-1 is checked on Preferences.java
            OBFUSCATE("-3_Toggle_Auto size vertically"),
            OBFUSCATE("Category_Menu"),
            OBFUSCATE("-6_Button_<font color='red'>Close settings</font>"),
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}
