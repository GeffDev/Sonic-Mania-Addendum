#include "SonicMania.h"

ObjectJuggleSaw *JuggleSaw;

// 🦀 crab in da code

void JuggleSaw_Update(void)
{
    RSDK_THIS(JuggleSaw);
    StateMachine_Run(entity->state);
}

void JuggleSaw_LateUpdate(void) {}

void JuggleSaw_StaticUpdate(void) {}

void JuggleSaw_Draw(void)
{
    RSDK_THIS(JuggleSaw);
    RSDK.DrawSprite(&entity->animator, 0, false);
}

void JuggleSaw_Create(void *data)
{
    RSDK_THIS(JuggleSaw);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0xC00000;
    entity->updateRange.y = 0xC00000;
    if (RSDK_sceneInfo->inEditor)
        return;
    if (data) {
        RSDK.SetSpriteAnimation(JuggleSaw->animID, 6, &entity->animator, true, 0);
        entity->state = JuggleSaw_Saw_Handle;
        return;
    }
    entity->spawnPos.x = entity->position.x;
    entity->spawnPos.y = entity->position.y;
    entity->spawnDir   = entity->direction;
    switch (entity->direction) {
        case FLIP_NONE: RSDK.SetSpriteAnimation(JuggleSaw->animID, entity->hasSaw == JSAW_HAS_SAW, &entity->animator, true, 0); break;
        case FLIP_X:
            entity->direction = FLIP_XY;
            RSDK.SetSpriteAnimation(JuggleSaw->animID, entity->hasSaw == JSAW_HAS_SAW, &entity->animator, true, 0);
            break;
        case FLIP_Y:
            entity->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(JuggleSaw->animID, 4 - (entity->hasSaw != JSAW_HAS_SAW), &entity->animator, true, 0);
            break;
        case FLIP_XY:
            entity->direction = FLIP_X;
            RSDK.SetSpriteAnimation(JuggleSaw->animID, 4 - (entity->hasSaw != JSAW_HAS_SAW), &entity->animator, true, 0);
            break;
        default: break;
    }
    if (!entity->sawSpeed)
        entity->sawSpeed = 8;
    entity->sawSpeed <<= 7;
    entity->state = JuggleSaw_Crab_Create;
}

void JuggleSaw_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        JuggleSaw->animID = RSDK.LoadSpriteAnimation("PSZ1/JuggleSaw.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        JuggleSaw->animID = RSDK.LoadSpriteAnimation("PSZ2/JuggleSaw.bin", SCOPE_STAGE);

    JuggleSaw->hitbox.left   = -12;
    JuggleSaw->hitbox.top    = -18;
    JuggleSaw->hitbox.right  = 12;
    JuggleSaw->hitbox.bottom = 18;

    JuggleSaw->friendbox.left   = -0x200;
    JuggleSaw->friendbox.top    = -0x200;
    JuggleSaw->friendbox.right  = 0x200;
    JuggleSaw->friendbox.bottom = 0x200;

    JuggleSaw->sawHitbox.left   = -12;
    JuggleSaw->sawHitbox.top    = -12;
    JuggleSaw->sawHitbox.right  = 12;
    JuggleSaw->sawHitbox.bottom = 12;

    JuggleSaw->grabboxR.left   = -24;
    JuggleSaw->grabboxR.top    = -38;
    JuggleSaw->grabboxR.right  = -16;
    JuggleSaw->grabboxR.bottom = -30;

    // yes im SURE this is accurate
    JuggleSaw->grabboxL.left   = 38;
    JuggleSaw->grabboxL.top    = -24;
    JuggleSaw->grabboxL.right  = 30;
    JuggleSaw->grabboxL.bottom = -16;

    DEBUGMODE_ADD_OBJ(JuggleSaw);
    JuggleSaw->explodeSFX = RSDK.GetSFX("Stage/Explosion.wav");
    JuggleSaw->juggleSFX  = RSDK.GetSFX("PSZ/Juggle.wav");
    JuggleSaw->throwSFX   = RSDK.GetSFX("PSZ/JuggleThrow.wav");
}

void JuggleSaw_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(JuggleSaw->animID, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void JuggleSaw_DebugSpawn(void)
{
    CREATE_ENTITY(JuggleSaw, 0, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y)->sawDelay = 30;
}

void JuggleSaw_EditorDraw(void) { JuggleSaw_DebugDraw(); }

void JuggleSaw_EditorLoad(void) { JuggleSaw_StageLoad(); }

void JuggleSaw_Serialize(void) {
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_UINT16, sawDelay);
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_ENUM, sawSpeed);
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_BOOL, hasSaw); //liars.
    RSDK_EDITABLE_VAR(JuggleSaw, VAR_UINT16, setID);
}

