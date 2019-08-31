
#include <godot_chipmunk.h>
#include "chipmunk_pin_joint.h"

ChipmunkPinJoint::ChipmunkPinJoint(cpConstraint *constraint)
    : ChipmunkConstraint(constraint)
{
    ERR_FAIL_COND(!cpConstraintIsPinJoint(constraint));
}

Vector2 ChipmunkPinJoint::get_anchor_a() const
{
    return GD(cpPinJointGetAnchorA(constraint));
}

void ChipmunkPinJoint::set_anchor_a(const Vector2 &value)
{
    cpPinJointSetAnchorA(constraint, CP(value));
}

Vector2 ChipmunkPinJoint::get_anchor_b() const
{
    return GD(cpPinJointGetAnchorB(constraint));
}

void ChipmunkPinJoint::set_anchor_b(const Vector2 &value)
{
    cpPinJointSetAnchorB(constraint, CP(value));
}

float ChipmunkPinJoint::get_dist() const
{
    return cpPinJointGetDist(constraint);
}

void ChipmunkPinJoint::set_dist(float value)
{
    cpPinJointSetDist(constraint, value);
}

void ChipmunkPinJoint::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_anchor_a"), &ChipmunkPinJoint::get_anchor_a);
    ClassDB::bind_method(D_METHOD("set_anchor_a", "anchor"), &ChipmunkPinJoint::set_anchor_a);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "anchor_a"), "set_anchor_a", "get_anchor_a");

    ClassDB::bind_method(D_METHOD("get_anchor_b"), &ChipmunkPinJoint::get_anchor_b);
    ClassDB::bind_method(D_METHOD("set_anchor_b", "anchor"), &ChipmunkPinJoint::set_anchor_b);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "anchor_b"), "set_anchor_b", "get_anchor_b");

    ClassDB::bind_method(D_METHOD("get_dist"), &ChipmunkPinJoint::get_dist);
    ClassDB::bind_method(D_METHOD("set_dist", "dist"), &ChipmunkPinJoint::set_dist);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "dist"), "set_dist", "get_dist");
}

ChipmunkPinJoint *ChipmunkConstraintFactory::pin_joint(ChipmunkBody *body_a, ChipmunkBody *body_b, const Vector2 &anchor_a, const Vector2 &anchor_b)
{
    ERR_FAIL_NULL_V(body_a, NULL);
    ERR_FAIL_NULL_V(body_b, NULL);
    auto *constraint = cpPinJointNew(*body_a, *body_b, CP(anchor_a), CP(anchor_b));
    return memnew(ChipmunkPinJoint(constraint));
}
