
#include <godot_chipmunk.h>
#include "chipmunk_arbiter.h"

ChipmunkArbiter::ChipmunkArbiter(cpArbiter *arbiter)
    : arbiter(arbiter)
{
}

float ChipmunkArbiter::get_restitution() const
{
    return cpArbiterGetRestitution(arbiter);
}

void ChipmunkArbiter::set_restitution(float value)
{
    cpArbiterSetRestitution(arbiter, value);
}

float ChipmunkArbiter::get_friction() const
{
    return cpArbiterGetFriction(arbiter);
}

void ChipmunkArbiter::set_friction(float value)
{
    cpArbiterSetFriction(arbiter, value);
}

Vector2 ChipmunkArbiter::get_surface_velocity() const
{
    return GD(cpArbiterGetSurfaceVelocity(arbiter));
}

void ChipmunkArbiter::set_surface_velocity(const Vector2 &value)
{
    cpArbiterSetSurfaceVelocity(arbiter, CP(value));
}

Vector2 ChipmunkArbiter::get_total_impulse() const
{
    return GD(cpArbiterTotalImpulse(arbiter));
}

float ChipmunkArbiter::get_total_kinetic_energy() const
{
    return cpArbiterTotalKE(arbiter);
}

bool ChipmunkArbiter::ignore()
{
    return cpArbiterIgnore(arbiter);
}

ChipmunkShape *ChipmunkArbiter::get_shape_a() const
{
    CP_ARBITER_GET_SHAPES(arbiter, a, b);
    return ChipmunkShape::get(a);
}

ChipmunkShape *ChipmunkArbiter::get_shape_b() const
{
    CP_ARBITER_GET_SHAPES(arbiter, a, b);
    return ChipmunkShape::get(b);
}

ChipmunkBody *ChipmunkArbiter::get_body_a() const
{
    CP_ARBITER_GET_BODIES(arbiter, a, b);
    return ChipmunkBody::get(a);
}

ChipmunkBody *ChipmunkArbiter::get_body_b() const
{
    CP_ARBITER_GET_BODIES(arbiter, a, b);
    return ChipmunkBody::get(b);
}

Dictionary ChipmunkArbiter::get_contact_point_set() const
{
    auto set = cpArbiterGetContactPointSet(arbiter);
    Dictionary r;
    r["count"] = set.count;
    r["normal"] = GD(set.normal);
    Array points;
    for (int i = 0; i < set.count; ++i)
    {
        Dictionary r;
        r["pointA"] = GD(set.points[i].pointA);
        r["pointB"] = GD(set.points[i].pointB);
        r["distance"] = set.points[i].distance;
        points.push_back(r);
    }
    r["points"] = points;
    return r;
}

bool ChipmunkArbiter::is_first_contact() const
{
    return cpArbiterIsFirstContact(arbiter);
}

bool ChipmunkArbiter::is_removal() const
{
    return cpArbiterIsRemoval(arbiter);
}

void ChipmunkArbiter::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_restitution"), &ChipmunkArbiter::get_restitution);
    ClassDB::bind_method(D_METHOD("set_restitution", "restitution:float"), &ChipmunkArbiter::set_restitution);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "restitution"), "set_restitution", "get_restitution");

    ClassDB::bind_method(D_METHOD("get_friction"), &ChipmunkArbiter::get_friction);
    ClassDB::bind_method(D_METHOD("set_friction", "friction:float"), &ChipmunkArbiter::set_friction);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "friction"), "set_friction", "get_friction");

    ClassDB::bind_method(D_METHOD("get_surface_velocity"), &ChipmunkArbiter::get_surface_velocity);
    ClassDB::bind_method(D_METHOD("set_surface_velocity", "velocity"), &ChipmunkArbiter::set_surface_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "surface_velocity"), "set_surface_velocity", "get_surface_velocity");

    ClassDB::bind_method(D_METHOD("get_total_impulse:Vector2"), &ChipmunkArbiter::get_total_impulse);
    ClassDB::bind_method(D_METHOD("get_total_kinetic_energy:float"), &ChipmunkArbiter::get_total_kinetic_energy);

    ClassDB::bind_method(D_METHOD("ignore:bool"), &ChipmunkArbiter::ignore);

    ClassDB::bind_method(D_METHOD("get_shape_a:ChipmunkShape"), &ChipmunkArbiter::get_shape_a);
    ClassDB::bind_method(D_METHOD("get_shape_b:ChipmunkShape"), &ChipmunkArbiter::get_shape_b);

    ClassDB::bind_method(D_METHOD("get_body_a:ChipmunkBody"), &ChipmunkArbiter::get_body_a);
    ClassDB::bind_method(D_METHOD("get_body_b:ChipmunkBody"), &ChipmunkArbiter::get_body_b);

    ClassDB::bind_method(D_METHOD("get_contact_point_set:Dictionary"), &ChipmunkArbiter::get_contact_point_set);

    ClassDB::bind_method(D_METHOD("is_first_contact:bool"), &ChipmunkArbiter::is_first_contact);
    ClassDB::bind_method(D_METHOD("is_removal:bool"), &ChipmunkArbiter::is_removal);
}
