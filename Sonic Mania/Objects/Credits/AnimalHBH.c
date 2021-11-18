#include "SonicMania.h"

ObjectAnimalHBH *AnimalHBH;

void AnimalHBH_Update(void)
{
    RSDK_THIS(AnimalHBH);
    self->position.x -= 0x10000;
    int32 pos = self->position.x - (ScreenInfo->position.x << 16) - (ScreenInfo->centerX << 16);
    if (pos < 0)
        pos = ((ScreenInfo->position.x + ScreenInfo->centerX) << 16) - self->position.x;
    if (pos < 0x800000)
        AnimalHBH->palCnt = 0x20 * self->type;
    if (!self->type) {
        ++self->timer;
        if ((ControllerInfo->keyA.press || ControllerInfo->keyStart.press))
            self->timer = 1400;
        if (self->timer == 1400) {
            EntityFXFade *fxFade = CREATE_ENTITY(FXFade, NULL, self->position.x, self->position.y);
            fxFade->speedIn      = 12;
            fxFade->wait         = 240;
        }
        if (self->timer == 1536) {
            RSDK.SetScene("Presentation", "Menu");
            RSDK.LoadScene();
        }

        if (AnimalHBH->palID < AnimalHBH->palCnt) {
            AnimalHBH->palID++;
        }

        RSDK.SetPaletteEntry(1, 1, RSDK.GetPaletteEntry(5, AnimalHBH->palID));
    }
}

void AnimalHBH_LateUpdate(void) {}

void AnimalHBH_StaticUpdate(void) {}

void AnimalHBH_Draw(void)
{
    RSDK_THIS(AnimalHBH);
    Vector2 drawPos;

    drawPos.x = 2 * (self->position.x - ((ScreenInfo->position.x + ScreenInfo->centerX) << 15));
    drawPos.y = self->position.y;

    RSDK.SetActivePalette(1, 0, ScreenInfo->height);
    RSDK.DrawSprite(&self->silhouetteData, &drawPos, 0);

    RSDK.SetActivePalette(4, 0, ScreenInfo->height);
    RSDK.DrawSprite(&self->animalHBHData, NULL, 0);

    RSDK.SetActivePalette(1, 0, ScreenInfo->height);
}

void AnimalHBH_Create(void *data)
{
    RSDK_THIS(AnimalHBH);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = 2;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(AnimalHBH->animalHBHSprite, self->type, &self->animalHBHData, true, 0);
        RSDK.SetSpriteAnimation(AnimalHBH->silhouetteSprite, self->type, &self->silhouetteData, true, 0);
    }
}

void AnimalHBH_StageLoad(void)
{
    AnimalHBH->animalHBHSprite  = RSDK.LoadSpriteAnimation("Credits/AnimalHBH.bin", SCOPE_STAGE);
    AnimalHBH->silhouetteSprite = RSDK.LoadSpriteAnimation("Credits/Silhouettes.bin", SCOPE_STAGE);
    AnimalHBH->palID            = 0;
    AnimalHBH->palCnt           = 0;
}

#if RETRO_INCLUDE_EDITOR
void AnimalHBH_EditorDraw(void) {}

void AnimalHBH_EditorLoad(void) {}
#endif

void AnimalHBH_Serialize(void) { RSDK_EDITABLE_VAR(AnimalHBH, VAR_UINT8, type); }