void JuggleSaw_Crab_Collide(void)
{
    RSDK_THIS(JuggleSaw);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &JuggleSaw->hitbox) && Player_CheckBadnikBreak(entity, player, false)) {
            if (entity->hasSaw == JSAW_HAS_SAW) {
                int debrisX = entity->position.x;
                int debrisY = entity->position.y;
                if (entity->spawnDir >= FLIP_Y)
                    debrisX += 0x200000 * ((entity->direction & 1) ? -1 : 1);
                else
                    debrisY += 0x200000 * ((entity->direction & 2) ? 1 : -1);
                EntityDebris *debris = CREATE_ENTITY(Debris, (void *)Debris_State_FallAndFlicker, debrisX, debrisY);
                RSDK.SetSpriteAnimation(JuggleSaw->animID, 6, &debris->animator, true, 0);
                int vx1 = -4, vx2 = 5, vy1 = -4, vy2 = 5;
                if (entity->spawnDir >= 2u) {
                    if (entity->direction & 1)
                        vx2 = -1;
                    else
                        vx1 = -4;
                }
                else {
                    if (entity->direction & 2)
                        vy1 = -4;
                    else
                        vy2 = -1;
                }
                debris->velocity.x    = RSDK.Rand(vx1, vx2) << 16;
                debris->velocity.y    = RSDK.Rand(vy1, vy2) << 16;
                debris->gravity       = 0x3800;
                debris->drawOrder     = entity->drawOrder;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }
            destroyEntity(entity);
        }
    }
}

void JuggleSaw_Crab_CheckOnScreen(void)
{
    RSDK_THIS(JuggleSaw);
    if (entity->hasSaw != JSAW_AWAITING_SAW && !RSDK.CheckOnScreen(entity, 0) && !RSDK.CheckPosOnScreen(&entity->spawnPos, &entity->updateRange)) {
        entity->sawSpeed   = entity->sawSpeed >> 7;
        entity->position.x = entity->spawnPos.x;
        entity->position.y = entity->spawnPos.y;
        entity->direction  = entity->spawnDir;
        JuggleSaw_Create(NULL);
    }
}

void JuggleSaw_Crab_Handle(void)
{
    // this was the worst func to decompile so far. -rmg
    RSDK_THIS(JuggleSaw);
    RSDK.ProcessAnimation(&entity->animator);
    switch (entity->spawnDir) {
        case FLIP_NONE:
        case FLIP_X: entity->position.x = (RSDK.Cos256(entity->angle) << 10) + entity->spawnPos.x; break;
        case FLIP_Y:
        case FLIP_XY: entity->position.y = (RSDK.Cos256(entity->angle) << 10) + entity->spawnPos.y; break;
        default: break;
    }
    if (entity->hasSaw != JSAW_HAS_SAW) {
        entity->angle += 2;
        JuggleSaw_Crab_Collide();
        JuggleSaw_Crab_CheckOnScreen();
        return;
    }
    entity->angle += 4;
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.PlaySFX(JuggleSaw->juggleSFX, 0, 255);
        if (entity->spawnDir >= FLIP_Y)
            entity->direction ^= FLIP_Y;
        else
            entity->direction ^= FLIP_X;
    }
    else {
        if (entity->sawTimer == entity->sawDelay - 1)
            --entity->sawTimer;
    }
    if (++entity->sawTimer < entity->sawDelay) {
        JuggleSaw_Crab_Collide();
        JuggleSaw_Crab_CheckOnScreen();
        return;
    }
    if (!entity->friends[0] || entity->friends[0]->objectID != entity->objectID) {
        entity->friends[0]  = NULL;
        entity->friendCount = 0;
        foreach_active(JuggleSaw, newFriend)
        {
            if (newFriend != entity && newFriend->hasSaw == JSAW_NO_SAW && newFriend->setID == entity->setID
                && RSDK.CheckObjectCollisionTouchBox(newFriend, &JuggleSaw->hitbox, entity, &JuggleSaw->friendbox)) {
                entity->friends[entity->friendCount++] = (Entity*)newFriend;
                if (entity->friendCount == 8)
                    foreach_break;
            }
        }
        if (!entity->friendCount) {
            JuggleSaw_Crab_Collide();
            JuggleSaw_Crab_CheckOnScreen();
            return;
        }
        entity->friends[0] = entity->friends[RSDK.Rand(0, entity->friendCount)];
        bool32 throwCheck  = true;
        if (entity->spawnDir >= 2u) {
            int friendY = entity->friends[0]->position.y;
            if (friendY < entity->position.y) {
                if (!(entity->direction & 2))
                    --entity->sawTimer;
            }
            throwCheck &= (entity->direction & 2) == 0;
        }
        else {
            int friendX = entity->friends[0]->position.x;
            if (friendX < entity->position.x) {
                if (!(entity->direction & 1))
                    --entity->sawTimer;
            }
            throwCheck &= (entity->direction & 1) == 0;
        }
        if (!(throwCheck && entity->sawTimer >= entity->sawDelay)) {
            --entity->sawTimer;
            JuggleSaw_Crab_Collide();
            JuggleSaw_Crab_CheckOnScreen();
            return;
        }
    }

    RSDK.SetSpriteAnimation(JuggleSaw->animID, entity->spawnDir > 1u ? 5 : 2, &entity->animator, true, 0);
    if (entity->spawnDir >= FLIP_Y)
        entity->direction ^= FLIP_Y;
    else
        entity->direction ^= FLIP_X;
    entity->state = JuggleSaw_Crab_ThrowSaw;
    JuggleSaw_Crab_Collide();
    JuggleSaw_Crab_CheckOnScreen();
}

