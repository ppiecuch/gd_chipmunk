
#include <godot_chipmunk.h>
#include "chipmunk_body.h"

ChipmunkBody::ChipmunkBody()
    : velocity_cb(0), position_cb(0)
{
    body = cpBodyNew(0, 0);
    cpBodySetUserData(body, get_instance_id());
}

ChipmunkBody::~ChipmunkBody()
{
    memdelete_notnull(position_cb);
    memdelete_notnull(velocity_cb);
    cpBodyFree(body);
}

void ChipmunkBody::activate()
{
    cpBodyActivate(body);
}

void ChipmunkBody::sleep()
{
    cpBodySleep(body);
}

bool ChipmunkBody::is_sleeping() const
{
    return cpBodyIsSleeping(body);
}

int ChipmunkBody::get_body_type() const
{
    return cpBodyGetType(body);
}

void ChipmunkBody::set_body_type(int value)
{
    cpBodySetType(body, static_cast<cpBodyType>(value));
}

ChipmunkSpace *ChipmunkBody::get_space() const
{
    return ChipmunkSpace::get(cpBodyGetSpace(body));
}

float ChipmunkBody::get_mass() const
{
    return cpBodyGetMass(body);
}

void ChipmunkBody::set_mass(float value)
{
    cpBodySetMass(body, value);
}

float ChipmunkBody::get_moment() const
{
    return cpBodyGetMoment(body);
}

void ChipmunkBody::set_moment(float value)
{
    cpBodySetMoment(body, value);
}

void ChipmunkBody::set_moment_to_infinity()
{
    cpBodySetMoment(body, INFINITY);
}

Vector2 ChipmunkBody::get_position() const
{
    return GD(cpBodyGetPosition(body));
}

void ChipmunkBody::set_position(const Vector2 &value)
{
    cpBodySetPosition(body, CP(value));
}

Vector2 ChipmunkBody::get_center_of_gravity() const
{
    return GD(cpBodyGetCenterOfGravity(body));
}

void ChipmunkBody::set_center_of_gravity(const Vector2 &value)
{
    cpBodySetCenterOfGravity(body, CP(value));
}

Vector2 ChipmunkBody::get_velocity() const
{
    return GD(cpBodyGetVelocity(body));
}

void ChipmunkBody::set_velocity(const Vector2 &value)
{
    cpBodySetVelocity(body, CP(value));
}

Vector2 ChipmunkBody::get_force() const
{
    return GD(cpBodyGetForce(body));
}

void ChipmunkBody::set_force(const Vector2 &value)
{
    cpBodySetForce(body, CP(value));
}

float ChipmunkBody::get_angle() const
{
    return cpBodyGetAngle(body);
}

void ChipmunkBody::set_angle(float value)
{
    cpBodySetAngle(body, value);
}

float ChipmunkBody::get_angular_velocity() const
{
    return cpBodyGetAngularVelocity(body);
}

void ChipmunkBody::set_angular_velocity(float value)
{
    cpBodySetAngularVelocity(body, value);
}

float ChipmunkBody::get_torque() const
{
    return cpBodyGetTorque(body);
}

void ChipmunkBody::set_torque(float value)
{
    cpBodySetTorque(body, value);
}

Variant ChipmunkBody::get_metadata() const
{
    return metadata;
}

void ChipmunkBody::set_metadata(const Variant &value)
{
    metadata = value;
}

void ChipmunkBody::set_velocity_update_callback(Object *receiver, const StringName &method)
{
    memdelete_notnull(velocity_cb);
    cpBodySetVelocityUpdateFunc(body, cpBodyUpdateVelocity);

    if (receiver)
    {
        velocity_cb = memnew(ChipmunkCallbackBinding);
        velocity_cb->id = receiver->get_instance_id();
        velocity_cb->method = method;
        cpBodySetVelocityUpdateFunc(body, VelocityUpdateFunc);
    }
}

void ChipmunkBody::set_position_update_callback(Object *receiver, const StringName &method)
{
    memdelete_notnull(position_cb);
    cpBodySetPositionUpdateFunc(body, cpBodyUpdatePosition);

    if (receiver)
    {
        position_cb = memnew(ChipmunkCallbackBinding);
        position_cb->id = receiver->get_instance_id();
        position_cb->method = method;
        cpBodySetPositionUpdateFunc(body, PositionUpdateFunc);
    }
}

void ChipmunkBody::apply_force_at_world_point(const Vector2 &force, const Vector2 &point)
{
    cpBodyApplyForceAtWorldPoint(body, CP(force), CP(point));
}

