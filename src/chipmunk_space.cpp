
#include <godot_chipmunk.h>
#include "chipmunk_space.h"

ChipmunkSpace::ChipmunkSpace()
    : last_collision_handler_id(0)
{
    space = cpSpaceNew();
    cpSpaceSetUserData(space, get_instance_id());

    static_body = new ChipmunkBody();
    static_body->set_body_type(CP_BODY_TYPE_STATIC);
    add_body(static_body);
}

ChipmunkSpace::~ChipmunkSpace()
{
    remove_body(static_body);
    delete static_body;

    List<uint32_t> keys;
	collision_handlers.get_key_list(&keys);

    for (auto *e = keys.front(); e; e = e->next())
        memdelete(collision_handlers[e->get()]);

    cpSpaceFree(space);
}

int ChipmunkSpace::get_iterations() const
{
    return cpSpaceGetIterations(space);
}

void ChipmunkSpace::set_iterations(int value)
{
    cpSpaceSetIterations(space, value);
}

Vector2 ChipmunkSpace::get_gravity() const
{
    return GD(cpSpaceGetGravity(space));
}

void ChipmunkSpace::set_gravity(const Vector2 &value)
{
    cpSpaceSetGravity(space, CP(value));
}

float ChipmunkSpace::get_damping() const
{
    return cpSpaceGetDamping(space);
}

void ChipmunkSpace::set_damping(float value)
{
    cpSpaceSetDamping(space, value);
}

float ChipmunkSpace::get_idle_speed_threshold() const
{
    return cpSpaceGetIdleSpeedThreshold(space);
}

void ChipmunkSpace::set_idle_speed_threshold(float value)
{
    cpSpaceSetIdleSpeedThreshold(space, value);
}

float ChipmunkSpace::get_sleep_time_threshold() const
{
    return cpSpaceGetSleepTimeThreshold(space);
}

void ChipmunkSpace::set_sleep_time_threshold(float value)
{
    cpSpaceSetSleepTimeThreshold(space, value);
}

float ChipmunkSpace::get_collision_slop() const
{
    return cpSpaceGetCollisionSlop(space);
}

void ChipmunkSpace::set_collision_slop(float value)
{
    cpSpaceSetCollisionSlop(space, value);
}

float ChipmunkSpace::get_collision_bias() const
{
    return cpSpaceGetCollisionBias(space);
}

void ChipmunkSpace::set_collision_bias(float value)
{
    cpSpaceSetCollisionBias(space, value);
}

int ChipmunkSpace::get_collision_persistence() const
{
    return cpSpaceGetCollisionPersistence(space);
}

void ChipmunkSpace::set_collision_persistence(int value)
{
    cpSpaceSetCollisionPersistence(space, value);
}

Variant ChipmunkSpace::get_metadata() const
{
    return metadata;
}

void ChipmunkSpace::set_metadata(const Variant &value)
{
    metadata = value;
}

ChipmunkBody *ChipmunkSpace::get_static_body() const
{
    return static_body;
}

float ChipmunkSpace::get_current_timestep() const
{
    return cpSpaceGetCurrentTimeStep(space);
}

bool ChipmunkSpace::is_locked() const
{
    return cpSpaceIsLocked(space);
}

ChipmunkCollisionHandler *ChipmunkSpace::add_default_collision_handler()
{
    auto *handler = cpSpaceAddDefaultCollisionHandler(space);
    return ChipmunkCollisionHandler::get_or_create(this, handler);
}

ChipmunkCollisionHandler *ChipmunkSpace::add_collision_handler(int type_a, int type_b)
{
    auto *handler = cpSpaceAddCollisionHandler(space, type_a, type_b);
    return ChipmunkCollisionHandler::get_or_create(this, handler);
}

ChipmunkCollisionHandler *ChipmunkSpace::add_wildcard_handler(int type)
{
    auto *handler = cpSpaceAddWildcardHandler(space, type);
    return ChipmunkCollisionHandler::get_or_create(this, handler);
}

void ChipmunkSpace::add_body(ChipmunkBody *body)
{
    ERR_FAIL_NULL(body);
    cpSpaceAddBody(space, *body);
}

void ChipmunkSpace::remove_body(ChipmunkBody *body)
{
    ERR_FAIL_NULL(body);
    cpSpaceRemoveBody(space, *body);
}

