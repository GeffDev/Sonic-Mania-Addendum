#include "SonicMania.h"

ObjectCheckerBall *CheckerBall;

void CheckerBall_Update(void)
{
    RSDK_THIS(CheckerBall);
    self->active = ACTIVE_NORMAL;
    CheckerBall_Unknown2();
    self->field_60 += self->field_5C;
    CheckerBall_Unknown1();
    RSDK.ProcessTileCollisions(self, &CheckerBall->outerBox, &CheckerBall->innerBox);
    CheckerBall_Unknown5();
    CheckerBall_Unknown3();
    self->roundedPos.x = self->position.x & 0xFFFF0000;
    self->roundedPos.y = self->position.y & 0xFFFF0000;
}

void CheckerBall_LateUpdate(void) {}

void CheckerBall_StaticUpdate(void) {}

void CheckerBall_Draw(void)
{
    RSDK_THIS(CheckerBall);
    self->animator.frameID = (((self->field_60 >> 14) + 8) >> 4) & 7;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void CheckerBall_Create(void *data)
{
    RSDK_THIS(CheckerBall);
    self->visible         = true;
    self->drawOrder       = Zone->drawOrderLow;
    self->active          = ACTIVE_BOUNDS;
    self->updateRange.x   = 0x400000;
    self->updateRange.y   = 0x400000;
    self->onGround        = false;
    self->tileCollisions  = true;
    self->collisionLayers = Zone->fgLayers;
    self->roundedPos.x    = self->position.x;
    self->roundedPos.y    = self->position.y;
    self->collisionPlane  = 0;
    RSDK.SetSpriteAnimation(CheckerBall->aniFrames, 0, &self->animator, true, 0);
}

void CheckerBall_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ")) {
        CheckerBall->aniFrames = RSDK.LoadSpriteAnimation("GHZ/CheckerBall.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Blueprint")) {
        CheckerBall->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CheckerBall.bin", SCOPE_STAGE);
    }

    CheckerBall->outerBox.left   = -9;
    CheckerBall->outerBox.top    = -22;
    CheckerBall->outerBox.right  = 9;
    CheckerBall->outerBox.bottom = 22;
    CheckerBall->innerBox.left   = -8;
    CheckerBall->innerBox.top    = -22;
    CheckerBall->innerBox.right  = 8;
    CheckerBall->innerBox.bottom = 22;
    CheckerBall->hitbox.left     = -22;
    CheckerBall->hitbox.top      = -22;
    CheckerBall->hitbox.right    = 22;
    CheckerBall->hitbox.bottom   = 22;

    DEBUGMODE_ADD_OBJ(CheckerBall);

    CheckerBall->array1[0]  = 0;
    CheckerBall->array1[1]  = 0;
    CheckerBall->array1[2]  = 0;
    CheckerBall->array1[3]  = 0;
    CheckerBall->array1[4]  = 1;
    CheckerBall->array1[5]  = 1;
    CheckerBall->array1[6]  = 1;
    CheckerBall->array1[7]  = 1;
    CheckerBall->array1[8]  = 2;
    CheckerBall->array1[9]  = 2;
    CheckerBall->array1[10] = 3;
    CheckerBall->array1[11] = 3;
    CheckerBall->array1[12] = 4;
    CheckerBall->array1[13] = 4;
    CheckerBall->array1[14] = 5;
    CheckerBall->array1[15] = 6;
    CheckerBall->array1[16] = 7;
    CheckerBall->array1[17] = 8;
    CheckerBall->array1[18] = 9;
    CheckerBall->array1[19] = 10;
    CheckerBall->array1[20] = 12;
    CheckerBall->array1[21] = 14;
    CheckerBall->array1[22] = 16;
    CheckerBall->array1[23] = 20;

    CheckerBall->array2[0]  = -3;
    CheckerBall->array2[1]  = -5;
    CheckerBall->array2[2]  = -8;
    CheckerBall->array2[3]  = -11;
    CheckerBall->array2[4]  = -13;
    CheckerBall->array2[5]  = -16;
    CheckerBall->array2[6]  = -19;
    CheckerBall->array2[7]  = -21;
    CheckerBall->array2[8]  = -24;
    CheckerBall->array2[9]  = -27;
    CheckerBall->array2[10] = -30;
    CheckerBall->array2[11] = -32;
    CheckerBall->array2[12] = -35;
    CheckerBall->array2[13] = -37;
    CheckerBall->array2[14] = -40;
    CheckerBall->array2[15] = -43;
    CheckerBall->array2[16] = -46;
    CheckerBall->array2[17] = -48;
    CheckerBall->array2[18] = -51;
    CheckerBall->array2[19] = -53;
    CheckerBall->array2[20] = -56;
    CheckerBall->array2[21] = -59;
    CheckerBall->array2[22] = -61;
    CheckerBall->array2[23] = -64;
}