void ChipmunkBody::apply_force_at_local_point(const Vector2 &force, const Vector2 &point)
{
    cpBodyApplyForceAtLocalPoint(body, CP(force), CP(point));
}

void ChipmunkBody::apply_impulse_at_world_point(const Vector2 &impulse, const Vector2 &point)
{
    cpBodyApplyImpulseAtWorldPoint(body, CP(impulse), CP(point));
}

void ChipmunkBody::apply_impulse_at_local_point(const Vector2 &impulse, const Vector2 &point)
{
    cpBodyApplyImpulseAtLocalPoint(body, CP(impulse), CP(point));
}

Vector2 ChipmunkBody::get_velocity_at_world_point(const Vector2 &point)
{
    return GD(cpBodyGetVelocityAtWorldPoint(body, CP(point)));
}

Vector2 ChipmunkBody::get_velocity_at_local_point(const Vector2 &point)
{
    return GD(cpBodyGetVelocityAtLocalPoint(body, CP(point)));
}

float ChipmunkBody::get_kinetic_energy() const
{
    return cpBodyKineticEnergy(body);
}

Array ChipmunkBody::get_shapes() const
{
    struct Local
    {
        static void cb(cpBody *body, cpShape *shape, void *user_data)
        {
            auto *obj = ChipmunkShape::get(shape);
            ((Array*)user_data)->push_back(obj);
        }
    };

    Array r;
    cpBodyEachShape(body, &Local::cb, &r);
    return r;
}

Array ChipmunkBody::get_constraints() const
{
    struct Local
    {
        static void cb(cpBody *body, cpConstraint *constraint, void *user_data)
        {
            auto *obj = ChipmunkConstraint::get(constraint);
            ((Array*)user_data)->push_back(obj);
        }
    };

    Array r;
    cpBodyEachConstraint(body, &Local::cb, &r);
    return r;
}

Array ChipmunkBody::get_arbiters() const
{
    struct Local
    {
        static void cb(cpBody *body, cpArbiter *arbiter, void *user_data)
        {
            Ref<ChipmunkArbiter> ref = memnew(ChipmunkArbiter(arbiter));
            ((Array*)user_data)->push_back(ref);
        }
    };

    Array r;
    cpBodyEachArbiter(body, &Local::cb, &r);
    return r;
}

