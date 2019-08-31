
#include <godot_chipmunk.h>
#include "chipmunk_pivot_joint.h"

ChipmunkPivotJoint::ChipmunkPivotJoint(cpConstraint *constraint)
    : ChipmunkConstraint(constraint)
{
    ERR_FAIL_COND(!cpConstraintIsPivotJoint(constraint));
}

Vector2 ChipmunkPivotJoint::get_anchor_a() const
{
    return GD(cpPivotJointGetAnchorA(constraint));
}

void ChipmunkPivotJoint::set_anchor_a(const Vector2 &value)
{
    cpPivotJointSetAnchorA(constraint, CP(value));
}

Vector2 ChipmunkPivotJoint::get_anchor_b() const
{
    return GD(cpPivotJointGetAnchorB(constraint));
}

void ChipmunkPivotJoint::set_anchor_b(const Vector2 &value)
{
    cpPivotJointSetAnchorB(constraint, CP(value));
}

void ChipmunkPivotJoint::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_anchor_a"), &ChipmunkPivotJoint::get_anchor_a);
    ClassDB::bind_method(D_METHOD("set_anchor_a", "anchor:Vector2"), &ChipmunkPivotJoint::set_anchor_a);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "anchor_a"), "set_anchor_a", "get_anchor_a");

    ClassDB::bind_method(D_METHOD("get_anchor_b"), &ChipmunkPivotJoint::get_anchor_b);
    ClassDB::bind_method(D_METHOD("set_anchor_b", "anchor:Vector2"), &ChipmunkPivotJoint::set_anchor_b);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "anchor_b"), "set_anchor_b", "get_anchor_b");
}

ChipmunkPivotJoint *ChipmunkConstraintFactory::pivot_joint(ChipmunkBody *body_a, ChipmunkBody *body_b, const Vector2 &anchor_a, const Vector2 &anchor_b)
{
    ERR_FAIL_NULL_V(body_a, NULL);
    ERR_FAIL_NULL_V(body_b, NULL);
    auto *constraint = cpPivotJointNew2(*body_a, *body_b, CP(anchor_a), CP(anchor_b));
    return memnew(ChipmunkPivotJoint(constraint));
}