void CheckerBall_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(CheckerBall, NULL, self->position.x, self->position.y);
}

void CheckerBall_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(CheckerBall->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void CheckerBall_Unknown1(void)
{
    RSDK_THIS(CheckerBall);
    if (self->onGround) {
        int32 spd = RSDK.Sin256(self->angle) << 13 >> 8;
        self->groundVel += spd;
        self->field_5C = self->groundVel;
        if (!spd && !self->flag) {
            if (self->groundVel <= 0) {
                self->groundVel += 0x400;
                if (self->groundVel > 0)
                    self->groundVel = 0;
            }
            else {
                self->groundVel -= 0x400;
                if (self->groundVel < 0)
                    self->groundVel = 0;
            }
        }
        self->velocity.x = self->groundVel * RSDK.Cos256(self->angle) >> 8;
        self->velocity.y = self->groundVel * RSDK.Sin256(self->angle) >> 8;
        self->flag       = false;
    }
    else {
        self->velocity.y += 0x3800;
        self->groundVel = self->velocity.x;
        self->flag      = false;
    }
}

void CheckerBall_Unknown2(void)
{
    RSDK_THIS(CheckerBall);

    Hitbox hitbox;
    hitbox.top    = -22;
    hitbox.bottom = 22;

    int32 playerID          = 0;
    self->activePlayers = 0;
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 acc              = player->acceleration >> 1;
        if (player->onGround) {
            if (player->position.x <= self->position.x) {
                hitbox.left  = -23;
                hitbox.right = 0;
                if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                    if (!player->right) {
                        if (self->velocity.x > 0 && player->groundVel > self->groundVel) {
                            player->velocity.x -= 0x10000;
                            player->groundVel -= 0x10000;
                        }
                    }
                    else {
                        self->activePlayers |= 1 << playerID;
                        if (self->groundVel < player->topSpeed)
                            self->groundVel += acc;
                        self->flag       = true;
                        player->position.x = self->roundedPos.x + ((playerHitbox->left - 22) << 16);
                        player->groundVel  = 0;
                    }
                }
            }
            else {
                hitbox.left  = 0;
                hitbox.right = 23;
                if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                    if (!player->left) {
                        if (self->velocity.x < 0 && player->groundVel < self->groundVel) {
                            player->velocity.x += 0x10000;
                            player->groundVel += 0x10000;
                        }
                    }
                    else {
                        self->activePlayers |= 1 << playerID;
                        if (self->groundVel > -player->topSpeed)
                            self->groundVel -= acc;
                        self->flag       = true;
                        player->position.x = self->roundedPos.x + ((playerHitbox->right + 22) << 16);
                        player->groundVel  = 0;
                    }
                }
            }
        }
        ++playerID;
    }
}

