#include "RM2Definitions.h"
#include <fstream>

RM2Definitions::RM2Definitions(GraphicsResources* gfxRes) {
    graphicsResources = gfxRes;
}

RM2* RM2Definitions::loadRM2(PGE::String filename) {
    return new RM2(graphicsResources, filename);
}

void RM2Definitions::deleteRM2(RM2* rm2) {
    delete rm2;
}

void RM2Definitions::registerToEngine(ScriptManager* mgr) {
    asIScriptEngine* engine = mgr->getAngelScriptEngine();

    engine->RegisterObjectType("RM2", sizeof(RM2), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterGlobalFunction("RM2@ LoadRM2(string filename)", asMETHOD(RM2Definitions, loadRM2), asCALL_THISCALL_ASGLOBAL, this);
    engine->RegisterGlobalFunction("void DeleteRM2(RM2@ rm2)", asMETHOD(RM2Definitions, deleteRM2), asCALL_THISCALL_ASGLOBAL, this);
}

void RM2Definitions::cleanup() {
    //TODO: implement
}
