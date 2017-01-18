/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"
#include "globals.h"
#include "print_string.h"

#include "chipmunk/chipmunk.h"
#include "chipmunk_server.h"

void register_chipmunk_types()
{
    print_line(String("Chipmunk version: ") + String(cpVersionString));

    // initialize server
    memnew(ChipmunkServer);

    // register server
    Globals::get_singleton()->add_singleton(Globals::Singleton("ChipmunkServer", ChipmunkServer::get_singleton()));
}

void unregister_chipmunk_types()
{
    // destroy server
    memdelete(ChipmunkServer::get_singleton());
}