#include "CallbackSystem.h"

// The CallbackSystem manages all about the entities, that has an delayed event (like bomb, that has an countdown to explode), etc.
// On every frame, decreases the time from the entities, by 1, and if the time is 0, then the callback function will be called.

void CallbackSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

void CallbackSystem::tick(World* world) {
    world->find<CallbackComponent>([](Entity* entity){
        auto callback = entity->get<CallbackComponent>();
        if (callback->time > 0) {
            callback->time--;
            if (callback->time == 0) callback->callback();
        } else {
            entity->remove<CallbackComponent>();
        }
    });
}

void CallbackSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void CallbackSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}
