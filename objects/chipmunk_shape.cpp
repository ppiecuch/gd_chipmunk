
#include "chipmunk_shape.h"
#include "chipmunk_body.h"
#include "chipmunk_space.h"

ChipmunkShape::ChipmunkShape(cpShape *shape)
    : shape(shape)
{
    cpShapeSetUserData(shape, get_instance_ID());
}

ChipmunkShape::~ChipmunkShape()
{
    cpShapeFree(shape);
}

ChipmunkSpace *ChipmunkShape::get_space() const
{
    return ChipmunkSpace::get(cpShapeGetSpace(shape));
}

ChipmunkBody *ChipmunkShape::get_body() const
{
    return ChipmunkBody::get(cpShapeGetBody(shape));
}

float ChipmunkShape::get_mass() const
{
    return cpShapeGetMass(shape);
}

void ChipmunkShape::set_mass(float value)
{
    cpShapeSetMass(shape, value);
}

float ChipmunkShape::get_density() const
{
    return cpShapeGetDensity(shape);
}

void ChipmunkShape::set_density(float value)
{
    cpShapeSetDensity(shape, value);
}

bool ChipmunkShape::get_sensor() const
{
    return cpShapeGetSensor(shape);
}

void ChipmunkShape::set_sensor(bool value)
{
    cpShapeSetSensor(shape, value);
}

float ChipmunkShape::get_elasticity() const
{
    return cpShapeGetElasticity(shape);
}

void ChipmunkShape::set_elasticity(float value)
{
    cpShapeSetElasticity(shape, value);
}

float ChipmunkShape::get_friction() const
{
    return cpShapeGetFriction(shape);
}

void ChipmunkShape::set_friction(float value)
{
    cpShapeSetFriction(shape, value);
}

int ChipmunkShape::get_collision_type() const
{
    return cpShapeGetCollisionType(shape);
}

void ChipmunkShape::set_collision_type(int value)
{
    cpShapeSetCollisionType(shape, value);
}

void ChipmunkShape::_bind_methods()
{
    ObjectTypeDB::bind_method(_MD("get_space"), &ChipmunkShape::get_space);
    ObjectTypeDB::bind_method(_MD("get_body"), &ChipmunkShape::get_body);

    ObjectTypeDB::bind_method(_MD("get_mass"), &ChipmunkShape::get_mass);
    ObjectTypeDB::bind_method(_MD("set_mass", "mass:real"), &ChipmunkShape::set_mass);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "mass"), _SCS("set_mass"), _SCS("get_mass"));

    ObjectTypeDB::bind_method(_MD("get_density"), &ChipmunkShape::get_density);
    ObjectTypeDB::bind_method(_MD("set_density", "density:real"), &ChipmunkShape::set_density);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "density"), _SCS("set_density"), _SCS("get_density"));

    ObjectTypeDB::bind_method(_MD("get_sensor"), &ChipmunkShape::get_sensor);
    ObjectTypeDB::bind_method(_MD("set_sensor", "sensor:bool"), &ChipmunkShape::set_sensor);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sensor"), _SCS("set_sensor"), _SCS("get_sensor"));

    ObjectTypeDB::bind_method(_MD("get_elasticity"), &ChipmunkShape::get_elasticity);
    ObjectTypeDB::bind_method(_MD("set_elasticity", "elasticity:real"), &ChipmunkShape::set_elasticity);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "elasticity"), _SCS("set_elasticity"), _SCS("get_elasticity"));

    ObjectTypeDB::bind_method(_MD("get_friction"), &ChipmunkShape::get_friction);
    ObjectTypeDB::bind_method(_MD("set_friction", "friction:real"), &ChipmunkShape::set_friction);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "friction"), _SCS("set_friction"), _SCS("get_friction"));

    ObjectTypeDB::bind_method(_MD("get_collision_type"), &ChipmunkShape::get_collision_type);
    ObjectTypeDB::bind_method(_MD("set_collision_type", "collision_type:int"), &ChipmunkShape::set_collision_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_type"), _SCS("set_collision_type"), _SCS("get_collision_type"));
}

ChipmunkShape *ChipmunkShape::get(cpShape *shape)
{
    auto id = shape ? cpShapeGetUserData(shape) : 0;
    return (ChipmunkShape*)ObjectDB::get_instance(id);
}

// -- Shape Factory

ChipmunkShape *ChipmunkShapeFactory::circle(ChipmunkBody *body, float radius, const Vector2 &offset)
{
    return wrap(cpCircleShapeNew(*body, radius, CP(offset)));
}

ChipmunkShape *ChipmunkShapeFactory::segment(ChipmunkBody *body, const Vector2 &a, const Vector2 &b, float radius)
{
    return wrap(cpSegmentShapeNew(*body, CP(a), CP(b), radius));
}

ChipmunkShape *ChipmunkShapeFactory::box(ChipmunkBody *body, const Rect2 &box, float radius)
{
    return wrap(cpBoxShapeNew2(*body, CP(box), radius));
}

ChipmunkShape *ChipmunkShapeFactory::poly(ChipmunkBody *body, const Vector2Array &verts, float radius)
{
    Vector2Array::Read r = verts.read();
    auto *cp_verts = reinterpret_cast<const cpVect*>(r.ptr());
    return wrap(cpPolyShapeNewRaw(*body, verts.size(), cp_verts, radius));
}

void ChipmunkShapeFactory::_bind_methods()
{
    ObjectTypeDB::bind_method(_MD("circle", "body:ChipmunkBody", "radius:real", "offset:Vector2"), &ChipmunkShapeFactory::circle);
    ObjectTypeDB::bind_method(_MD("segment", "body:ChipmunkBody", "a:Vector2", "b:Vector2", "radius:real"), &ChipmunkShapeFactory::segment);
    ObjectTypeDB::bind_method(_MD("box", "body:ChipmunkBody", "box:Rect2", "radius:real"), &ChipmunkShapeFactory::box);
    ObjectTypeDB::bind_method(_MD("poly", "body:ChipmunkBody", "verts:Vector2Array", "radius:real"), &ChipmunkShapeFactory::poly);
}

ChipmunkShape *ChipmunkShapeFactory::wrap(cpShape *shape)
{
    return memnew(ChipmunkShape(shape));
}
