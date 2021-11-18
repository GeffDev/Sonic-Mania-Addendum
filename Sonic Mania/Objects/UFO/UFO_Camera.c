#include "SonicMania.h"

ObjectUFO_Camera *UFO_Camera;

void UFO_Camera_Update(void) {}

void UFO_Camera_LateUpdate(void)
{
    RSDK_THIS(UFO_Camera);
    StateMachine_Run(self->state);
    UFO_Camera_Unknown1();
}

void UFO_Camera_StaticUpdate(void) {}

void UFO_Camera_Draw(void) {}

void UFO_Camera_Create(void *data)
{
    RSDK_THIS(UFO_Camera);
    self->active   = ACTIVE_NORMAL;
    self->state    = UFO_Camera_Unknown2;
    self->height   = 0x300000;
    self->field_64 = 0x2800;

    foreach_all(UFO_Player, player)
    {
        player->camera = self;
        self->target = (Entity *)player;
    }
}

void UFO_Camera_StageLoad(void)
{
    RSDK.ResetEntitySlot(SLOT_UFO_CAMERA, UFO_Camera->objectID, 0);
    UFO_Camera->isSS7 = RSDK.CheckStageFolder("UFO7");
}

void UFO_Camera_Unknown1(void)
{
    RSDK_THIS(UFO_Camera);
    int32 val = RSDK.Cos1024(-self->angleX) << 12;
    if (val < 0x3C0000)
        val = 0x3C0000;

    if (!val)
        val = 1;
    int32 div  = !val ? 1 : val;
    int32 val2 = self->angle - self->dword6C;
    int32 val3 = val2 - 0x400;
    if (self->angle <= 0x200)
        val3 = val2 + 0x400;

    if (abs(val2) >= abs(val3))
        ScreenInfo->position.x -= 2 * val3;
    else
        ScreenInfo->position.x -= 2 * val2;

    int32 offset               = ((RSDK.Sin1024(-self->angleX) << 12) << 8) / div;
    ScreenInfo->position.y = offset - ScreenInfo->centerY + 512;
    self->dword6C          = self->angle;

    self->clipY = clampVal(ScreenInfo->centerY - offset + 8, -0x40, ScreenInfo->height);
}

void UFO_Camera_Unknown2(void)
{
    RSDK_THIS(UFO_Camera);
    EntityUFO_Player *target = (EntityUFO_Player *)self->target;
    if (target) {
        int32 angle2   = (-0x200 - target->angle) & 0x3FF;
        int32 angleDif = angle2 - self->angle;

        int32 val1, val2, val3;
        if (angleDif >= 0)
            val1 = angle2 - self->angle;
        else
            val1 = self->angle - angle2;

        if (angleDif + 0x400 >= 0)
            val2 = angleDif + 0x400;
        else
            val2 = self->angle - angle2 - 0x400;

        if (angleDif - 0x400 >= 0)
            val3 = angleDif - 0x400;
        else
            val3 = self->angle - angle2 + 0x400;
        if (val1 >= val3) {
            if (val3 < val2) {
                self->angle = self->angle + ((angleDif - 0x400) >> 3);
            }
            else {
                self->angle = self->angle + ((angleDif + 0x400) >> 3);
            }
        }
        else if (val1 < val2) {
            self->angle = self->angle + (angleDif >> 3);
        }
        else {
            self->angle = self->angle + ((angleDif + 0x400) >> 3);
        }

        self->angle &= 0x3FF;
        if (target->state == UFO_Player_HandleTilt) {
            self->angleX = -(target->height >> 18);
            int32 val         = self->field_64 * RSDK.Cos1024(self->angleX) >> 10;

            self->position.x = target->position.x - val * RSDK.Sin1024(self->angle);
            self->position.y = target->position.y - val * RSDK.Cos1024(self->angle);

            self->height = (target->height >> 1) - (RSDK.Sin1024(self->angleX) << 14) + 0x400000;
        }
        else {
            self->angleX    = 0;
            self->position.x = target->position.x - self->field_64 * RSDK.Sin1024(self->angle);
            self->position.y = target->position.y - self->field_64 * RSDK.Cos1024(self->angle);
            self->height     = (target->height >> 2) + 0x400000;
        }
    }

    RSDK.MatrixTranslateXYZ(&self->matWorld, -self->position.x, -self->height, -self->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, self->angleX, self->angle, 0);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &self->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&self->matWorld, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &self->matWorld);
    if (UFO_Camera->isSS7) {
        RSDK.MatrixRotateXYZ(&UFO_Camera->matTemp, self->angleX + 4 * UFO_Setup->timer, self->angle, 0);
        RSDK.MatrixMultiply(&UFO_Camera->matTemp, &UFO_Camera->matTemp, &self->matWorld);
    }
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &self->matWorld);
}

void UFO_Camera_Unknown3(void)
{
    RSDK_THIS(UFO_Camera);
    self->height += 0x20000;

    if (self->angleX > -0x100)
        self->angleX -= 4;

    self->angle += 2;
    RSDK.MatrixTranslateXYZ(&self->matWorld, -self->position.x, -self->height, -self->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, self->angleX, self->angle, 0);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &self->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&self->matWorld, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &self->matWorld);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &self->matWorld);
}

void UFO_Camera_Unknown4(void)
{
    RSDK_THIS(UFO_Camera);
    self->height += 0x80000;
    if (self->angleX > -0x100)
        self->angleX -= 8;

    self->position.x += RSDK.Sin1024(self->angle) << 8;
    self->position.y += RSDK.Cos1024(self->angle) << 8;
    RSDK.MatrixTranslateXYZ(&self->matWorld, -self->position.x, -self->height, -self->position.y, true);
    RSDK.MatrixRotateXYZ(&UFO_Camera->matView, self->angleX, self->angle, 0);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &self->matWorld, &UFO_Camera->matView);
    RSDK.MatrixScaleXYZ(&self->matWorld, -0x100, 0x100, 0x100);
    RSDK.MatrixMultiply(&UFO_Camera->matView, &UFO_Camera->matView, &self->matWorld);
    RSDK.MatrixMultiply(&UFO_Camera->matWorld, &UFO_Camera->matWorld, &self->matWorld);
}

#if RETRO_INCLUDE_EDITOR
void UFO_Camera_EditorDraw(void) {}

void UFO_Camera_EditorLoad(void) {}
#endif

void UFO_Camera_Serialize(void) {}