void ChipmunkBody::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("activate"), &ChipmunkBody::activate);
    ClassDB::bind_method(D_METHOD("sleep"), &ChipmunkBody::sleep);
    ClassDB::bind_method(D_METHOD("is_sleeping"), &ChipmunkBody::is_sleeping);

    ClassDB::bind_method(D_METHOD("get_body_type"), &ChipmunkBody::get_body_type);
    ClassDB::bind_method(D_METHOD("set_body_type", "type:int"), &ChipmunkBody::set_body_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "body_type"), "set_body_type", "get_body_type");

    ClassDB::bind_method(D_METHOD("get_space"), &ChipmunkBody::get_space);

    ClassDB::bind_method(D_METHOD("get_mass"), &ChipmunkBody::get_mass);
    ClassDB::bind_method(D_METHOD("set_mass", "mass:float"), &ChipmunkBody::set_mass);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "mass"), "set_mass", "get_mass");

    ClassDB::bind_method(D_METHOD("get_moment"), &ChipmunkBody::get_moment);
    ClassDB::bind_method(D_METHOD("set_moment", "moment:float"), &ChipmunkBody::set_moment);
    ClassDB::bind_method(D_METHOD("set_moment_to_infinity"), &ChipmunkBody::set_moment_to_infinity);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "moment"), "set_moment", "get_moment");

    ClassDB::bind_method(D_METHOD("get_position"), &ChipmunkBody::get_position);
    ClassDB::bind_method(D_METHOD("set_position", "position:Vector2"), &ChipmunkBody::set_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "position"), "set_position", "get_position");

    ClassDB::bind_method(D_METHOD("get_center_of_gravity"), &ChipmunkBody::get_center_of_gravity);
    ClassDB::bind_method(D_METHOD("set_center_of_gravity", "center_of_gravity:Vector2"), &ChipmunkBody::set_center_of_gravity);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center_of_gravity"), "set_center_of_gravity", "get_center_of_gravity");

    ClassDB::bind_method(D_METHOD("get_velocity"), &ChipmunkBody::get_velocity);
    ClassDB::bind_method(D_METHOD("set_velocity", "velocity:Vector2"), &ChipmunkBody::set_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity"), "set_velocity", "get_velocity");

    ClassDB::bind_method(D_METHOD("get_force"), &ChipmunkBody::get_force);
    ClassDB::bind_method(D_METHOD("set_force", "force:Vector2"), &ChipmunkBody::set_force);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "force"), "set_force", "get_force");

    ClassDB::bind_method(D_METHOD("get_angle"), &ChipmunkBody::get_angle);
    ClassDB::bind_method(D_METHOD("set_angle", "angle:float"), &ChipmunkBody::set_angle);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "angle"), "set_angle", "get_angle");

    ClassDB::bind_method(D_METHOD("get_angular_velocity"), &ChipmunkBody::get_angular_velocity);
    ClassDB::bind_method(D_METHOD("set_angular_velocity", "angular_velocity:float"), &ChipmunkBody::set_angular_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "angular_velocity"), "set_angular_velocity", "get_angular_velocity");

    ClassDB::bind_method(D_METHOD("get_torque"), &ChipmunkBody::get_torque);
    ClassDB::bind_method(D_METHOD("set_torque", "torque:float"), &ChipmunkBody::set_torque);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "torque"), "set_torque", "get_torque");

    ClassDB::bind_method(D_METHOD("get_metadata"), &ChipmunkBody::get_metadata);
    ClassDB::bind_method(D_METHOD("set_metadata", "metadata:Variant"), &ChipmunkBody::set_metadata);

    ClassDB::bind_method(D_METHOD("set_velocity_update_callback", "receiver:Object", "method:StringName"), &ChipmunkBody::set_velocity_update_callback);
    ClassDB::bind_method(D_METHOD("set_position_update_callback", "receiver:Object", "method:StringName"), &ChipmunkBody::set_position_update_callback);

    ClassDB::bind_method(D_METHOD("apply_force_at_world_point", "force:Vector2", "point:Vector2"), &ChipmunkBody::apply_force_at_world_point);
    ClassDB::bind_method(D_METHOD("apply_force_at_local_point", "force:Vector2", "point:Vector2"), &ChipmunkBody::apply_force_at_local_point);

    ClassDB::bind_method(D_METHOD("apply_impulse_at_world_point", "impulse:Vector2", "point:Vector2"), &ChipmunkBody::apply_impulse_at_world_point);
    ClassDB::bind_method(D_METHOD("apply_impulse_at_local_point", "impulse:Vector2", "point:Vector2"), &ChipmunkBody::apply_impulse_at_local_point);

    ClassDB::bind_method(D_METHOD("get_velocity_at_world_point", "point:Vector2"), &ChipmunkBody::get_velocity_at_world_point);
    ClassDB::bind_method(D_METHOD("get_velocity_at_local_point", "point:Vector2"), &ChipmunkBody::get_velocity_at_local_point);

    ClassDB::bind_method(D_METHOD("get_kinetic_energy"), &ChipmunkBody::get_kinetic_energy);

    ClassDB::bind_method(D_METHOD("get_shapes:Array"), &ChipmunkBody::get_shapes);
    ClassDB::bind_method(D_METHOD("get_constraints:Array"), &ChipmunkBody::get_constraints);
    ClassDB::bind_method(D_METHOD("get_arbiters:Array"), &ChipmunkBody::get_arbiters);

    ClassDB::bind_integer_constant(get_class_static(), StringName(), "DYNAMIC", CP_BODY_TYPE_DYNAMIC);
    ClassDB::bind_integer_constant(get_class_static(), StringName(), "KINEMATIC", CP_BODY_TYPE_KINEMATIC);
    ClassDB::bind_integer_constant(get_class_static(), StringName(), "STATIC", CP_BODY_TYPE_STATIC);

    BIND_CONSTANT(CP_BODY_TYPE_DYNAMIC);
    BIND_CONSTANT(CP_BODY_TYPE_KINEMATIC);
    BIND_CONSTANT(CP_BODY_TYPE_STATIC);
}

void ChipmunkBody::VelocityUpdateFunc(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
    cpBodyUpdateVelocity(body, gravity, damping, dt);
    auto *obj = ChipmunkBody::get(body);
    if (obj->velocity_cb)
        obj->velocity_cb->call(obj);
}

void ChipmunkBody::PositionUpdateFunc(cpBody *body, cpFloat dt)
{
    cpBodyUpdatePosition(body, dt);
    auto *obj = ChipmunkBody::get(body);
    if (obj->position_cb)
        obj->position_cb->call(obj);
}

ChipmunkBody *ChipmunkBody::get(const cpBody *body)
{
    auto id = body ? cpBodyGetUserData(body) : 0;
    return (ChipmunkBody*)ObjectDB::get_instance(id);
}
