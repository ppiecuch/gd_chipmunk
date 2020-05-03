/* register_types.cpp */

#include <core/print_string.h>
#include <scene/2d/canvas_item.h>

#include "register_types.h"
#include "godot_chipmunk.h"

void register_gd_chipmunk_types()
{
    print_line(String("Chipmunk version: ") + String(cpVersionString));

    // register auxiliary types
    ClassDB::register_class<ChipmunkSpace>();
    ClassDB::register_class<ChipmunkBody>();
    ClassDB::register_virtual_class<ChipmunkShape>();
    ClassDB::register_virtual_class<ChipmunkConstraint>();

    ClassDB::register_virtual_class<ChipmunkDampedSpring>();
    ClassDB::register_virtual_class<ChipmunkPinJoint>();

    ClassDB::register_class<ChipmunkShapeFactory>();
    ClassDB::register_class<ChipmunkConstraintFactory>();

    ClassDB::register_virtual_class<ChipmunkCollisionHandler>();
    ClassDB::register_virtual_class<ChipmunkArbiter>();
    ClassDB::register_class<ChipmunkShapeFilter>();

    // Little hack
    ClassDB::bind_method(D_METHOD("set_block_transform_notify", "enable:bool"), &CanvasItem::set_block_transform_notify);
}

void unregister_gd_chipmunk_types()
{
}