bool ChipmunkSpace::contains_body(ChipmunkBody *body) const
{
    ERR_FAIL_NULL_V(body, false);
    return cpSpaceContainsBody(space, *body);
}

void ChipmunkSpace::add_shape(ChipmunkShape *shape)
{
    ERR_FAIL_NULL(shape);
    cpSpaceAddShape(space, *shape);
}

void ChipmunkSpace::remove_shape(ChipmunkShape *shape)
{
    ERR_FAIL_NULL(shape);
    cpSpaceRemoveShape(space, *shape);
}

bool ChipmunkSpace::contains_shape(ChipmunkShape *shape) const
{
    ERR_FAIL_NULL_V(shape, false);
    return cpSpaceContainsShape(space, *shape);
}

void ChipmunkSpace::add_constraint(ChipmunkConstraint *constraint)
{
    ERR_FAIL_NULL(constraint);
    cpSpaceAddConstraint(space, *constraint);
}

void ChipmunkSpace::remove_constraint(ChipmunkConstraint *constraint)
{
    ERR_FAIL_NULL(constraint);
    cpSpaceRemoveConstraint(space, *constraint);
}

bool ChipmunkSpace::contains_constraint(ChipmunkConstraint *constraint) const
{
    ERR_FAIL_NULL_V(constraint, false);
    return cpSpaceContainsConstraint(space, *constraint);
}

Array ChipmunkSpace::point_query(const Vector2 &point, float maxDistance, const Ref<ChipmunkShapeFilter> &filter) const
{
    struct Local
    {
        static void cb(cpShape *shape, cpVect point, cpFloat distance, cpVect gradient, void *user_data)
        {
            Dictionary r;
            r["shape"] = ChipmunkShape::get(shape);
            r["point"] = GD(point);
            r["distance"] = distance;
            r["gradient"] = GD(gradient);
            ((Array*)user_data)->push_back(r);
        }
    };

    Array r;
    cpSpacePointQuery(space, CP(point), maxDistance, CP(filter), &Local::cb, &r);
    return r;
}

Dictionary ChipmunkSpace::point_query_nearest(const Vector2 &point, float maxDistance, const Ref<ChipmunkShapeFilter> &filter) const
{
    cpPointQueryInfo info;
    Dictionary r;
    if (cpSpacePointQueryNearest(space, CP(point), maxDistance, CP(filter), &info))
    {
        r["shape"] = ChipmunkShape::get(info.shape);
        r["point"] = GD(info.point);
        r["distance"] = info.distance;
        r["gradient"] = GD(info.gradient);
    }
    return r;
}

Array ChipmunkSpace::segment_query(const Vector2 &start, const Vector2 &end, float radius, const Ref<ChipmunkShapeFilter> &filter) const
{
    struct Local
    {
        static void cb(cpShape *shape, cpVect point, cpVect normal, cpFloat alpha, void *user_data)
        {
            Dictionary r;
            r["shape"] = ChipmunkShape::get(shape);
            r["point"] = GD(point);
            r["normal"] = GD(normal);
            r["alpha"] = alpha;
            ((Array*)user_data)->push_back(r);
        }
    };

    Array r;
    cpSpaceSegmentQuery(space, CP(start), CP(end), radius, CP(filter), &Local::cb, &r);
    return r;
}

Dictionary ChipmunkSpace::segment_query_first(const Vector2 &start, const Vector2 &end, float radius, const Ref<ChipmunkShapeFilter> &filter) const
{
    cpSegmentQueryInfo info;
    Dictionary r;
    if (cpSpaceSegmentQueryFirst(space, CP(start), CP(end), radius, CP(filter), &info))
    {
        r["shape"] = ChipmunkShape::get(info.shape);
        r["point"] = GD(info.point);
        r["normal"] = GD(info.normal);
        r["alpha"] = info.alpha;
    }
    return r;
}

Array ChipmunkSpace::bb_query(const Rect2 &bb, const Ref<ChipmunkShapeFilter> &filter) const
{
    struct Local
    {
        static void cb(cpShape *shape, void *user_data)
        {
            auto *obj = ChipmunkShape::get(shape);
            ((Array*)user_data)->push_back(obj);
        }
    };

    Array r;
    cpSpaceBBQuery(space, CP(bb), CP(filter), &Local::cb, &r);
    return r;
}

