#include "SonicMania.h"

ObjectBGSwitch *BGSwitch;

void BGSwitch_Update(void)
{
    RSDK_THIS(BGSwitch);

    for (BGSwitch->screenID = 0; BGSwitch->screenID < SCREEN_MAX; BGSwitch->screenID++) {
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1 + BGSwitch->screenID, Camera);
        if (!camera->objectID)
            break;

        if (abs(camera->position.x - self->position.x) < self->size.x && abs(camera->position.y - self->position.y) < self->size.y) {
            if (self->bgID < LAYER_COUNT)
                BGSwitch->layerIDs[BGSwitch->screenID] = self->bgID;
        }

        int32 id = BGSwitch->layerIDs[BGSwitch->screenID];
        if (id != BGSwitch->layerIDs[BGSwitch->screenID + 4]) {
            BGSwitch->layerIDs[BGSwitch->screenID + 4] = id;
            StateMachine_Run(BGSwitch->switchCallback[id]);
        }
    }
}

void BGSwitch_LateUpdate(void) {}

void BGSwitch_StaticUpdate(void) {}

void BGSwitch_Draw(void) {}

void BGSwitch_Create(void *data)
{
    RSDK_THIS(BGSwitch);
    if (!SceneInfo->inEditor) {
        self->updateRange.x = self->size.x;
        self->updateRange.y = self->size.y;
        self->active        = ACTIVE_BOUNDS;
    }
}

void BGSwitch_StageLoad(void)
{
    BGSwitch->layerIDs[4] = -1;
    BGSwitch->layerIDs[5] = -1;
    BGSwitch->layerIDs[6] = -1;
    BGSwitch->layerIDs[7] = -1;
}

void BGSwitch_EditorDraw(void)
{
    RSDK_THIS(BGSwitch);
    self->updateRange.x = self->size.x;
    self->updateRange.y = self->size.y;

    RSDK.SetSpriteAnimation(BGSwitch->aniFrames, 0, &self->animator, true, 5);
    RSDK.DrawSprite(&self->animator, NULL, false);

    //Bounds
    RSDK.DrawLine(self->position.x - self->size.x, self->position.y - self->size.y,
                  self->position.x + self->size.x, self->position.y - self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x - self->size.x, self->position.y + self->size.y,
                  self->position.x + self->size.x, self->position.y + self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x - self->size.x, self->position.y - self->size.y,
                  self->position.x - self->size.x, self->position.y + self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x + self->size.x, self->position.y - self->size.y,
                  self->position.x + self->size.x, self->position.y + self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
}

void BGSwitch_EditorLoad(void) { BGSwitch->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }

void BGSwitch_Serialize(void)
{
    RSDK_EDITABLE_VAR(BGSwitch, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(BGSwitch, VAR_UINT8, bgID);
}
