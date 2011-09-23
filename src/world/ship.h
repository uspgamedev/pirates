////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: ship.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PIRATES_WORLD_SHIP_H_
#define PIRATES_WORLD_SHIP_H_

#include "asyncTask.h"
#include "genericAsyncTask.h"
#include "world/worldactor.h"

namespace pirates {

namespace world {

namespace utils {
class Navigator;
}

class Ship : public WorldActor {
    
  public :   
    Ship::Ship(const string& name, const string& model) : WorldActor(name, model) {}

    const bool Initialize(const LPoint3f&   init_pos     = GAME()->planet()->default_start_pos(),
                          const LPoint3f&   init_look_at = GAME()->planet()->default_look_at(),
                          LVector3f&        init_up      = LVector3f(0.0f)  );

  private:

};

} // namespace world
} // namespace pirates

#endif
