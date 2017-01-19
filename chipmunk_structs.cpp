
#include "chipmunk_structs.h"

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

ChipmunkShapeFilter::ChipmunkShapeFilter()
    : group(CP_NO_GROUP), categories(CP_ALL_CATEGORIES), mask(CP_ALL_CATEGORIES)
{
}

ChipmunkShapeFilter::ChipmunkShapeFilter(const cpShapeFilter &cp)
    : group(cp.group), categories(cp.categories), mask(cp.mask)
{
}

ChipmunkShapeFilter::operator cpShapeFilter() const
{
    return cpShapeFilterNew(group, categories, mask);
}

void ChipmunkShapeFilter::_bind_methods()
{
    ObjectTypeDB::bind_method(_MD("set_group", "group"), &ChipmunkShapeFilter::set_group);
    ObjectTypeDB::bind_method(_MD("get_group"), &ChipmunkShapeFilter::get_group);

    ObjectTypeDB::bind_method(_MD("set_categories", "categories"), &ChipmunkShapeFilter::set_categories);
    ObjectTypeDB::bind_method(_MD("get_categories"), &ChipmunkShapeFilter::get_categories);

    ObjectTypeDB::bind_method(_MD("set_mask", "mask"), &ChipmunkShapeFilter::set_mask);
    ObjectTypeDB::bind_method(_MD("get_mask"), &ChipmunkShapeFilter::get_mask);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "group"), _SCS("set_group"), _SCS("get_group"));
    ADD_PROPERTY(PropertyInfo(Variant::INT, "categories"), _SCS("set_categories"), _SCS("get_categories"));
    ADD_PROPERTY(PropertyInfo(Variant::INT, "mask"), _SCS("set_mask"), _SCS("get_mask"));
}