void JuggleSaw_Crab_Create(void)
{
    RSDK_THIS(JuggleSaw);
    entity->active     = ACTIVE_NORMAL;
    entity->friends[0] = NULL;
    entity->angle      = 0;
    entity->sawTimer   = 0;
    entity->state      = JuggleSaw_Crab_Handle;
    JuggleSaw_Crab_Handle();
}

void JuggleSaw_Crab_ThrowSaw(void)
{
    RSDK_THIS(JuggleSaw);
    Animator *animator = &entity->animator;
    RSDK.ProcessAnimation(animator);
    if (animator->frameID == 3) {
        EntityJuggleSaw *reciever = (EntityJuggleSaw *)entity->friends[0];
        if (reciever->objectID == JuggleSaw->objectID) {
            RSDK.PlaySFX(JuggleSaw->throwSFX, 0, 255);
            reciever->hasSaw    = JSAW_AWAITING_SAW;
            reciever->active     = ACTIVE_NORMAL;
            EntityJuggleSaw *saw = CREATE_ENTITY(JuggleSaw, intToVoid(1), entity->position.x, entity->position.y);
            int sx = 0x2C0000, sy = -0xE0000;
            if (entity->spawnDir >= 2u) {
                sx = 0xE0000;
                sy = 0x2C0000;
            }
            saw->position.x += sx * ((entity->direction & FLIP_X) ? -1 : 1);
            saw->position.y += sy * ((entity->direction & FLIP_Y) ? -1 : 1);

            /*if (entity->spawnDir >= 2u) {
                if ((entity->direction & FLIP_X) != 0)
                    saw->position.x -= 0xE0000;
                else
                    saw->position.x += 0xE0000;
                if ((entity->direction & FLIP_Y) != 0)
                    saw->position.y -= 0x2C0000;
                else
                    saw->position.y += 0x2C0000;
            }
            else {
                if ((entity->direction & FLIP_Y) != 0)
                    saw->position.y += 0xE0000;
                else
                    saw->position.y -= 0xE0000;
                if ((entity->direction & FLIP_X) != 0)
                    saw->position.x -= 0x2C0000;
                else
                    saw->position.x += 0x2C0000;
            }//*/
            int recieverX   = reciever->position.x, targetX;
            int recieverY   = reciever->position.y, targetY;
            int recieverDir = reciever->direction, sawDir = 0;
            if (reciever->spawnDir >= FLIP_Y) {
                targetX = recieverX + 0x220000 * ((recieverDir & FLIP_X) ? -1 : 1);
                targetY = recieverY + 0x140000 * (reciever->spawnPos.y >= entity->spawnPos.y ? 1 : -1);
                sawDir  = (recieverDir & FLIP_X) | (reciever->spawnPos.y >= entity->spawnPos.y ? FLIP_Y : 0);
            }
            else {
                targetY = recieverY + 0x220000 * ((recieverDir & FLIP_Y) ? 1 : -1);
                targetX = recieverX + 0x140000 * (reciever->spawnPos.x >= entity->spawnPos.x ? 1 : -1);
                sawDir  = (recieverDir & FLIP_Y) | (reciever->spawnPos.x >= entity->spawnPos.x ? FLIP_X : 0);
            }
            /*if (reciever->spawnDir >= 2u) {
                int v15 = recieverDir & FLIP_X;
                if ((recieverDir & FLIP_X) != 0)
                    targetX = recieverX - 0x220000;
                else
                    targetX = recieverX + 0x220000;
                sawDir = recieverDir & FLIP_X;
                if (reciever->spawnPos.y >= entity->spawnPos.y) {
                    targetY = recieverY + 0x140000;
                    sawDir  = v15 | 2;
                }
                else {
                    targetY = recieverY - 0x140000;
                }
            }
            else {
                int v14 = recieverDir & FLIP_Y;
                if ((recieverDir & FLIP_Y) != 0)
                    targetY = recieverY + 0x220000;
                else
                    targetY = recieverY - 0x220000;
                sawDir = recieverDir & FLIP_Y;
                if (reciever->spawnPos.x >= entity->spawnPos.x) {
                    targetX = recieverX + 0x140000;
                    sawDir  = v14 | 1;
                }
                else {
                    targetX = recieverX - 0x140000;
                }
            }//*/

            saw->direction   = sawDir;
            int targetAngle  = RSDK.ATan2(targetX - saw->position.x, targetY - saw->position.y);
            saw->velocity.x  = entity->sawSpeed * RSDK.Cos256(targetAngle);
            saw->velocity.y  = entity->sawSpeed * RSDK.Sin256(targetAngle);
            saw->friends[0]  = (Entity*)reciever;
            saw->hasSaw     = JSAW_HAS_SAW;
            saw->active      = ACTIVE_NORMAL;
            entity->sawTimer = 0;
            entity->hasSaw  = JSAW_NO_SAW;
        }
        else {
            RSDK.SetSpriteAnimation(JuggleSaw->animID, entity->spawnDir > 1u ? 4 : 1, animator, true, 0);
            entity->state = JuggleSaw_Crab_Handle;
        }
        entity->friends[0] = NULL;
    }
    if (animator->frameID == animator->frameCount - 1) {
        RSDK.SetSpriteAnimation(JuggleSaw->animID, entity->spawnDir > 1u ? 3 : 0, animator, true, 0);
        entity->state = JuggleSaw_Crab_Handle;
    }
    JuggleSaw_Crab_Collide();
    JuggleSaw_Crab_CheckOnScreen();
}

