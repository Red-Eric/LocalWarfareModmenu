#include "AadilTypes.h"
#include "Includes/obfuscate.h"
#include "Includes/Logger.h"
#include <unordered_set>
#include <iostream>

float GetDistance(const Vector3& a, const Vector3& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

Vector3 SubtractVec(const Vector3& target, const Vector3& camPos)
{
    return {
            target.x - camPos.x,
            target.y - camPos.y,
            target.z - camPos.z
    };
}


bool EnableESP, ESPLine;
std::unordered_set<void*> entityList = {};
Vector3 myPosition(0,0,0);

// toggle
bool godMod, infAmmo, aimbot = false;
float humanHeight = 1.8f;
float humanWidth = 1.0f;
void* (*get_transform)(void* object);
Vector3 (*get_position)(void* object);
Vector3 (*get_worldToScreenPoint)(void* object, Vector3 pos);
void* (*get_mainCamera)();
int (*get_height)();
int (*get_width)();
Quaternion (*Euler)(Vector3 euler);
void (*setRotation)(void* characterControllerInstance, Quaternion bodyRotation , Quaternion CameraRotation);
void (*set_rotation_camera)(void* transform, Quaternion value);


Vector2 convertToDeviceScreen(float posX, float posY, float deviceWidth , float deviceHeight, float gameWidth, float gameHeight){
    return Vector2(posX*deviceWidth/gameWidth, deviceHeight - (posY*deviceHeight/gameHeight));
}

//Address
void* localPlayer = nullptr;

// every frame
void DrawESP(AadilDrawing esp, int width, int height) {
    if (!EnableESP) return;
    if(!localPlayer) return;
//    LOGI("Width : %d , height : %d", get_width(), get_height());
    void* camera = get_mainCamera();
    if(!camera) return;
    Vector2 drawFrom = Vector2(get_width()/2 , 0);
    entityList.erase(localPlayer);

    // Get LocalPlayerPosition


    if (!entityList.empty()) {

        if(localPlayer){
            void* localPlayerTransform = get_transform(localPlayer);
            if(localPlayerTransform){
                myPosition = get_position(localPlayerTransform);
            }
        }

        int i = 0;

        for (void* entity : entityList) {
            bool isAlive = true;
            float* hp = (float*)((uintptr_t)entity + 0x58);
            if(*hp <= 0)
                isAlive = false;
            void* transform = get_transform(entity);
            if(transform){
                Vector3 posBottom = get_position(transform);
                Vector3 screenPosBottomVec = get_worldToScreenPoint(camera, posBottom);

                Vector3 posTop = get_position(transform);
                posTop.y = posTop.y + humanHeight;
                Vector3 screenPosTopVec = get_worldToScreenPoint(camera, posTop);

                // distance
                float  dist = GetDistance(myPosition, posBottom);
                // Aimbot


                // ESP draw
                if(screenPosBottomVec.z > 0 && screenPosTopVec.z > 0){


                    if(aimbot){
                        void* camTransform = get_transform(camera);
                        void* localPlayerTransform = get_transform(localPlayer);
                        if(camTransform){
                            Vector3 camPos = get_position(camTransform);
                            Vector3 targetPos = get_position(transform);
                            targetPos.y += 1;
                            Vector3 dir = SubtractVec(targetPos, camPos);
                            float distXZ = std::sqrtf(dir.x * dir.x + dir.z * dir.z);
                            float yaw = std::atan2(dir.x, dir.z) * (180.0f / 3.1415);
                            float pitch = -std::atan2(dir.y, distXZ) * 57.295779513f;
                            Quaternion aimRotation = Euler({pitch, yaw, 0});

                            if(localPlayer){
                                void* firstPlayerController = *(void**)((uintptr_t)localPlayer + 0x40);
                                if(firstPlayerController){
                                    if(infAmmo){
                                        Quaternion bodyRot   = Euler({0.0f, yaw, 0.0f});
                                        Quaternion cameraRot = Euler({pitch, 0.0f, 0.0f});
                                        setRotation(firstPlayerController, bodyRot, cameraRot);
                                    }else{
                                        setRotation(firstPlayerController, aimRotation, aimRotation);
                                    }
                                }
                            }

//                            set_rotation_camera(camTransform, aimRotation);
//                            set_rotation_camera(localPlayerTransform, aimRotation);
                        }
                    }


                    Vector2 screenPosBottom = convertToDeviceScreen(screenPosBottomVec.x, screenPosBottomVec.y, width, height, get_width(), get_height());
                    Vector2 screenPosTop = convertToDeviceScreen(screenPosTopVec.x, screenPosTopVec.y, width, height, get_width(), get_height());
                    if(EnableESP && isAlive){
                        esp.DrawLine({255,0,0,255}, 3.0f, drawFrom, screenPosTop);
//                        esp.DrawBox({255,0,0,255},3.0f, {screenPosTop.x - humanWidth, screenPosTop.y}, {screenPosBottom.x + humanWidth , screenPosBottom.y});

                        float height11 = screenPosBottom.y - screenPosTop.y;
                        float width11  = height11 * 0.45f;
                        esp.DrawBox(
                                {255, 0, 0, 255},
                                3.0f,
                                {screenPosTop.x, screenPosTop.y},
                                width11,
                                height11,
                                (int)*hp,
                                dist
                        );
                    }
                }

            }
            i++;
        }
    }

}

// getLocalPlayerInstance

void (*old_onStartLocalPlayer)(void* instance);
void onStartLocalPlayer(void* instance){
    if(instance){
        localPlayer = instance;
        LOGI("onStartLocalPlayer called : %p", instance);

    }
    old_onStartLocalPlayer(instance);
}


// Gun Update

void(*old_updateGun)(void* instance);
void updateGun(void* instance){
    if(godMod && localPlayer && instance){
        float* hp = (float*)((uintptr_t )localPlayer + 0x58);
        *hp = 1000;
    }

    old_updateGun(instance);
}

// baseCharactereController

void (*old_updateBaseCharacter)(void* instance);
void updateBaseCharacter (void* instance){ // OnStartServer
    if(instance){
        entityList.insert(instance);
    }
    old_updateBaseCharacter(instance);
}

// fireHook (debug)
void (*old_fire)(void* gun);
void fire(void* gun){
    if(gun && infAmmo){
        int* ammo = (int*)((uintptr_t )gun + 0xAC);
        *ammo = 1000;
    }

    old_fire(gun);
}

// on destroy

void (*old_onDestroy)(void* instance);
void onDestroy(void* instance){
    if(instance){
        entityList.clear();
        LOGI("EntityList Clear");
        if(localPlayer == instance){
            localPlayer == nullptr;
        }
    }
    old_onDestroy(instance);
}
