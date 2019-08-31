
#include <godot_chipmunk.h>
#include "chipmunk_shape.h"

ChipmunkShape::ChipmunkShape(cpShape *shape)
    : shape(shape)
{
    cpShapeSetUserData(shape, get_instance_id());
}

ChipmunkShape::~ChipmunkShape()
{
    cpShapeFree(shape);
}

Rect2 ChipmunkShape::cache_bb()
{
    return GD(cpShapeCacheBB(shape));
}

Rect2 ChipmunkShape::update(const Transform2D &value)
{
    return GD(cpShapeUpdate(shape, CP(value)));
}

Rect2 ChipmunkShape::get_bb() const
{
    return GD(cpShapeGetBB(shape));
}

Dictionary ChipmunkShape::point_query(const Vector2 &point) const
{
    cpPointQueryInfo info;
    cpShapePointQuery(shape, CP(point), &info);
    Dictionary r;
    r["shape"] = this;
    r["point"] = GD(info.point);
    r["distance"] = info.distance;
    r["gradient"] = GD(info.gradient);
    return r;
}

Dictionary ChipmunkShape::segment_query(const Vector2 &a, const Vector2 &b, float radius) const
{
    cpSegmentQueryInfo info;
    cpShapeSegmentQuery(shape, CP(a), CP(b), radius, &info);
    Dictionary r;
    r["shape"] = this;
    r["point"] = GD(info.point);
    r["normal"] = GD(info.normal);
    r["alpha"] = info.alpha;
    return r;
}

Dictionary ChipmunkShape::collide(ChipmunkShape *other) const
{
    Dictionary r;
    ERR_FAIL_NULL_V(other, r);
    auto set = cpShapesCollide(*this, *other);
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

float ChipmunkShape::get_moment() const
{
    return cpShapeGetMoment(shape);
}

float ChipmunkShape::get_area() const
{
    return cpShapeGetArea(shape);
}

Vector2 ChipmunkShape::get_center_of_gravity() const
{
    return GD(cpShapeGetCenterOfGravity(shape));
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

Vector2 ChipmunkShape::get_surface_velocity() const
{
    return GD(cpShapeGetSurfaceVelocity(shape));
}

void ChipmunkShape::set_surface_velocity(const Vector2 &value)
{
    cpShapeSetSurfaceVelocity(shape, CP(value));
}

Variant ChipmunkShape::get_metadata() const
{
    return metadata;
}

void ChipmunkShape::set_metadata(const Variant &value)
{
    metadata = value;
}

int ChipmunkShape::get_collision_type() const
{
    return cpShapeGetCollisionType(shape);
}

void ChipmunkShape::set_collision_type(int value)
{
    cpShapeSetCollisionType(shape, value);
}

Ref<ChipmunkShapeFilter> ChipmunkShape::get_filter() const
{
    return memnew(ChipmunkShapeFilter(cpShapeGetFilter(shape)));
}

void ChipmunkShape::set_filter(const Ref<ChipmunkShapeFilter> &filter)
{
    ERR_FAIL_COND(filter.is_null());
    cpShapeSetFilter(shape, **filter);
}

void ChipmunkShape::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("cache_bb:Rect2"), &ChipmunkShape::cache_bb);
    ClassDB::bind_method(D_METHOD("update:Rect2", "transform:Transform2D"), &ChipmunkShape::update);
    ClassDB::bind_method(D_METHOD("get_bb:Rect2"), &ChipmunkShape::get_bb);

    ClassDB::bind_method(D_METHOD("point_query:Dictionary", "point:Vector2"), &ChipmunkShape::point_query);
    ClassDB::bind_method(D_METHOD("segment_query:Dictionary", "a:Vector2", "b:Vector2", "radius:float"), &ChipmunkShape::segment_query, DEFVAL(.0f));

    ClassDB::bind_method(D_METHOD("collide:Dictionary", "shape:ChipmunkShape"), &ChipmunkShape::collide);

    ClassDB::bind_method(D_METHOD("get_space"), &ChipmunkShape::get_space);
    ClassDB::bind_method(D_METHOD("get_body"), &ChipmunkShape::get_body);

    ClassDB::bind_method(D_METHOD("get_mass"), &ChipmunkShape::get_mass);
    ClassDB::bind_method(D_METHOD("set_mass", "mass"), &ChipmunkShape::set_mass);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "mass"), "set_mass", "get_mass");

    ClassDB::bind_method(D_METHOD("get_density"), &ChipmunkShape::get_density);
    ClassDB::bind_method(D_METHOD("set_density", "density"), &ChipmunkShape::set_density);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "density"), "set_density", "get_density");

    ClassDB::bind_method(D_METHOD("get_moment:float"), &ChipmunkShape::get_moment);
    ClassDB::bind_method(D_METHOD("get_area:float"), &ChipmunkShape::get_area);
    ClassDB::bind_method(D_METHOD("get_center_of_gravity:Vector2"), &ChipmunkShape::get_center_of_gravity);

    ClassDB::bind_method(D_METHOD("get_sensor"), &ChipmunkShape::get_sensor);
    ClassDB::bind_method(D_METHOD("set_sensor", "sensor"), &ChipmunkShape::set_sensor);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sensor"), "set_sensor", "get_sensor");

    ClassDB::bind_method(D_METHOD("get_elasticity"), &ChipmunkShape::get_elasticity);
    ClassDB::bind_method(D_METHOD("set_elasticity", "elasticity"), &ChipmunkShape::set_elasticity);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "elasticity"), "set_elasticity", "get_elasticity");

    ClassDB::bind_method(D_METHOD("get_friction"), &ChipmunkShape::get_friction);
    ClassDB::bind_method(D_METHOD("set_friction", "friction"), &ChipmunkShape::set_friction);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "friction"), "set_friction", "get_friction");

    ClassDB::bind_method(D_METHOD("get_surface_velocity"), &ChipmunkShape::get_surface_velocity);
    ClassDB::bind_method(D_METHOD("set_surface_velocity", "velocity"), &ChipmunkShape::set_surface_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "surface_velocity"), "set_surface_velocity", "get_surface_velocity");

    ClassDB::bind_method(D_METHOD("get_metadata"), &ChipmunkShape::get_metadata);
    ClassDB::bind_method(D_METHOD("set_metadata", "metadata:Variant"), &ChipmunkShape::set_metadata);

    ClassDB::bind_method(D_METHOD("get_collision_type"), &ChipmunkShape::get_collision_type);
    ClassDB::bind_method(D_METHOD("set_collision_type", "collision_type"), &ChipmunkShape::set_collision_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_type"), "set_collision_type", "get_collision_type");

    ClassDB::bind_method(D_METHOD("get_filter:ChipmunkShapeFilter"), &ChipmunkShape::get_filter);
    ClassDB::bind_method(D_METHOD("set_filter", "filter:ChipmunkShapeFilter"), &ChipmunkShape::set_filter);
}