void JuggleSaw_Saw_Handle(void)
{
    RSDK_THIS(JuggleSaw);
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    entity->position.y += entity->velocity.y;
    EntityJuggleSaw *reciever = (EntityJuggleSaw *)entity->friends[0];
    int oldDir                = reciever->direction;
    reciever->direction       = entity->direction;
    Hitbox *grabbox           = &JuggleSaw->grabboxR;
    if (!(reciever->spawnDir < FLIP_Y))
        grabbox = &JuggleSaw->grabboxL;
    reciever->direction = oldDir;
    if (RSDK.CheckObjectCollisionTouchBox(reciever, grabbox, entity, &JuggleSaw->sawHitbox)) {
        int newDir = 0;
        if (reciever->spawnDir >= FLIP_Y) {
            int velY = entity->velocity.y;
            if (velY) {
                if (velY >= 0)
                    newDir = oldDir | FLIP_Y;
                else
                    newDir = oldDir & ~FLIP_Y;
            }
        }
        else {
            int velX = entity->velocity.x;
            if (velX) {
                if (velX >= 0)
                    newDir = oldDir | FLIP_X;
                else
                    newDir = oldDir & ~FLIP_X;
            }
        }
        reciever->direction = newDir;

        reciever->hasSaw = JSAW_HAS_SAW;
        RSDK.SetSpriteAnimation(JuggleSaw->animID, entity->spawnDir > 1u ? 4 : 1, &reciever->animator, true, 0);
        destroyEntity(entity);
        return;
    }
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        RSDK.ProcessAnimation(&entity->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &JuggleSaw->sawHitbox)) {
                int animID = player->playerAnimator.animationID;
                if (player->characterID == ID_MIGHTY
                    && (animID == ANI_CROUCH || animID == ANI_JUMP || animID == ANI_SPINDASH || animID == ANI_DROPDASH)) {
                    if (!player->uncurlTimer) {
                        RSDK.PlaySFX(Player->sfx_PimPom, 0, 0xFF);
                        player->uncurlTimer = 30;
                    }
                    int targetAngle     = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                    entity->velocity.x  = -0x400 * RSDK.Cos256(targetAngle);
                    entity->velocity.y  = -0x600 * RSDK.Sin256(targetAngle);
                    entity->interaction = false;
                    entity->state       = JuggleSaw_Saw_Knocked;
                }
                else
                    Player_CheckHit(player, entity);
            }
        }
    }
    else if (!entity->friends[0] || entity->friends[0]->objectID != JuggleSaw->objectID)
        destroyEntity(entity);
}

void JuggleSaw_Saw_Knocked(void)
{
    RSDK_THIS(JuggleSaw);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->visible ^= true;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}