Array ChipmunkSpace::shape_query(ChipmunkShape *shape) const
{
    struct Local
    {
        static void cb(cpShape *shape, cpContactPointSet *points, void *user_data)
        {
            // TODO: return points as well
            auto *obj = ChipmunkShape::get(shape);
            ((Array*)user_data)->push_back(obj);
        }
    };

    Array r;
    ERR_FAIL_NULL_V(shape, r);
    cpSpaceShapeQuery(space, *shape, &Local::cb, &r);
    return r;
}

Array ChipmunkSpace::get_bodies() const
{
    struct Local
    {
        static void cb(cpBody *body, void *user_data)
        {
            auto *obj = ChipmunkBody::get(body);
            ((Array*)user_data)->push_back(obj);
        }
    };

    Array r;
    cpSpaceEachBody(space, &Local::cb, &r);
    return r;
}

Array ChipmunkSpace::get_shapes() const
{
    struct Local
    {
        static void cb(cpShape *shape, void *user_data)
        {
            auto *obj = ChipmunkShape::get(shape);
            ((Array*)user_data)->push_back(obj);
        }
    };

    Array r;
    cpSpaceEachShape(space, &Local::cb, &r);
    return r;
}

Array ChipmunkSpace::get_constraints() const
{
    struct Local
    {
        static void cb(cpConstraint *constraint, void *user_data)
        {
            auto *obj = ChipmunkConstraint::get(constraint);
            ((Array*)user_data)->push_back(obj);
        }
    };

    Array r;
    cpSpaceEachConstraint(space, &Local::cb, &r);
    return r;
}

void ChipmunkSpace::step(float dt)
{
    cpSpaceStep(space, dt);
}

