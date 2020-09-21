
#include <godot_chipmunk.h>
#include "chipmunk_collision_handler.h"

ChipmunkCollisionHandler::ChipmunkCollisionHandler(ChipmunkSpace *space, cpCollisionHandler *handler)
    : default_handler(*handler), handler(handler), space(space)
	, begin_cb(0), pre_solve_cb(0), post_solve_cb(0), separate_cb(0)
{
}

ChipmunkCollisionHandler::~ChipmunkCollisionHandler()
{
    auto &id = handler->userData;
    space->collision_handlers.erase(id);

    // 'remove' the handler (by reverting its properties)
    memcpy(handler, &default_handler, sizeof(cpCollisionHandler));

    // clean-up callbacks
    memdelete_notnull(begin_cb);
    memdelete_notnull(pre_solve_cb);
    memdelete_notnull(post_solve_cb);
    memdelete_notnull(separate_cb);
}

ChipmunkCollisionHandler *ChipmunkCollisionHandler::get(cpSpace *space, cpDataPointer id)
{
    // TODO: sanity check?
    return ChipmunkSpace::get(space)->collision_handlers[id];
}

ChipmunkCollisionHandler *ChipmunkCollisionHandler::get_or_create(ChipmunkSpace *space, cpCollisionHandler *handler)
{
    // check if it already exists
    auto &id = handler->userData;
    if (id > 0) return space->collision_handlers[id];

    // create handler
    auto *obj = memnew(ChipmunkCollisionHandler(space, handler));

    // store and return it
    id = ++space->last_collision_handler_id;
    space->collision_handlers[id] = obj;
    return obj;
}

ChipmunkSpace *ChipmunkCollisionHandler::get_space() const
{
    return space;
}

int ChipmunkCollisionHandler::get_collision_type_a() const
{
    return handler->typeA;
}

int ChipmunkCollisionHandler::get_collision_type_b() const
{
    return handler->typeB;
}

void ChipmunkCollisionHandler::set_begin_func(Object *receiver, const StringName &method)
{
    memdelete_notnull(begin_cb);
    handler->beginFunc = default_handler.beginFunc;

    if (receiver)
    {
        begin_cb = memnew(ChipmunkCallbackBinding);
        begin_cb->id = receiver->get_instance_id();
        begin_cb->method = method;
        handler->beginFunc = &BeginFunc;
    }
}

void ChipmunkCollisionHandler::set_pre_solve_func(Object *receiver, const StringName &method)
{
    memdelete_notnull(pre_solve_cb);
    handler->preSolveFunc = default_handler.preSolveFunc;

    if (receiver)
    {
        pre_solve_cb = memnew(ChipmunkCallbackBinding);
        pre_solve_cb->id = receiver->get_instance_id();
        pre_solve_cb->method = method;
        handler->preSolveFunc = &PreSolveFunc;
    }
}

void ChipmunkCollisionHandler::set_post_solve_func(Object *receiver, const StringName &method)
{
    memdelete_notnull(post_solve_cb);
    handler->postSolveFunc = default_handler.postSolveFunc;

    if (receiver)
    {
        post_solve_cb = memnew(ChipmunkCallbackBinding);
        post_solve_cb->id = receiver->get_instance_id();
        post_solve_cb->method = method;
        handler->postSolveFunc = &PostSolveFunc;
    }
}

void ChipmunkCollisionHandler::set_separate_func(Object *receiver, const StringName &method)
{
    memdelete_notnull(separate_cb);
    handler->separateFunc = default_handler.separateFunc;

    if (receiver)
    {
        separate_cb = memnew(ChipmunkCallbackBinding);
        separate_cb->id = receiver->get_instance_id();
        separate_cb->method = method;
        handler->separateFunc = &SeparateFunc;
    }
}

void ChipmunkCollisionHandler::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_space:ChipmunkSpace"), &ChipmunkCollisionHandler::get_space);

    ClassDB::bind_method(D_METHOD("get_collision_type_a:int"), &ChipmunkCollisionHandler::get_collision_type_a);
    ClassDB::bind_method(D_METHOD("get_collision_type_b:int"), &ChipmunkCollisionHandler::get_collision_type_b);

    ClassDB::bind_method(D_METHOD("set_begin_func", "receiver:Object", "method:StringName"), &ChipmunkCollisionHandler::set_begin_func);
    ClassDB::bind_method(D_METHOD("set_pre_solve_func", "receiver:Object", "method:StringName"), &ChipmunkCollisionHandler::set_pre_solve_func);
    ClassDB::bind_method(D_METHOD("set_post_solve_func", "receiver:Object", "method:StringName"), &ChipmunkCollisionHandler::set_post_solve_func);
    ClassDB::bind_method(D_METHOD("set_separate_func", "receiver:Object", "method:StringName"), &ChipmunkCollisionHandler::set_separate_func);
}

bool ChipmunkCollisionHandler::BeginFunc(cpArbiter *arb, cpSpace *space, cpDataPointer userData)
{
    auto *handler = get(space, userData);
    ERR_FAIL_NULL_V(handler->begin_cb, true);
    handler->begin_cb->call(memnew(ChipmunkArbiter(arb)));
    return false;
}

bool ChipmunkCollisionHandler::PreSolveFunc(cpArbiter *arb, cpSpace *space, cpDataPointer userData)
{
    auto *handler = get(space, userData);
    ERR_FAIL_NULL_V(handler->pre_solve_cb, true);
    handler->pre_solve_cb->call(memnew(ChipmunkArbiter(arb)));
    return false;
}

void ChipmunkCollisionHandler::PostSolveFunc(cpArbiter *arb, cpSpace *space, cpDataPointer userData)
{
    auto *handler = get(space, userData);
    ERR_FAIL_NULL(handler->post_solve_cb);
    handler->post_solve_cb->call(memnew(ChipmunkArbiter(arb)));
}

void ChipmunkCollisionHandler::SeparateFunc(cpArbiter *arb, cpSpace *space, cpDataPointer userData)
{
    auto *handler = get(space, userData);
    ERR_FAIL_NULL(handler->separate_cb);
    handler->separate_cb->call(memnew(ChipmunkArbiter(arb)));
}
