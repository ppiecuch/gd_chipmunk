#ifndef GODOT_CHIPMUNK_H
#define GODOT_CHIPMUNK_H

#include <core/object.h>
#include <core/reference.h>

#include <chipmunk/chipmunk.h>

/***********************************************************************
 * Callback data
 **********************************************************************/
struct ChipmunkCallbackBinding
{
    ObjectID id;
    StringName method;

    void call();
    void call(const Variant &arg0);
};

/***********************************************************************
 * Enable conversion from Variant to any class derived from Object
 **********************************************************************/
template<class O>
struct VariantCaster<O*>
{
	static O *cast(const Variant &p_variant)
    {
        auto *obj = (Object*)p_variant;
        return obj ? Object::cast_to<O>(obj) : NULL;
	}
};

/***********************************************************************
 * cpShapeFilter
 **********************************************************************/
class ChipmunkShapeFilter : public Reference
{
    GDCLASS(ChipmunkShapeFilter, Reference);
public:
    /** Lifecycle */
    ChipmunkShapeFilter();

public:
    /** Chipmunk properties */
    int group;
    int categories;
    int mask;

public:
    /** Properties setters/getters */
    void set_group(int p_group);
    int get_group() const;

    void set_categories(int p_categories);
    int get_categories() const;

    void set_mask(int p_mask);
    int get_mask() const;

protected:
    /** Godot bindings */
	static void _bind_methods();

public:
    /** Chipmunk interoperability */
    operator cpShapeFilter() const;
    ChipmunkShapeFilter(const cpShapeFilter&);
};

/***********************************************************************
 * 2D Math interoperability
 **********************************************************************/
static inline cpVect CP(const Vector2 &v) { return cpv(v.x, v.y); }
static inline cpBB CP(const Rect2 &v) { return cpBBNew(v.position.x, v.position.y, v.position.x + v.size.x, v.position.y + v.size.y); }

static inline cpTransform CP(const Transform2D &v)
{
    return cpTransform
    {
        v[0].x, v.elements[0].y,
        v[1].x, v.elements[1].y,
        v[2].x, v.elements[2].y
    };
}

static inline cpShapeFilter CP(const Ref<ChipmunkShapeFilter> &v)
{
    return v.is_valid() ? (cpShapeFilter)**v : CP_SHAPE_FILTER_ALL;
}

static inline Vector2 GD(const cpVect &v) { return Vector2(v.x, v.y); }
static inline Rect2 GD(const cpBB &v) { return Rect2(v.l, v.b, v.r - v.l, v.t - v.b); }

static inline Transform2D GD(const cpTransform &v)
{
    return Transform2D(v.a, v.b, v.c, v.d, v.tx, v.ty);
}

/***********************************************************************
 * Include entities
 **********************************************************************/
class ChipmunkSpace;
class ChipmunkBody;
class ChipmunkShape;
class ChipmunkConstraint;
class ChipmunkCollisionHandler;
class ChipmunkArbiter;

#include "src/chipmunk_space.h"
#include "src/chipmunk_body.h"
#include "src/chipmunk_shape.h"
#include "src/chipmunk_constraint.h"

#include "src/chipmunk_collision_handler.h"
#include "src/chipmunk_arbiter.h"

#include "src/chipmunk_pin_joint.h"
#include "src/chipmunk_pivot_joint.h"
#include "src/chipmunk_damped_spring.h"

#endif
