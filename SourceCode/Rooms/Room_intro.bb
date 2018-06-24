Function FillRoom_intro(r.Rooms)
    Local d.Doors, d2.Doors, sc.SecurityCams, de.Decals, r2.Rooms, sc2.SecurityCams
    Local it.Items, i%
    Local xtemp%, ytemp%, ztemp%

    Local t1%;, Bump

    r\objects[0] = CreatePivot()
    PositionEntity(r\objects[0], EntityX(r\obj) + 40.0 * RoomScale, 460.0 * RoomScale, EntityZ(r\obj) + 1072.0 * RoomScale)
    r\objects[1] = CreatePivot()
    PositionEntity(r\objects[1], EntityX(r\obj) - 80.0 * RoomScale, 100.0 * RoomScale, EntityZ(r\obj) + 526.0 * RoomScale)
    r\objects[2] = CreatePivot()
    PositionEntity(r\objects[2], EntityX(r\obj) - 128.0 * RoomScale, 100.0 * RoomScale, EntityZ(r\obj) + 320.0 * RoomScale)

    r\objects[3] = CreatePivot()
    PositionEntity(r\objects[3], EntityX(r\obj) + 660.0 * RoomScale, 100.0 * RoomScale, EntityZ(r\obj) + 526.0 * RoomScale)
    r\objects[4] = CreatePivot()
    PositionEntity(r\objects[4], EntityX(r\obj) + 700 * RoomScale, 100.0 * RoomScale, EntityZ(r\obj) + 320.0 * RoomScale)

    r\objects[5] = CreatePivot()
    PositionEntity(r\objects[5], EntityX(r\obj) + 1472.0 * RoomScale, 100.0 * RoomScale, EntityZ(r\obj) + 912.0 * RoomScale)

    For i = 0 To 5
        EntityParent(r\objects[i], r\obj)
    Next

    r\roomDoors[1] = CreateDoor(r\zone, EntityX(r\obj) + 288.0 * RoomScale, 0, EntityZ(r\obj) + 384.0 * RoomScale, 90, r, False, True)
    r\roomDoors[1]\autoClose = False ;: r\roomDoors[1]\locked = True
    r\roomDoors[1]\dir = 1 : r\roomDoors[1]\open = False

    FreeEntity(r\roomDoors[1]\buttons[0]) : r\roomDoors[1]\buttons[0] = 0
    FreeEntity(r\roomDoors[1]\buttons[1]) : r\roomDoors[1]\buttons[1] = 0

    de.Decals = CreateDecal(Rand(4, 5), EntityX(r\objects[5], True), 0.002, EntityZ(r\objects[5], True), 90, Rnd(360), 0)
    de\size = 1.2
    ScaleSprite(de\obj, de\size, de\size)

    For xtemp% = 0 To 1
        For ztemp% = 0 To 1
            de.Decals = CreateDecal(Rand(4, 6), r\x + 700.0 * RoomScale + xtemp * 700.0 * RoomScale + Rnd(-0.5, 0.5), Rnd(0.001, 0.0018), r\z + 600 * ztemp * RoomScale + Rnd(-0.5, 0.5), 90, Rnd(360), 0)
            de\size = Rnd(0.5, 0.8)
            de\alpha = Rnd(0.8, 1.0)
            ScaleSprite(de\obj, de\size, de\size)
        Next
    Next

    ;AddLight(r, r\x-224.0*RoomScale, r\y+640.0*RoomScale, r\z+128.0*RoomScale,2,2,200,200,200)
    ;AddLight(r, r\x-1056.0*RoomScale, r\y+608.0*RoomScale, r\z+416.0*RoomScale,2,2,200,200,200)

    r\roomDoors[2] = CreateDoor(r\zone, r\x - 1008.0 * RoomScale, 0, r\z - 688.0 * RoomScale, 90, r, True)
    r\roomDoors[2]\autoClose = False : r\roomDoors[2]\open = False : r\roomDoors[2]\locked = True
    FreeEntity(r\roomDoors[2]\buttons[0]) : r\roomDoors[2]\buttons[0] = 0
    FreeEntity(r\roomDoors[2]\buttons[1]) : r\roomDoors[2]\buttons[1] = 0

    r\roomDoors[3] = CreateDoor(r\zone, r\x - 2320.0 * RoomScale, 0, r\z - 1248.0 * RoomScale, 90, r, True)
    r\roomDoors[3]\autoClose = False : r\roomDoors[3]\open = True : r\roomDoors[3]\locked = True

    r\roomDoors[4] = CreateDoor(r\zone, r\x - 4352.0 * RoomScale, 0, r\z - 1248.0 * RoomScale, 90, r, True)
    r\roomDoors[4]\autoClose = False : r\roomDoors[4]\open = True : r\roomDoors[4]\locked = True

    ;the door in the office below the walkway
    r\roomDoors[7] = CreateDoor(r\zone, r\x - 3712.0 * RoomScale, -385*RoomScale, r\z - 128.0 * RoomScale, 0, r, True)
    r\roomDoors[7]\autoClose = False : r\roomDoors[7]\open = True

    d.Doors = CreateDoor(r\zone, r\x - 3712 * RoomScale, -385*RoomScale, r\z - 2336 * RoomScale, 0, r, False)
    d\locked = True

    ;the door from the concrete tunnel to the large hall
    d.Doors = CreateDoor(r\zone, r\x - 6864 * RoomScale, 0, r\z - 1248 * RoomScale, 90, r, True)
    d\autoClose = False
    d\locked = True

    ;the locked door to the lower level of the hall
    d.Doors = CreateDoor(r\zone, r\x - 5856 * RoomScale, 0, r\z - 1504 * RoomScale, 0, r, False)
    d\locked = True

    ;the door to the staircase in the office room
    d.Doors = CreateDoor(r\zone, r\x - 2432 * RoomScale, 0, r\z - 1000 * RoomScale, 0, r, False)
    PositionEntity(d\buttons[0], r\x - 2592 * RoomScale, EntityY(d\buttons[0],True), r\z - 1024 * RoomScale, True)
    PositionEntity(d\buttons[1], r\x - 2592 * RoomScale, EntityY(d\buttons[0],True), r\z - 992 * RoomScale, True)
    d\locked = True

    Local tex% = LoadTexture("GFX/Map/Textures/Door02.jpg")
    For ztemp = 0 To 1
        d.Doors = CreateDoor(r\zone, r\x - 5760 * RoomScale, 0, r\z + (320+896*ztemp) * RoomScale, 0, r, False)
        d\locked = True

        d.Doors = CreateDoor(r\zone, r\x - 8288 * RoomScale, 0, r\z + (320+896*ztemp) * RoomScale, 0, r, False)
        d\locked = True
        If (ztemp = 0) Then d\open = True

        For xtemp = 0 To 2
            d.Doors = CreateDoor(r\zone, r\x - (7424.0-512.0*xtemp) * RoomScale, 0, r\z + (1008.0-480.0*ztemp) * RoomScale, 180*(Not ztemp), r, False)
            EntityTexture(d\obj, tex)
            d\locked = True
            FreeEntity(d\obj2)
			d\obj2=0
            FreeEntity(d\buttons[0])
			d\buttons[0]=0
            FreeEntity(d\buttons[1])
			d\buttons[1]=0
        Next
        For xtemp = 0 To 4
            d.Doors = CreateDoor(r\zone, r\x - (5120.0-512.0*xtemp) * RoomScale, 0, r\z + (1008.0-480.0*ztemp) * RoomScale, 180*(Not ztemp), r, False)
            EntityTexture(d\obj, tex)
            d\locked = True
            FreeEntity(d\obj2)
			d\obj2=0
            FreeEntity(d\buttons[0])
			d\buttons[0]=0
            FreeEntity(d\buttons[1])
			d\buttons[1]=0

            If (xtemp = 2 And ztemp = 1) Then r\roomDoors[6] = d
        Next
    Next

    CreatePaper("docORI", r\x-(2914+1024)*RoomScale, 170.0*RoomScale, r\z+40*RoomScale)

    sc.SecurityCams = CreateSecurityCam(r\x - 4048.0 * RoomScale, r\y - 32.0 * RoomScale, r\z - 1232.0 * RoomScale, r, True)
    sc\angle = 270
    sc\turn = 45
    sc\room = r
    TurnEntity(sc\cameraObj, 20, 0, 0)
    EntityParent(sc\obj, r\obj)

    PositionEntity(sc\scrObj, r\x - 2256 * RoomScale, 224.0 * RoomScale, r\z - 928.0 * RoomScale)
    TurnEntity(sc\scrObj, 0, 90, 0)
    EntityParent(sc\scrObj, r\obj)

    r\objects[9] = LoadMesh("GFX/Map/173_2.b3d",r\obj)
    EntityType(r\objects[9],HIT_MAP)
    EntityPickMode(r\objects[9],2)
End Function

Function UpdateEvent173(e.Events)
	;TODO: REDO
End Function



;~IDEal Editor Parameters:
;~C#Blitz3D