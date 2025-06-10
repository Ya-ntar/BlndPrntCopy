//
// Created by user on 25.05.2025.
//
#include "Mob.h"

std::atomic<std::size_t> Mob::globalIdCounter{0};
void Mob::beAffectedByItem(const ItemID &item) {
  if (type.canBeAffected(item)) {
    type.getReactionFor(item)(*this);
  }
}

Mob::Mob(QueuedMessageBus &bus, const MobType &typeRef) : SmartSubscriber(bus),
                                                          type(typeRef),
                                                          currentHp(typeRef.getMaxHp()),
                                                          uniqueId(globalIdCounter++) {}


