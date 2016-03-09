#include <jni.h>
#include <dlfcn.h>
#include "modpe.h"

void attack(Player&, Entity&);

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* none)
{
	ModPE* modpe = ModPE::GetInstance();
	modpe->initModPE();
	modpe->attackHook(attack);
	return JNI_VERSION_1_6;
}

void attack(Player& p, Entity& e)
{
	ModPE::GetInstance()->Player_setHealth(20);
}