void ChipmunkSpace::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_iterations"), &ChipmunkSpace::get_iterations);
    ClassDB::bind_method(D_METHOD("set_iterations", "iterations"), &ChipmunkSpace::set_iterations);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "iterations"), "set_iterations", "get_iterations");

    ClassDB::bind_method(D_METHOD("get_gravity"), &ChipmunkSpace::get_gravity);
    ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &ChipmunkSpace::set_gravity);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "gravity"), "set_gravity", "get_gravity");

    ClassDB::bind_method(D_METHOD("get_damping"), &ChipmunkSpace::get_damping);
    ClassDB::bind_method(D_METHOD("set_damping", "damping"), &ChipmunkSpace::set_damping);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "damping"), "set_damping", "get_damping");

    ClassDB::bind_method(D_METHOD("get_idle_speed_threshold"), &ChipmunkSpace::get_idle_speed_threshold);
    ClassDB::bind_method(D_METHOD("set_idle_speed_threshold", "idle_speed_threshold"), &ChipmunkSpace::set_idle_speed_threshold);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "idle_speed_threshold"), "set_idle_speed_threshold", "get_idle_speed_threshold");

    ClassDB::bind_method(D_METHOD("get_sleep_time_threshold"), &ChipmunkSpace::get_sleep_time_threshold);
    ClassDB::bind_method(D_METHOD("set_sleep_time_threshold", "sleep_time_threshold"), &ChipmunkSpace::set_sleep_time_threshold);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "sleep_time_threshold"), "set_sleep_time_threshold", "get_sleep_time_threshold");

    ClassDB::bind_method(D_METHOD("get_collision_slop"), &ChipmunkSpace::get_collision_slop);
    ClassDB::bind_method(D_METHOD("set_collision_slop", "collision_slop"), &ChipmunkSpace::set_collision_slop);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "collision_slop"), "set_collision_slop", "get_collision_slop");

    ClassDB::bind_method(D_METHOD("get_collision_bias"), &ChipmunkSpace::get_collision_bias);
    ClassDB::bind_method(D_METHOD("set_collision_bias", "collision_bias"), &ChipmunkSpace::set_collision_bias);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "collision_bias"), "set_collision_bias", "get_collision_bias");

    ClassDB::bind_method(D_METHOD("get_collision_persistence"), &ChipmunkSpace::get_collision_persistence);
    ClassDB::bind_method(D_METHOD("set_collision_persistence", "collision_persistence"), &ChipmunkSpace::set_collision_persistence);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_persistence"), "set_collision_persistence", "get_collision_persistence");

    ClassDB::bind_method(D_METHOD("get_metadata"), &ChipmunkSpace::get_metadata);
    ClassDB::bind_method(D_METHOD("set_metadata", "metadata:Variant"), &ChipmunkSpace::set_metadata);

    ClassDB::bind_method(D_METHOD("get_static_body:ChipmunkBody"), &ChipmunkSpace::get_static_body);

    ClassDB::bind_method(D_METHOD("get_current_timestep"), &ChipmunkSpace::get_current_timestep);
    ClassDB::bind_method(D_METHOD("is_locked"), &ChipmunkSpace::is_locked);

    ClassDB::bind_method(D_METHOD("add_default_collision_handler:ChipmunkCollisionHandler"), &ChipmunkSpace::add_default_collision_handler);
    ClassDB::bind_method(D_METHOD("add_collision_handler:ChipmunkCollisionHandler", "type_a:int", "type_b:int"), &ChipmunkSpace::add_collision_handler);
    ClassDB::bind_method(D_METHOD("add_wildcard_handler:ChipmunkCollisionHandler", "type:int"), &ChipmunkSpace::add_wildcard_handler);

    ClassDB::bind_method(D_METHOD("add_body", "body:ChipmunkBody"), &ChipmunkSpace::add_body);
    ClassDB::bind_method(D_METHOD("remove_body", "body:ChipmunkBody"), &ChipmunkSpace::remove_body);
    ClassDB::bind_method(D_METHOD("contains_body", "body:ChipmunkBody"), &ChipmunkSpace::contains_body);

    ClassDB::bind_method(D_METHOD("add_shape", "shape:ChipmunkShape"), &ChipmunkSpace::add_shape);
    ClassDB::bind_method(D_METHOD("remove_shape", "shape:ChipmunkShape"), &ChipmunkSpace::remove_shape);
    ClassDB::bind_method(D_METHOD("contains_shape", "shape:ChipmunkShape"), &ChipmunkSpace::contains_shape);

    ClassDB::bind_method(D_METHOD("add_constraint", "constraint:ChipmunkConstraint"), &ChipmunkSpace::add_constraint);
    ClassDB::bind_method(D_METHOD("remove_constraint", "constraint:ChipmunkConstraint"), &ChipmunkSpace::remove_constraint);
    ClassDB::bind_method(D_METHOD("contains_constraint", "constraint:ChipmunkConstraint"), &ChipmunkSpace::contains_constraint);

    ClassDB::bind_method(D_METHOD("point_query:Array", "point:Vector2", "maxDistance:float", "filter:ChipmunkShapeFilter"), &ChipmunkSpace::point_query);
    ClassDB::bind_method(D_METHOD("point_query_nearest:Dictionary", "point:Vector2", "maxDistance:float", "filter:ChipmunkShapeFilter"), &ChipmunkSpace::point_query_nearest);

    ClassDB::bind_method(D_METHOD("segment_query:Array", "start:Vector2", "end:Vector2", "radius:float", "filter:ChipmunkShapeFilter"), &ChipmunkSpace::segment_query);
    ClassDB::bind_method(D_METHOD("segment_query_first:Dictionary", "start:Vector2", "end:Vector2", "radius:float", "filter:ChipmunkShapeFilter"), &ChipmunkSpace::segment_query_first);

    ClassDB::bind_method(D_METHOD("bb_query:Array", "bb:Rect2", "filter:ChipmunkShapeFilter"), &ChipmunkSpace::bb_query);
    ClassDB::bind_method(D_METHOD("shape_query:Array", "shape:ChipmunkShape"), &ChipmunkSpace::shape_query);

    ClassDB::bind_method(D_METHOD("get_bodies:Array"), &ChipmunkSpace::get_bodies);
    ClassDB::bind_method(D_METHOD("get_shapes:Array"), &ChipmunkSpace::get_shapes);
    ClassDB::bind_method(D_METHOD("get_constraints:Array"), &ChipmunkSpace::get_constraints);

    ClassDB::bind_method(D_METHOD("step", "dt:float"), &ChipmunkSpace::step);
}

ChipmunkSpace *ChipmunkSpace::get(const cpSpace *space)
{
    auto id = space ? cpSpaceGetUserData(space) : 0;
    return (ChipmunkSpace*)ObjectDB::get_instance(id);
}