void CheckerBall_Unknown3(void)
{
    RSDK_THIS(CheckerBall);
    int32 x              = ((self->position.x - self->collisionOffset.x) & 0xFFFF0000) - self->roundedPos.x;
    int32 y              = ((self->position.y - self->collisionOffset.y) & 0xFFFF0000) - self->roundedPos.y;
    int32 startX         = self->position.x;
    int32 startY         = self->position.y;
    self->position.x = self->roundedPos.x;
    self->position.y = self->roundedPos.y;

    int32 playerID = 0;
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        if ((1 << playerID) & self->activePlayers) {
            if (player->position.x >= self->position.x)
                player->position.x = (22 - playerHitbox->left) << 16;
            else
                player->position.x = (-22 - playerHitbox->right) << 16;
            player->position.x += startX;
        }
        else {
            int32 result = Player_CheckCollisionBox(player, self, &CheckerBall->hitbox);
            if (result == 4 && self->velocity.y >= 0 && player->onGround) {
                player->hurtFlag = true;
            }
            else if (result == 1) {
                player->position.x += x + (x >> 1) + self->collisionOffset.x;
                player->position.y += y + self->collisionOffset.y;

                if (player->velocity.x) {
                    if (player->velocity.x <= 0)
                        self->groundVel += player->acceleration >> 2;
                    else
                        self->groundVel -= player->acceleration >> 2;
                    self->flag = true;
                }
                self->position.x = startX;
                self->position.y = startY;
                self->position.x = self->roundedPos.x;
                self->position.y = self->roundedPos.y;
                if (!Player_CheckCollisionBox(player, self, &CheckerBall->hitbox)) {
                    player->groundVel += x + (x >> 1);
                    player->velocity.x  = player->groundVel;
                    player->onGround    = false;
                    player->controlLock = 15;
                }
            }
        }
        ++playerID;
    }
    self->position.x        = startX;
    self->position.y        = startY;
    self->collisionOffset.x = 0;
    self->collisionOffset.y = 0;
}

void CheckerBall_BadnikBreak(void *b, Hitbox *hitbox)
{
    RSDK_THIS(CheckerBall);
    Entity *badnik = (Entity *)b;
    if (RSDK.CheckObjectCollisionTouchBox(badnik, hitbox, self, &CheckerBall->hitbox)) {
        int32 id = RSDK.Rand(0, 32);
        CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[id >> 4] + 1), badnik->position.x, badnik->position.y);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), badnik->position.x, badnik->position.y - 0x100000)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        EntityPlayer *player1       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityScoreBonus *bonus     = CREATE_ENTITY(ScoreBonus, NULL, badnik->position.x, badnik->position.y);
        bonus->drawOrder            = Zone->drawOrderHigh;
        bonus->animator.frameID     = player1->scoreBonus;
        switch (player1->scoreBonus) {
            case 0: Player_GiveScore(player1, 100); break;
            case 1: Player_GiveScore(player1, 200); break;
            case 2: Player_GiveScore(player1, 500); break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14: Player_GiveScore(player1, 1000); break;
            case 15: Player_GiveScore(player1, 10000); break;
            default: break;
        }

        if (player1->scoreBonus < 15)
            player1->scoreBonus++;
        destroyEntity(badnik);
    }
}

