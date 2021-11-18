#include "SonicMania.h"

ObjectPuyoAttack *PuyoAttack;

void PuyoAttack_Update(void)
{
    RSDK_THIS(PuyoAttack);

    if (self->delay > 0) {
        if (--self->delay)
            return;
        self->visible = true;
    }

    RSDK.ProcessAnimation(&self->animator);
    int angle = RSDK.ATan2((self->targetPos.x - self->position.x) >> 16, (self->targetPos.y - self->position.y) >> 16);
    int rot   = 2 * angle;
    int val   = rot - self->rotation;

    if (abs(rot - self->rotation) >= abs(val - 0x200)) {
        if (abs(val - 0x200) < abs(val + 0x200))
            self->rotation += ((val - 512) >> 3);
        else
            self->rotation += ((val + 512) >> 3);
    }
    else if (abs(rot - self->rotation) >= abs(val + 0x200))
        self->rotation += ((val + 512) >> 3);
    else
        self->rotation += (val >> 3);

    self->rotation &= 0x1FF;
    self->position.x += self->radius * RSDK.Cos512(self->rotation);
    self->position.y += self->radius * RSDK.Sin512(self->rotation);
    if (self->radius < 1024)
        self->radius += 8;
    if (self->position.y < self->targetPos.y) {
        PuyoMatch_StartPuyoAttack(self->playerID, self->score);
        destroyEntity(self);
    }
}

void PuyoAttack_LateUpdate(void) {}

void PuyoAttack_StaticUpdate(void) {}

void PuyoAttack_Draw(void)
{
    RSDK_THIS(PuyoAttack);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PuyoAttack_Create(void *data)
{
    RSDK_THIS(PuyoAttack);
    if (!SceneInfo->inEditor) {
        self->delay     = 16;
        self->radius    = 512;
        self->drawOrder = Zone->hudDrawOrder;
        self->playerID  = voidToInt(data);
        if (!self->playerID)
            self->rotation = 0x40;
        else
            self->rotation = 0x140;
        self->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(PuyoAttack->aniFrames, self->playerID ^ 1, &self->animator, true, 0);
    }
}

void PuyoAttack_StageLoad(void)
{
    PuyoAttack->aniFrames = RSDK.LoadSpriteAnimation("Puyo/Combos.bin", SCOPE_STAGE);
    PuyoAttack->sfxAttack = RSDK.GetSFX("Puyo/Attack.wav");
}

#if RETRO_INCLUDE_EDITOR
void PuyoAttack_EditorDraw(void)
{
    RSDK_THIS(PuyoAttack);
    RSDK.SetSpriteAnimation(PuyoAttack->aniFrames, 0, &self->animator, true, 3);

    PuyoAttack_Draw();
}

void PuyoAttack_EditorLoad(void) { PuyoAttack->aniFrames = RSDK.LoadSpriteAnimation("Puyo/Combos.bin", SCOPE_STAGE); }
#endif

void PuyoAttack_Serialize(void) {}
