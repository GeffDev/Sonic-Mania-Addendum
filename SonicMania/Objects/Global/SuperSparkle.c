// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SuperSparkle Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSuperSparkle *SuperSparkle;

void SuperSparkle_Update(void)
{
    RSDK_THIS(SuperSparkle);

    EntityPlayer *player = self->player;

    if (!player) {
        destroyEntity(self);
    }
    else {
        if (player->groundedStore)
            self->canSpawnSparkle = abs(player->velocity.y) + abs(player->velocity.x) > TO_FIXED(6);

        if (self->canSpawnSparkle) {
            if (++self->timer == 12) {
                self->timer = 0;

                EntityDebris *sparkle = CREATE_ENTITY(Debris, NULL, player->position.x, player->position.y);
                sparkle->state        = Debris_State_Move;
                sparkle->timer        = 16;
                sparkle->inkEffect    = INK_ADD;
                sparkle->alpha        = 0x100;
                sparkle->drawGroup    = Zone->objectDrawGroup[1];
                sparkle->drawGroup    = player->drawGroup;
                RSDK.SetSpriteAnimation(SuperSparkle->aniFrames, 0, &sparkle->animator, true, 0);
            }
        }
        else {
            self->timer = 0;
        }

        if (!(Zone->timer & 7)) {
            int32 x = player->position.x + RSDK.Rand(-TO_FIXED(12), TO_FIXED(12));
            int32 y = player->position.y + RSDK.Rand(-TO_FIXED(18), TO_FIXED(18));

            EntityRing *sparkle = CREATE_ENTITY(Ring, NULL, x, y);
            sparkle->state      = Ring_State_Sparkle;
            sparkle->stateDraw  = Ring_Draw_Sparkle;
            sparkle->active     = ACTIVE_NORMAL;
            sparkle->visible    = false;
            sparkle->velocity.y = -TO_FIXED(1);
            sparkle->drawGroup  = player->drawGroup;
            switch (player->characterID) {
                case ID_SONIC: RSDK.SetSpriteAnimation(SuperSparkle->superFrames, Zone->timer % 3, &sparkle->animator, true, 0); break;
                case ID_TAILS: RSDK.SetSpriteAnimation(SuperSparkle->superFrames, Zone->timer % 3 + 3, &sparkle->animator, true, 0); break;
                case ID_KNUCKLES: RSDK.SetSpriteAnimation(SuperSparkle->superFrames, Zone->timer % 3 + 6, &sparkle->animator, true, 0); break;
                case ID_MIGHTY: RSDK.SetSpriteAnimation(SuperSparkle->superFrames, Zone->timer % 3 + 9, &sparkle->animator, true, 0); break;
                case ID_RAY: RSDK.SetSpriteAnimation(SuperSparkle->superFrames, Zone->timer % 3 + 12, &sparkle->animator, true, 0); break;
                case ID_AMY: RSDK.SetSpriteAnimation(SuperSparkle->superFrames, Zone->timer % 3 + 15, &sparkle->animator, true, 0); break;
            }
            int32 cnt = sparkle->animator.frameCount;
            switch (sparkle->animator.animationID) {
                case 0:
                case 3:
                case 6:
                case 9:
                case 12:
                case 15:
                sparkle->alpha = 0xE0;
                cnt >>= 1;
                break;
            }
            sparkle->maxFrameCount  = cnt - 1;
            sparkle->animator.speed = RSDK.Rand(6, 8);
        }

        if (player->superState != SUPERSTATE_SUPER || player->active == ACTIVE_NEVER)
            destroyEntity(self);
    }
}

void SuperSparkle_LateUpdate(void) {}

void SuperSparkle_StaticUpdate(void) {}

void SuperSparkle_Draw(void) {}

void SuperSparkle_Create(void *data)
{
    RSDK_THIS(SuperSparkle);

    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_NORMAL;
        self->player = (EntityPlayer *)data;
    }
}

void SuperSparkle_StageLoad(void)
{
    SuperSparkle->aniFrames   = RSDK.LoadSpriteAnimation("Global/SuperSparkle.bin", SCOPE_STAGE);
    SuperSparkle->superFrames = RSDK.LoadSpriteAnimation("Global/SuperParticles.bin", SCOPE_STAGE);
}

#if GAME_INCLUDE_EDITOR
void SuperSparkle_EditorDraw(void)
{
    Animator animator;
    RSDK.SetSpriteAnimation(SuperSparkle->aniFrames, 0, &animator, true, 3);

    RSDK.DrawSprite(&animator, NULL, false);
}

void SuperSparkle_EditorLoad(void)
{
    SuperSparkle->aniFrames   = RSDK.LoadSpriteAnimation("Global/SuperSparkle.bin", SCOPE_STAGE);
    SuperSparkle->superFrames = RSDK.LoadSpriteAnimation("Global/SuperParticles.bin", SCOPE_STAGE);
}
#endif

void SuperSparkle_Serialize(void) {}
