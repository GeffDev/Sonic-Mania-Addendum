#include "SonicMania.h"

ObjectBallHog *BallHog;

void BallHog_Update(void)
{
    RSDK_THIS(BallHog);

    StateMachine_Run(self->state);
}

void BallHog_LateUpdate(void) {}

void BallHog_StaticUpdate(void) {}

void BallHog_Draw(void)
{
    RSDK_THIS(BallHog);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void BallHog_Create(void *data)
{
    RSDK_THIS(BallHog);

    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(BallHog->aniFrames, 4, &self->animator, true, 0);
        self->state = BallHog_State_Bomb;
    }
    else {
        self->startPos = self->position;
        self->startDir = self->direction;
        if (!self->bombTime)
            self->bombTime = 6;
        if (!self->numJumps)
            self->numJumps = 3;

        self->drawFX |= FX_FLIP;
        self->jumpCount = self->numJumps;
        self->timer     = 20;
        RSDK.SetSpriteAnimation(BallHog->aniFrames, 0, &self->animator, true, 0);
        self->state = BallHog_State_Setup;
    }
}

void BallHog_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        BallHog->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/BallHog.bin", SCOPE_STAGE);
    BallHog->hitboxBadnik.left   = -12;
    BallHog->hitboxBadnik.top    = -18;
    BallHog->hitboxBadnik.right  = 12;
    BallHog->hitboxBadnik.bottom = 18;
    BallHog->hitboxBomb.left     = -6;
    BallHog->hitboxBomb.top      = -6;
    BallHog->hitboxBomb.right    = 6;
    BallHog->hitboxBomb.bottom   = 6;

    BallHog->sfxExplosion = RSDK.GetSFX("Stage/Explosion.wav");
    BallHog->sfxArrowHit  = RSDK.GetSFX("PSZ/ArrowHit.wav");
    BallHog->sfxDrop      = RSDK.GetSFX("TMZ1/HogDrop.wav");
    BallHog->sfxJump      = RSDK.GetSFX("TMZ1/HogJump.wav");

    DEBUGMODE_ADD_OBJ(BallHog);
}

void BallHog_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(BallHog, NULL, self->position.x, self->position.y);
}

void BallHog_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(BallHog->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void BallHog_CheckPlayerCollisions(void)
{
    RSDK_THIS(BallHog);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &BallHog->hitboxBadnik))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void BallHog_CheckOnScreen(void)
{
    RSDK_THIS(BallHog);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        BallHog_Create(NULL);
    }
}

void BallHog_State_Setup(void)
{
    RSDK_THIS(BallHog);
    self->active = ACTIVE_NORMAL;
    self->state  = BallHog_State_Idle;
    BallHog_State_Idle();
}

void BallHog_State_Idle(void)
{
    RSDK_THIS(BallHog);
    if (!--self->timer) {
        self->timer = 20;
        RSDK.SetSpriteAnimation(BallHog->aniFrames, 1, &self->animator, true, 0);
        self->state = BallHog_State_Jump;
    }
    RSDK.ProcessAnimation(&self->animator);
    BallHog_CheckPlayerCollisions();
    BallHog_CheckOnScreen();
}

void BallHog_State_Jump(void)
{
    RSDK_THIS(BallHog);

    if (self->timer) {
        self->timer--;
        if (!self->timer) {
            RSDK.PlaySfx(BallHog->sfxJump, false, 255);
            self->velocity.y = -0x203D9;
            self->position.y -= 0x29999;
        }
    }
    else {
        self->position.y += self->velocity.y;
        self->velocity.y += 0x95C0;

        if (self->velocity.y > 0 && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, 2)) {
            self->velocity.y = 0;
            self->timer      = 10;
            RSDK.SetSpriteAnimation(BallHog->aniFrames, 2, &self->animator, true, 0);
            self->state = BallHog_State_Land;
        }
    }
    RSDK.ProcessAnimation(&self->animator);
    BallHog_CheckPlayerCollisions();
    BallHog_CheckOnScreen();
}

void BallHog_State_Land(void)
{
    RSDK_THIS(BallHog);

    if (!--self->timer) {
        RSDK.SetSpriteAnimation(BallHog->aniFrames, 0, &self->animator, true, 0);

        if (!--self->jumpCount) {
            self->timer     = 28;
            self->jumpCount = self->numJumps + 1;
            self->state     = BallHog_State_DropBomb;
        }
        else {
            self->timer = 20;
            self->state = BallHog_State_Idle;
        }
    }
    RSDK.ProcessAnimation(&self->animator);
    BallHog_CheckPlayerCollisions();
    BallHog_CheckOnScreen();
}

void BallHog_State_DropBomb(void)
{
    RSDK_THIS(BallHog);

    if (--self->timer) {
        if (self->timer == 8)
            RSDK.SetSpriteAnimation(BallHog->aniFrames, 3, &self->animator, true, 0);
    }
    else {
        self->timer = 18;
        RSDK.PlaySfx(BallHog->sfxDrop, false, 255);

        EntityBallHog *bomb = CREATE_ENTITY(BallHog, intToVoid(true), self->position.x, self->position.y);
        if (self->direction) {
            bomb->position.x += 0x40000;
            bomb->velocity.x = 0x10000;
        }
        else {
            bomb->position.x -= 0x40000;
            bomb->velocity.x = -0x10000;
        }
        bomb->position.y += 0xC0000;
        bomb->timer   = 60 * self->bombTime;
        self->state = BallHog_State_Land;
    }
    RSDK.ProcessAnimation(&self->animator);
    BallHog_CheckPlayerCollisions();
    BallHog_CheckOnScreen();
}

void BallHog_State_Bomb(void)
{
    RSDK_THIS(BallHog);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        if (self->velocity.y > 0 && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 4)) {
            RSDK.PlaySfx(BallHog->sfxArrowHit, false, 255);
            int32 storeY = self->position.y;

            self->velocity.y = -0x30000;
            RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x20000, 0x80000, 8);
            int32 y1 = self->position.y;

            self->position.y = storeY;
            RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x20000, 0x80000, 8);
            int32 y2 = self->position.y;

            self->position.y = storeY;
            if (self->velocity.x <= 0 ? y1 < y2 : y2 < y1)
                self->velocity.x = -self->velocity.x;
        }
        RSDK.ProcessAnimation(&self->animator);

        bool32 flag = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &BallHog->hitboxBomb)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x100, player, 2, &player->uncurlTimer)) {
#endif
                    Player_CheckHit(player, self);
                    flag = true;
                    foreach_break;
#if RETRO_USE_PLUS
                }
#endif
            }
        }

        if (flag || !--self->timer) {
            RSDK.PlaySfx(BallHog->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(1), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
            destroyEntity(self);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void BallHog_EditorDraw(void) { BallHog_Draw(); }

void BallHog_EditorLoad(void) { BallHog->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/BallHog.bin", SCOPE_STAGE); }
#endif

void BallHog_Serialize(void)
{
    RSDK_EDITABLE_VAR(BallHog, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(BallHog, VAR_UINT8, numJumps);
    RSDK_EDITABLE_VAR(BallHog, VAR_UINT8, bombTime);
}
