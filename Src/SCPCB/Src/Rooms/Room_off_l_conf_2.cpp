#include <bbblitz3d.h>
#include <bbmath.h>
#include <bbgraphics.h>
#include <bbaudio.h>

#include "../GameMain.h"
#include "../MapSystem.h"
#include "../Doors.h"
#include "../Items/Items.h"
#include "../Decals.h"
#include "../Particles.h"
#include "../Events.h"
#include "../Player.h"
#include "../NPCs/NPCs.h"
#include "../Audio.h"
#include "../MathUtils/MathUtils.h"
#include "../Menus/Menu.h"
#include "../Objects.h"
#include <bbblitz3d.h>
#include <bbmath.h>
#include <bbgraphics.h>
#include <bbaudio.h>

#include "../GameMain.h"
#include "../MapSystem.h"
#include "../Doors.h"
#include "../Items/Items.h"
#include "../Decals.h"
#include "../Particles.h"
#include "../Events.h"
#include "../Player.h"
#include "../NPCs/NPCs.h"
#include "../Audio.h"
#include "../MathUtils/MathUtils.h"
#include "../Menus/Menu.h"
#include "../Objects.h"
#include "Room_off_l_conf_2.h"

namespace CBN {

// Functions.
void FillRoom_off_l_conf_2(Room* r) {
    Door* d;
    Door* d2;
    SecurityCam* sc;
    Decal* de;
    Room* r2;
    SecurityCam* sc2;
    Item* it;
    int i;
    int xtemp;
    int ytemp;
    int ztemp;

    //, Bump
    int t1;

    d = CreateDoor(r->x + 240.0 * RoomScale, 0.0, r->z + 48.0 * RoomScale, 270, r, false, DOOR_TYPE_DEF, r->roomTemplate->name);
    bbPositionEntity(d->buttons[0], r->x + 224.0 * RoomScale, bbEntityY(d->buttons[0],true), r->z + 176.0 * RoomScale,true);
    bbPositionEntity(d->buttons[1], r->x + 256.0 * RoomScale, bbEntityY(d->buttons[1],true), bbEntityZ(d->buttons[1],true),true);
    d->autoClose = false;
    d->open = false;

    r->doors[0] = CreateDoor(r->x - 432.0 * RoomScale, 0.0, r->z, 90, r, false, DOOR_TYPE_DEF, "", "1234");
    bbPositionEntity(r->doors[0]->buttons[0], r->x - 416.0 * RoomScale, bbEntityY(r->doors[0]->buttons[0],true), r->z + 176.0 * RoomScale,true);
    bbFreeEntity(r->doors[0]->buttons[1]);
    r->doors[0]->buttons[1] = 0;
    r->doors[0]->autoClose = false;
    r->doors[0]->open = false;
    r->doors[0]->locked = true;

    de = CreateDecal(DECAL_CORROSION, r->x - 808.0 * RoomScale, 0.005, r->z - 72.0 * RoomScale, 90, bbRand(360), 0);
    bbEntityParent(de->obj, r->obj);
    de = CreateDecal(DECAL_BLOOD_SPREAD, r->x - 808.0 * RoomScale, 0.01, r->z - 72.0 * RoomScale, 90, bbRand(360), 0);
    de->size = 0.3;
    bbScaleSprite(de->obj, de->size, de->size);
    bbEntityParent(de->obj, r->obj);

    de = CreateDecal(DECAL_CORROSION, r->x - 432.0 * RoomScale, 0.01, r->z, 90, bbRand(360), 0);
    bbEntityParent(de->obj, r->obj);

    r->objects[0] = bbCreatePivot(r->obj);
    bbPositionEntity(r->objects[0], r->x - 808.0 * RoomScale, 1.0, r->z - 72.0 * RoomScale, true);

    it = CreatePaper("drL1", r->x - 688.0 * RoomScale, 1.0, r->z - 16.0 * RoomScale);
    bbEntityParent(it->collider, r->obj);

    it = CreatePaper("drL5", r->x - 808.0 * RoomScale, 1.0, r->z - 72.0 * RoomScale);
    bbEntityParent(it->collider, r->obj);
}

void UpdateEvent_off_l_conf_2(Event* e) {
    float dist;
    int i;
    int temp;
    int pvt;
    String strtemp;
    int j;
    int k;

    Particle* p;
    NPC* n;
    Room* r;
    Event* e2;
    Item* it;
    Emitter* em;
    SecurityCam* sc;
    SecurityCam* sc2;

    String CurrTrigger = "";

    float x;
    float y;
    float z;

    float angle;

    //[Block]
    if (mainPlayer->currRoom == e->room) {
        if (e->eventState == 0) {
            if (e->room->doors[0]->open == true) {
                if (e->room->doors[0]->openstate == 180) {
                    e->eventState = 1;
                    //TODO: load temp sound.
                    //TODO: fix
                    //PlaySound2(HorrorSFX(5))
                }
            } else {
                if (bbEntityDistance(mainPlayer->collider, e->room->doors[0]->obj)<1.5) & (RemoteDoorOn) {
                    e->room->doors[0]->open = true;
                }
            }
        } else {
            if (bbEntityDistance(e->room->objects[0], mainPlayer->collider) < 2.0) {
                //HeartBeatVolume = CurveValue(0.5, HeartBeatVolume, 5)
                mainPlayer->heartbeatIntensity = CurveValue(120, mainPlayer->heartbeatIntensity, 150);
                //TODO: fix
                //e\soundChannels[0] = LoopRangedSound(OldManSFX(4), e\soundChannels[0], mainPlayer\cam, e\room\obj, 5.0, 0.3)
                Curr106->state = Curr106->state-timing->tickDuration*3;
            }

        }
    }
    //[End Block]
}

}