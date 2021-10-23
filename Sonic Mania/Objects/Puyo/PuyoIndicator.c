#include "SonicMania.h"

ObjectPuyoIndicator *PuyoIndicator;

void PuyoIndicator_Update(void)
{
    RSDK_THIS(PuyoIndicator);

    entity->visible = false;
    if (entity->state) {
        StateMachine_Run(entity->state);
        RSDK.ProcessAnimation(&entity->animator);
    }
}

void PuyoIndicator_LateUpdate(void) {}

void PuyoIndicator_StaticUpdate(void) {}

void PuyoIndicator_Draw(void)
{
    RSDK_THIS(PuyoIndicator);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PuyoIndicator_Create(void *data)
{
    RSDK_THIS(PuyoIndicator);
    entity->active     = ACTIVE_NORMAL;
    entity->drawOrder  = Zone->drawOrderHigh;
    entity->startPos.x = entity->position.x;
    entity->startPos.y = entity->position.y;
    entity->visible    = true;
    entity->drawFX     = FX_SCALE | FX_FLIP;
    entity->scale.x    = 0x200;
    entity->scale.y    = 0x200;
}

void PuyoIndicator_StageLoad(void) { PuyoIndicator->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoIndicator.bin", SCOPE_STAGE); }

void PuyoIndicator_Unknown1(void)
{
    RSDK_THIS(PuyoIndicator);

    entity->scale.x = 0x200;
    entity->scale.y = 0x200;
    entity->visible = true;
    RSDK.SetSpriteAnimation(PuyoIndicator->aniFrames, 0, &entity->animator, false, 0);
    entity->position = entity->startPos;
    entity->scale.x  = 0x200;
    entity->scale.y  = 0x200;
    entity->scale.x += (RSDK.Sin512(8 * Zone->timer) >> 3) + 32;
    entity->scale.y += (RSDK.Sin512(8 * Zone->timer) >> 3) + 32;
}

void PuyoIndicator_Unknown2(void)
{
    RSDK_THIS(PuyoIndicator);

    entity->scale.x = 0x200;
    entity->scale.y = 0x200;
    entity->visible = true;
    RSDK.SetSpriteAnimation(PuyoIndicator->aniFrames, 1, &entity->animator, false, 0);
    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y + 0x20000;
    entity->position.y += RSDK.Sin256(4 * Zone->timer) << 10;
}

void PuyoIndicator_Unknown3(void)
{
    RSDK_THIS(PuyoIndicator);

    entity->scale.x = 0x200;
    entity->scale.y = 0x200;
    entity->visible = true;
    RSDK.SetSpriteAnimation(PuyoIndicator->aniFrames, 2, &entity->animator, false, 0);
    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
}

void PuyoIndicator_EditorDraw(void) {}

void PuyoIndicator_EditorLoad(void) {}

void PuyoIndicator_Serialize(void) { RSDK_EDITABLE_VAR(PuyoIndicator, VAR_UINT8, playerID); }