ChipmunkShape *ChipmunkShape::get(const cpShape *shape)
{
    auto id = shape ? cpShapeGetUserData(shape) : 0;
    return (ChipmunkShape*)ObjectDB::get_instance(id);
}

// -- Shape Factory

ChipmunkShape *ChipmunkShapeFactory::circle(ChipmunkBody *body, float radius, const Vector2 &offset)
{
    auto *owner = body ? (cpBody*)*body : NULL;
    return wrap(cpCircleShapeNew(owner, radius, CP(offset)));
}

ChipmunkShape *ChipmunkShapeFactory::segment(ChipmunkBody *body, const Vector2 &a, const Vector2 &b, float radius)
{
    auto *owner = body ? (cpBody*)*body : NULL;
    return wrap(cpSegmentShapeNew(owner, CP(a), CP(b), radius));
}

ChipmunkShape *ChipmunkShapeFactory::box(ChipmunkBody *body, const Rect2 &box, float radius)
{
    auto *owner = body ? (cpBody*)*body : NULL;
    return wrap(cpBoxShapeNew2(owner, CP(box), radius));
}

ChipmunkShape *ChipmunkShapeFactory::poly(ChipmunkBody *body, const PoolVector2Array &verts, float radius)
{
    auto *owner = body ? (cpBody*)*body : NULL;
    PoolVector2Array::Read r = verts.read();
    auto *cp_verts = reinterpret_cast<const cpVect*>(r.ptr());
    return wrap(cpPolyShapeNewRaw(owner, verts.size(), cp_verts, radius));
}

void ChipmunkShapeFactory::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("circle", "body:ChipmunkBody", "radius:float", "offset:Vector2"), &ChipmunkShapeFactory::circle);
    ClassDB::bind_method(D_METHOD("segment", "body:ChipmunkBody", "a:Vector2", "b:Vector2", "radius:float"), &ChipmunkShapeFactory::segment);
    ClassDB::bind_method(D_METHOD("box", "body:ChipmunkBody", "box:Rect2", "radius:float"), &ChipmunkShapeFactory::box);
    ClassDB::bind_method(D_METHOD("poly", "body:ChipmunkBody", "verts:PoolVector2Array", "radius:float"), &ChipmunkShapeFactory::poly);
}

ChipmunkShape *ChipmunkShapeFactory::wrap(cpShape *shape)
{
    return memnew(ChipmunkShape(shape));
}
