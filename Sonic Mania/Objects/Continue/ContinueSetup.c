#include "SonicMania.h"

ObjectContinueSetup *ContinueSetup;

void ContinueSetup_Update(void)
{
    RSDK_THIS(ContinueSetup);
    StateMachine_Run(self->state);
    self->angle = (self->angle - 2) & 0x3FF;
}

void ContinueSetup_LateUpdate(void) {}

void ContinueSetup_StaticUpdate(void) {}

void ContinueSetup_Draw(void)
{
    RSDK_THIS(ContinueSetup);
    RSDK.Prepare3DScene(ContinueSetup->sceneIndex);
    RSDK.MatrixTranslateXYZ(&self->matrix2, 0, -0xF0000, 0x500000, true);
    RSDK.MatrixRotateX(&self->matrix3, self->rotationX);
    RSDK.MatrixRotateZ(&self->matrix4, self->angle);
    RSDK.MatrixTranslateXYZ(&self->matrix1, -0x120000, 0, 0, true);
    RSDK.MatrixMultiply(&self->matrix5, &self->matrix4, &self->matrix3);
    RSDK.MatrixMultiply(&self->matrix5, &self->matrix1, &self->matrix5);
    RSDK.MatrixMultiply(&self->matrix5, &self->matrix5, &self->matrix2);
    RSDK.AddModelTo3DScene(ContinueSetup->countIndex[self->countTimer / 10 % 10], ContinueSetup->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                           &self->matrix5, &self->matrix5, self->colour);
    RSDK.MatrixTranslateXYZ(&self->matrix1, 0x120000, 0, 0, true);
    RSDK.MatrixMultiply(&self->matrix5, &self->matrix4, &self->matrix3);
    RSDK.MatrixMultiply(&self->matrix5, &self->matrix1, &self->matrix5);
    RSDK.MatrixMultiply(&self->matrix5, &self->matrix5, &self->matrix2);
    RSDK.AddModelTo3DScene(ContinueSetup->countIndex[self->countTimer % 10], ContinueSetup->sceneIndex, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                           &self->matrix5, &self->matrix5, self->colour);
    RSDK.Draw3DScene(ContinueSetup->sceneIndex);

    Vector2 drawPos;
    drawPos.y = 0x600000;
    drawPos.x = ((ScreenInfo->centerX + 4) << 16) - (globals->continues << 19);
    if (self->dword74 == 1 && globals->continues > 0) {
        RSDK.DrawSprite(&ContinueSetup->animator, &drawPos, true);
    }

    drawPos.x += 0x140000;
    for (int32 i = 0; i < globals->continues; ++i) {
        RSDK.DrawSprite(&ContinueSetup->animator, &drawPos, true);
        drawPos.x += 0x140000;
    }
}

void ContinueSetup_Create(void *data)
{
    RSDK_THIS(ContinueSetup);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->visible       = true;
        self->drawOrder     = 1;
        self->rotationX     = 240;
        self->angle         = 256;
        self->countTimer    = 10;
        self->colour        = 0xFF00FF;
        self->dword74       = 1;
        self->state         = ContinueSetup_Unknown1;
        self->updateRange.x = 0x4000000;
        self->updateRange.y = 0x4000000;
        switch (globals->playerID & 0xFF) {
            case ID_TAILS: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 1); break;
            case ID_KNUCKLES: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 2); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 3); break;
            case ID_RAY: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 4); break;
#endif
            default: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_ICON, &ContinueSetup->animator, true, 0); break;
        }
        RSDK.SetActivePalette(1, 0, ScreenInfo->height);
    }
}

void ContinueSetup_StageLoad(void)
{
    const char *paths[10] = {
        "Continue/Count0.bin", "Continue/Count1.bin", "Continue/Count2.bin", "Continue/Count3.bin", "Continue/Count4.bin",
        "Continue/Count5.bin", "Continue/Count6.bin", "Continue/Count7.bin", "Continue/Count8.bin", "Continue/Count9.bin",
    };
    for (int32 i = 0; i < 10; ++i) {
        ContinueSetup->countIndex[i] = RSDK.LoadMesh(paths[i], SCOPE_STAGE);
    }
    ContinueSetup->sceneIndex = RSDK.Create3DScene("View:Continue", 4096, SCOPE_STAGE);
    RSDK.SetDiffuseColour(ContinueSetup->sceneIndex, 160, 160, 160);
    RSDK.SetDiffuseIntensity(ContinueSetup->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(ContinueSetup->sceneIndex, 15, 15, 15);
    ContinueSetup->sfxAccept = RSDK.GetSFX("Global/MenuAccept.wav");
}

void ContinueSetup_Unknown1(void)
{
    RSDK_THIS(ContinueSetup);
    if (++self->timer >= 8) {
        if (!RSDK.GetEntityCount(FXFade->objectID, true)) {
            self->timer = 0;
            self->state = ContinueSetup_Unknown2;
        }
    }
}

void ContinueSetup_Unknown2(void)
{
    RSDK_THIS(ContinueSetup);
    if (++self->timer2 == 60) {
        self->timer2 = 0;
        if (self->countTimer > 0) {
            self->countTimer--;
            if (self->alpha < 255)
                self->alpha += 24;
            self->colour = RSDK.GetPaletteEntry(2, self->alpha);
        }
    }
    if (ControllerInfo->keyA.press || ControllerInfo->keyStart.press || TouchInfo->count) {
        foreach_active(ContinuePlayer, player)
        {
            if (!player->isPlayer2)
                RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, (player->animator.animationID + 1), &player->animator, true, 0);
            player->state = ContinuePlayer_Unknown2;
        }
        self->state = ContinueSetup_Unknown3;
        RSDK.PlaySfx(ContinueSetup->sfxAccept, 0, 255);
    }
    if (!self->countTimer && ++self->timer == 60) {
        self->timer      = 0;
        self->state      = ContinueSetup_Unknown4;
        EntityFXFade *fade = (EntityFXFade *)RSDK.CreateEntity(FXFade->objectID, NULL, self->position.x, self->position.y);
        fade->speedIn      = 12;
        fade->wait         = 240;
    }
}

void ContinueSetup_Unknown3(void)
{
    RSDK_THIS(ContinueSetup);
    if (++self->timer == 90) {
        Music_FadeOut(0.0125);
    }
    if (self->timer == 180) {
        EntityFXFade *fade = (EntityFXFade *)RSDK.CreateEntity(FXFade->objectID, NULL, self->position.x, self->position.y);
        fade->speedIn      = 12;
        fade->wait         = 240;
    }
    if (self->timer == 260) {
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetScene("Encore Mode", "");
        else
#endif
            RSDK.SetScene("Mania Mode", "");
        SaveGame->saveRAM->continues   = globals->continues;
        SceneInfo->listPos = SaveGame->saveRAM->storedStageID;
        RSDK.LoadScene();
    }

    if (self->timer < 58) {
        self->dword74 = ((self->timer >> 1) & 1);
    }
    if (self->timer == 60) {
        self->dword74 = 1;
        if (globals->continues > 0)
            globals->continues--;
    }
}

void ContinueSetup_Unknown4(void)
{
    RSDK_THIS(ContinueSetup);
    if (++self->timer == 80) {
        RSDK.SetScene("Presentation", "Menu");
        RSDK.LoadScene();
    }
}

#if RETRO_INCLUDE_EDITOR
void ContinueSetup_EditorDraw(void) {}

void ContinueSetup_EditorLoad(void) {}
#endif

void ContinueSetup_Serialize(void) {}
