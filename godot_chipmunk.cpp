
#include <godot_chipmunk.h>

void ChipmunkCallbackBinding::call()
{
    if (auto *obj = ObjectDB::get_instance(id))
    {
        Variant::CallError ce;
        obj->call(method, NULL, 0, ce);
    }
}

void ChipmunkCallbackBinding::call(const Variant &arg0)
{
    if (auto *obj = ObjectDB::get_instance(id))
    {
        const Variant *vp[] = { &arg0 };
        Variant::CallError ce;
        obj->call(method, vp, 1, ce);
    }
}

ChipmunkShapeFilter::ChipmunkShapeFilter()
    : group(CP_NO_GROUP), categories(CP_ALL_CATEGORIES), mask(CP_ALL_CATEGORIES)
{
}

void ChipmunkShapeFilter::set_group(int p_group)
{
    group = p_group;
}

int ChipmunkShapeFilter::get_group() const
{
    return group;
}

void ChipmunkShapeFilter::set_categories(int p_categories)
{
    categories = p_categories;
}

int ChipmunkShapeFilter::get_categories() const
{
    return categories;
}

void ChipmunkShapeFilter::set_mask(int p_mask)
{
    mask = p_mask;
}

int ChipmunkShapeFilter::get_mask() const
{
    return mask;
}

ChipmunkShapeFilter::operator cpShapeFilter() const
{
    return cpShapeFilterNew(group, categories, mask);
}

ChipmunkShapeFilter::ChipmunkShapeFilter(const cpShapeFilter &cp)
    : group(cp.group), categories(cp.categories), mask(cp.mask)
{
}

void ChipmunkShapeFilter::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_group", "group"), &ChipmunkShapeFilter::set_group);
    ClassDB::bind_method(D_METHOD("get_group"), &ChipmunkShapeFilter::get_group);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "group"), "set_group", "get_group");

    ClassDB::bind_method(D_METHOD("set_categories", "categories"), &ChipmunkShapeFilter::set_categories);
    ClassDB::bind_method(D_METHOD("get_categories"), &ChipmunkShapeFilter::get_categories);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "categories"), "set_categories", "get_categories");

    ClassDB::bind_method(D_METHOD("set_mask", "mask"), &ChipmunkShapeFilter::set_mask);
    ClassDB::bind_method(D_METHOD("get_mask"), &ChipmunkShapeFilter::get_mask);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "mask"), "set_mask", "get_mask");
}