void CheckerBall_Unknown5(void)
{
    RSDK_THIS(CheckerBall);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    foreach_active(PlaneSwitch, planeSwitch)
    {
        int32 x     = (self->position.x - planeSwitch->position.x) >> 8;
        int32 y     = (self->position.y - planeSwitch->position.y) >> 8;
        int32 scanX = (y * RSDK.Sin256(planeSwitch->negAngle)) + (x * RSDK.Cos256(planeSwitch->negAngle)) + planeSwitch->position.x;
        int32 scanY = (y * RSDK.Cos256(planeSwitch->negAngle)) - (x * RSDK.Sin256(planeSwitch->negAngle)) + planeSwitch->position.y;
        int32 pos   = ((self->velocity.y >> 8) * RSDK.Sin256(planeSwitch->negAngle)) + (self->velocity.x >> 8) * RSDK.Cos256(planeSwitch->negAngle);
        RSDK.Cos256(planeSwitch->negAngle);
        RSDK.Sin256(planeSwitch->negAngle);
        if (!(planeSwitch->onPath && !self->onGround)) {
            int32 xDif = abs(scanX - planeSwitch->position.x);
            int32 yDif = abs(scanY - planeSwitch->position.y);

            if (xDif < 0x180000 && yDif < planeSwitch->size << 19) {
                if (scanX + pos >= planeSwitch->position.x) {
                    self->collisionPlane = (planeSwitch->flags >> 3) & 1;
                    if (!(planeSwitch->flags & 4)) {
                        self->drawOrder = Zone->playerDrawLow;
                    }
                    else {
                        self->drawOrder = Zone->playerDrawHigh;
                    }
                }
                else {
                    self->collisionPlane = (planeSwitch->flags >> 1) & 1;
                    if (!(planeSwitch->flags & 1)) {
                        self->drawOrder = Zone->playerDrawLow;
                    }
                    else {
                        self->drawOrder = Zone->playerDrawHigh;
                    }
                }
            }
        }
    }

    foreach_active(CheckerBall, checkerBall)
    {
        if (checkerBall != self && self->collisionPlane == checkerBall->collisionPlane) {
            int32 pos = abs(self->position.x - checkerBall->position.x) >> 17;
            if (pos >= 24)
                pos = 23;
            Hitbox hitbox;
            hitbox.top       = CheckerBall->array1[pos] - 22 + (CheckerBall->array1[pos] >> 1);
            hitbox.bottom    = -hitbox.top;
            hitbox.left      = -22;
            hitbox.right     = 22;
            switch (RSDK.CheckObjectCollisionBox(checkerBall, &hitbox, self, &hitbox, true)) {
                case 1:
                    if (self->position.x >= checkerBall->position.x) {
                        self->groundVel += 0xC00;
                        self->velocity.x += 0xC00;
                        checkerBall->groundVel -= 0xC00;
                        checkerBall->velocity.x -= 0xC00;
                        checkerBall->field_5C -= 0xC00;
                    }
                    else {
                        self->groundVel -= 0xC00;
                        self->velocity.x -= 0xC00;
                        checkerBall->groundVel += 0xC00;
                        checkerBall->velocity.x += 0xC00;
                        checkerBall->field_5C += 0xC00;
                    }
                    break;
                case 2:
                case 3:
                    if (self->groundVel <= 0) {
                        if (self->groundVel < 0 && self->position.x > checkerBall->position.x) {
                            self->velocity.x = checkerBall->velocity.x;
                            self->groundVel  = checkerBall->groundVel;
                        }
                    }
                    else if (self->position.x < checkerBall->position.x) {
                        self->velocity.x      = checkerBall->velocity.x;
                        self->groundVel       = checkerBall->groundVel;
                        checkerBall->groundVel  = self->groundVel;
                        checkerBall->velocity.x = self->velocity.x;
                    }
                    else {
                        if (self->groundVel < 0 && self->position.x > checkerBall->position.x) {
                            self->velocity.x = checkerBall->velocity.x;
                            self->groundVel  = checkerBall->groundVel;
                        }
                    }
                    break;
                case 4:
                    if (self->velocity.y <= 0) {
                        if (self->velocity.y < 0 && self->position.y > checkerBall->position.y) {
                            self->velocity.y      = checkerBall->velocity.y;
                            checkerBall->velocity.y = self->velocity.y;
                            if (self->onGround) {
                                self->velocity.x = checkerBall->velocity.x;
                                self->groundVel  = checkerBall->groundVel;
                            }
                            if (checkerBall->onGround) {
                                checkerBall->groundVel  = self->groundVel;
                                checkerBall->velocity.x = self->velocity.x;
                            }
                        }
                    }
                    if (self->position.y < checkerBall->position.y) {
                        self->velocity.y      = checkerBall->velocity.y;
                        checkerBall->velocity.y = self->velocity.y;
                        if (self->onGround) {
                            self->velocity.x = checkerBall->velocity.x;
                            self->groundVel  = checkerBall->groundVel;
                        }
                        if (checkerBall->onGround) {
                            checkerBall->groundVel  = self->groundVel;
                            checkerBall->velocity.x = self->velocity.x;
                        }
                    }
                    else {
                        if (self->velocity.y < 0 && self->position.y > checkerBall->position.y) {
                            self->velocity.y      = checkerBall->velocity.y;
                            checkerBall->velocity.y = self->velocity.y;
                            if (self->onGround) {
                                self->velocity.x = checkerBall->velocity.x;
                                self->groundVel  = checkerBall->groundVel;
                            }
                            if (checkerBall->onGround) {
                                checkerBall->groundVel  = self->groundVel;
                                checkerBall->velocity.x = self->velocity.x;
                            }
                        }
                    }
                    break;
                default: break;
            }

            if (RSDK.CheckObjectCollisionTouchBox(checkerBall, &hitbox, self, &hitbox)) {
                if (self->position.x >= checkerBall->position.x)
                    self->position.x += 0x10000;
                else
                    self->position.x -= 0x10000;

                if (self->position.y >= checkerBall->position.y)
                    self->position.y += 0x10000;
                else
                    self->position.y -= 0x10000;
            }
        }
    }

    foreach_active(Platform, platform)
    {
        if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
            platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
            int32 result           = 0;
            if (platform->collision)
                result = RSDK.CheckObjectCollisionBox(platform, &platform->hitbox, self, &CheckerBall->hitbox, true);
            else
                result = RSDK.CheckObjectCollisionPlatform(platform, &platform->hitbox, self, &CheckerBall->hitbox, true);
            if (result == 1) {
                if (platform->state == Platform_State_Collapsing && !platform->collapseDelay)
                    platform->collapseDelay = 30;
                platform->stood = true;
                self->position.x += platform->collisionOffset.x;
                self->position.y += platform->collisionOffset.y;
                self->position.y &= 0xFFFF0000;
                self->onGround          = true;
                self->collisionOffset.x = platform->collisionOffset.x;
                self->collisionOffset.y = platform->collisionOffset.y;
            }
            platform->position.x = platform->centerPos.x;
            platform->position.y = platform->centerPos.y;
        }
    }

    foreach_active(Bridge, bridge)
    {
        if (self->position.x > bridge->startPos && self->position.x < bridge->endPos) {
            if (self == (EntityCheckerBall *)bridge->playerPtr) {
                int32 distance     = bridge->endPos - bridge->startPos;
                bridge->stoodPos = self->position.x - bridge->startPos;
                bridge->field_68 = RSDK.Sin512((bridge->stoodPos >> 8) / (distance >> 16)) * (distance >> 13);
                if (self->position.y > bridge->position.y - 0x300000) {
                    if (self->velocity.y < 0) {
                        bridge->playerPtr = (Entity *)-2;
                    }
                    else {
                        ++bridge->activePlayerCount;
                        self->position.y = bridge->position.y + bridge->field_6C - ((CheckerBall->hitbox.bottom + 8) << 16);
                        if (!self->onGround) {
                            self->onGround  = true;
                            self->groundVel = self->velocity.x;
                        }
                        self->velocity.y = 0;
                    }
                }
            }
            else {
                Hitbox bridgeHitbox;
                bridgeHitbox.left  = -0x400;
                bridgeHitbox.right = 0x400;

                if (!bridge->activePlayerCount)
                    bridge->stoodPos = self->position.x - bridge->startPos;

                if (self->velocity.y >= 0) {
                    int32 divisor = 0;
                    int32 ang     = 0;
                    if (self->position.x - bridge->startPos > bridge->stoodPos) {
                        divisor = bridge->endPos - bridge->startPos - bridge->stoodPos;
                        ang     = (bridge->endPos - self->position.x) << 7;
                    }
                    else {
                        divisor = bridge->stoodPos;
                        ang     = (self->position.x - bridge->startPos) << 7;
                    }

                    int32 hitY = (bridge->field_6C * RSDK.Sin512(ang / divisor) >> 9) - 0x80000;
                    if (self->velocity.y < 0x8000) {
                        bridgeHitbox.bottom = (hitY >> 16);
                        bridgeHitbox.top    = (hitY >> 16) - 8;
                    }
                    else {
                        bridgeHitbox.top    = (hitY >> 16);
                        bridgeHitbox.bottom = (hitY >> 16) + 8;
                    }

                    if (RSDK.CheckObjectCollisionTouchBox(bridge, &bridgeHitbox, self, &CheckerBall->hitbox)) {
                        ++bridge->activePlayerCount;

                        self->position.y = hitY + bridge->position.y - (CheckerBall->hitbox.bottom << 16);
                        if (!self->onGround) {
                            self->onGround  = true;
                            self->groundVel = self->velocity.x;
                        }

                        if (bridge->playerPtr == (Entity *)-1) {
                            bridge->playerPtr = (Entity *)self;
                            if (self->velocity.y < 256)
                                bridge->timer = 0x80;
                        }

                        if (bridge->playerPtr == (Entity *)-2)
                            bridge->playerPtr = (Entity *)self;
                        self->velocity.y = 0;
                    }
                }
            }
        }
        else if (self == (EntityCheckerBall *)bridge->playerPtr) {
            bridge->playerPtr = (Entity *)-2;
            bridge->timer     = 32;
        }
    }

    foreach_active(Batbrain, batBrain) { CheckerBall_BadnikBreak(batBrain, &Batbrain->hitbox); }

    foreach_active(BuzzBomber, buzzBomber)
    {
        if (buzzBomber->state == BuzzBomber_Unknown6 || buzzBomber->state == BuzzBomber_Unknown7) {
            if (RSDK.CheckObjectCollisionTouchBox(buzzBomber, &BuzzBomber->hitbox2, self, &CheckerBall->hitbox)) {
                int32 angle              = RSDK.ATan2(self->position.x - buzzBomber->position.x, self->position.y - buzzBomber->position.y);
                buzzBomber->velocity.x = -0x80 * RSDK.Cos256(angle);
                buzzBomber->velocity.y = -0x80 * RSDK.Sin256(angle);
            }
        }
        else {
            CheckerBall_BadnikBreak(buzzBomber, &BuzzBomber->hitbox1);
        }
    }

    foreach_active(Chopper, chopper) { CheckerBall_BadnikBreak(chopper, &Chopper->hitbox); }

    foreach_active(Crabmeat, crabmeat)
    {
        if (crabmeat->state == Crabmeat_State_Projectile) {
            if (RSDK.CheckObjectCollisionTouchBox(crabmeat, &Crabmeat->projectileHitbox, self, &CheckerBall->hitbox)) {
                int32 angle            = RSDK.ATan2(self->position.x - crabmeat->position.x, self->position.y - crabmeat->position.y);
                crabmeat->velocity.x = -0x80 * RSDK.Cos256(angle);
                crabmeat->velocity.y = -0x80 * RSDK.Sin256(angle);
            }
        }
        else {
            CheckerBall_BadnikBreak(crabmeat, &Crabmeat->hitbox);
        }
    }

    foreach_active(Motobug, motobug)
    {
        if (motobug->state != Motobug_State_Smoke)
            CheckerBall_BadnikBreak(motobug, &Motobug->hitbox);
    }

    foreach_active(Newtron, newtron)
    {
        if (newtron->state == Newtron_State_Projectile) {
            if (RSDK.CheckObjectCollisionTouchBox(newtron, &Newtron->hitbox3, self, &CheckerBall->hitbox)) {
                int32 angle           = RSDK.ATan2(self->position.x - newtron->position.x, self->position.y - newtron->position.y);
                newtron->velocity.x = -0x800 * RSDK.Cos256(angle);
                newtron->velocity.y = -0x800 * RSDK.Sin256(angle);
            }
        }
        else {
            if (newtron->type == 1)
                CheckerBall_BadnikBreak(newtron, &Newtron->hitbox2);
            else
                CheckerBall_BadnikBreak(newtron, &Newtron->hitbox1);
        }
    }

    foreach_active(Splats, splats) { CheckerBall_BadnikBreak(splats, &Splats->hitbox1); }

    foreach_active(ItemBox, itemBox)
    {
        if ((itemBox->state == ItemBox_State_Normal || itemBox->state == ItemBox_State_Falling)
            && RSDK.CheckObjectCollisionTouchBox(itemBox, &ItemBox->hitbox, self, &CheckerBall->hitbox)) {
            RSDK.CreateEntity(TYPE_BLANK, 0, itemBox->position.x, itemBox->position.y);
            itemBox->storedEntity  = (Entity *)player1;
            itemBox->alpha         = 0x100;
            itemBox->contentsSpeed = -0x38000;
            itemBox->active        = 1;
            itemBox->velocity.y    = -0x20000;
            itemBox->state         = ItemBox_State_Contents;
            RSDK.SetSpriteAnimation(ItemBox->aniFrames, 1, &itemBox->animatorBox, true, 0);
            itemBox->animatorBox.frameID = ItemBox->brokenFrame++;
            ItemBox->brokenFrame %= 3;
            RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->animatorOverlay, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->animatorDebris, true, 0);
            RSDK.CreateEntity(Explosion->objectID, 0, itemBox->position.x, itemBox->position.y - 0x100000);

            for (int32 i = 0; i < 6; ++i) {
                EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, 0, itemBox->position.x + RSDK.Rand(-0x80000, 0x80000),
                                                                         itemBox->position.y + RSDK.Rand(-0x80000, 0x80000));
                debris->state        = Debris_State_Fall;
                debris->gravity      = 0x4000;
                debris->velocity.x   = RSDK.Rand(0, 0x20000);
                if (debris->position.x < itemBox->position.x)
                    debris->velocity.x = -debris->velocity.x;
                debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                debris->drawFX     = FX_FLIP;
                debris->direction  = i & 3;
                debris->drawOrder  = Zone->drawOrderHigh;
                RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
            }

            RSDK.PlaySfx(ItemBox->sfxDestroy, 0, 255);
            itemBox->active = ACTIVE_NORMAL;
            if (itemBox->type == 13) {
                if (itemBox->type == 7) {
                    switch (player1->characterID) {
                        case ID_SONIC: itemBox->type = 7; break;
                        case ID_TAILS: itemBox->type = 8; break;
                        case ID_KNUCKLES: itemBox->type = 9; break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY: itemBox->type = 15; break;
                        case ID_RAY: itemBox->type = 16; break;
#endif
                        default: break;
                    }
                }
                itemBox->animatorContents.frameID = itemBox->type;
            }
        }
    }

    foreach_active(BreakableWall, breakableWall)
    {
        if (breakableWall->state == BreakableWall_State_BreakableSides
            && RSDK.CheckObjectCollisionTouchBox(breakableWall, &breakableWall->hitbox, self, &CheckerBall->hitbox)) {
            int32 *arrayPtr2 = BreakableWall->value3;

            int32 *arrayPtr = NULL;
            if (self->position.x >= breakableWall->position.x)
                arrayPtr = BreakableWall->value5;
            else
                arrayPtr = BreakableWall->value4;

            for (int32 y = 0; y < 4; ++y) {
                for (int32 x = 0; x < 2; ++x) {
                    EntityBreakableWall *block = (EntityBreakableWall *)RSDK.CreateEntity(
                        BreakableWall->objectID, intToVoid(1), breakableWall->position.x + arrayPtr2[0], arrayPtr2[1] + breakableWall->position.y);
                    block->tileInfo   = RSDK.GetTileInfo(Zone->fgHigh, (breakableWall->position.x + arrayPtr2[0]) >> 20,
                                                       (arrayPtr2[1] + breakableWall->position.y) >> 20);
                    block->velocity.x = arrayPtr[0];
                    block->velocity.y = arrayPtr[1];
                    RSDK.SetTileInfo(Zone->fgHigh, (breakableWall->position.x + arrayPtr2[0]) >> 20, (arrayPtr2[1] + breakableWall->position.y) >> 20,
                                     0xFFFF);
                    arrayPtr2 += 2;
                    arrayPtr += 2;
                }
            }
            RSDK.PlaySfx(BreakableWall->sfxBreak, 0, 255);
            RSDK.ResetEntityPtr(breakableWall, TYPE_BLANK, NULL);
        }
    }

    foreach_active(CollapsingPlatform, collapsingPlatform)
    {
        if (RSDK.CheckObjectCollisionTouchBox(collapsingPlatform, &collapsingPlatform->hitbox, self, &CheckerBall->hitbox) && self->onGround) {
            collapsingPlatform->playerPos.x = self->position.x;
        }
    }

    foreach_active(Spikes, spikes)
    {
        if (RSDK.CheckObjectCollisionBox(spikes, &spikes->hitbox, self, &CheckerBall->hitbox, true)) {
            self->onGround = true;
            self->position.x += spikes->offset.x;
            self->position.y += spikes->offset.y;
        }
    }

    foreach_active(Spring, spring)
    {
        int32 result = RSDK.CheckObjectCollisionBox(spring, &spring->hitbox, self, &CheckerBall->hitbox, true);
        if (result) {
            if (spring->state == Spring_State_Vertical) {
                if (spring->direction) {
                    if (result == 4) {
                        self->onGround   = false;
                        self->velocity.y = spring->velocity.y;
                        RSDK.SetSpriteAnimation(Spring->aniFrames, spring->type, &spring->animator, true, 0);
                        RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                    }
                }
                else if (result == 1) {
                    self->onGround = false;
                    self->velocity.y = spring->velocity.y;
                    RSDK.SetSpriteAnimation(Spring->aniFrames, spring->type, &spring->animator, true, 0);
                    RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                }
            }
            else {
                if (spring->state == Spring_State_Diagonal) {
                    if (!self->onGround && self->velocity.y < 0 && abs(self->velocity.x) <= -self->velocity.y) {
                        continue;
                    }
                    else {
                        self->onGround   = false;
                        self->velocity.x = spring->velocity.x;
                        self->velocity.y = spring->velocity.y;
                        RSDK.SetSpriteAnimation(Spring->aniFrames, spring->type, &spring->animator, true, 0);
                        RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                    }
                }
                else if (spring->state == Spring_State_Horizontal) {
                    if (!spring->direction) {
                        if (result == 3) {
                            if (!spring->onGround || self->onGround) {
                                self->velocity.x = spring->velocity.x;
                                self->groundVel  = spring->velocity.x;
                                RSDK.SetSpriteAnimation(Spring->aniFrames, spring->type, &spring->animator, true, 0);
                                RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                            }
                        }
                    }
                    else {
                        if (result == 2) {
                            if (!spring->onGround || self->onGround) {
                                self->velocity.x = spring->velocity.x;
                                self->groundVel  = spring->velocity.x;
                                RSDK.SetSpriteAnimation(Spring->aniFrames, spring->type, &spring->animator, true, 0);
                                RSDK.PlaySfx(Spring->sfxSpring, 0, 255);
                            }
                        }
                    }
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void CheckerBall_EditorDraw(void) {}

void CheckerBall_EditorLoad(void) {}
#endif

void CheckerBall_Serialize(void) {}
