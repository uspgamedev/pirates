
#include "world/utils/movtasks.h"
#include "world/utils/navigator.h"
#include "base/game.h"
#include "world/worldactor.h"
#include <string>
#include <utility>
#include <list>

namespace pirates {
namespace world {
namespace utils {

  /*********************/
  /* HueCyclingMovTask */
  /*********************/

HueCyclingMovTask::HueCyclingMovTask(const string& movtask_name, WorldActor* actor)
  : AsyncTask(movtask_name) {
    actor_ = actor;
    last_time_ = 0.0;
    speed_based_hue_ = 0.0f;
    warned_once_about_texture_blend_stage_missing_ = false;
    int x = 0;
}

const Colorf HueCyclingMovTask::UpdateSpeedBasedHueAndReturnColor(const Navigator* navi, const float dt) {
    
    // Update the hue.
    speed_based_hue_ += navi->speed()/50.0f;
    speed_based_hue_ -= (float)( 6*( (int)(speed_based_hue_)/6 ) ) ;
        // == speed_based_hue_ = speed_based_hue_%6

    // Bunch of temporary variables.
    float hue_mod = speed_based_hue_ - (float)( 2*( (int)(speed_based_hue_)/2 ) ); // == speed_based_hue_%2
    float triangle = (1 - fabs(hue_mod - 1));
    int hue_control = (int)( floor(speed_based_hue_) );

    // Do the magic. ( return Colorf(red,gree,blue,alpha) )
    switch(hue_control) {
        case 0: return Colorf(     1.0f, triangle,     0.0f, 1.0f); break;
        case 1: return Colorf( triangle,     1.0f,     0.0f, 1.0f); break;
        case 2: return Colorf(     0.0f,     1.0f, triangle, 1.0f); break;
        case 3: return Colorf(     0.0f, triangle,     1.0f, 1.0f); break;
        case 4: return Colorf( triangle,     0.0f,     1.0f, 1.0f); break;
        case 5: return Colorf(     1.0f,     0.0f, triangle, 1.0f); break;
        default: break;
    }
    return Colorf(0.0f);
}

AsyncTask::DoneStatus HueCyclingMovTask::do_task() {

    // Timing stuff.
    float dt = (float)( get_elapsed_time() - last_time_ );
    last_time_ = get_elapsed_time();

    // Find the WorldActor's Navigator and Node
    Navigator* navi = actor_->navigator(); // HEY! LISTEN! HEY! LISTEN! HEY! LISTEN! HEY! LISTEN! HEY! LISTEN!
    NodePath&  node = actor_->node();

    // Make the actor move.
    if(navi) {
        if( navi->Step(dt) ) {
            // Set position and look_at.
            node.set_pos( navi->pos() );
            LPoint3f look_at = navi->pos() + navi->dir();
            // Now we need to project this "look at" point to the current tangent plane.
            

            node.look_at( look_at, navi->up() );
        }

        TextureStage* ts_blend = actor_->texture_blend_stage();
        if(ts_blend) {
            ts_blend->set_color( UpdateSpeedBasedHueAndReturnColor(navi,dt) );
        } else if(!warned_once_about_texture_blend_stage_missing_) {
            fprintf(stderr, "Warning: at HueCyclingMovTask::do_task()\n");
            fprintf(stderr, "    a HueCyclingMovTask has been created for an WorldActor\n");
            fprintf(stderr, "    that has no texture_blend_stage_ .\n");
            warned_once_about_texture_blend_stage_missing_ = true;
        }
    } else {
        // In this case, the parent Navigator has died.
        // This task is abnormal and should commit seppuku in a horrible, messy way.
        _manager->remove(this);
        return AsyncTask::DS_exit;
    }

    // And... We're done.
    return AsyncTask::DS_cont;
}
/*
void HueCyclingMovTask::upon_birth(AsyncTaskManager *manager) {

    // Place the WorldActor on the world.
    Navigator* navi = actor_->navigator(); // HEY! LISTEN! HEY! LISTEN! HEY! LISTEN! HEY! LISTEN! HEY! LISTEN!
    NodePath&  node = actor_->node();

    if( navi ) {
        node.set_pos(navi->pos());
        LPoint3f look_at = navi->pos() + navi->dir();
        node.look_at(look_at, navi->up());
    }
    //TODO: what if navi has disappeared?
}*/

void HueCyclingMovTask::upon_death(AsyncTaskManager *manager, bool clean_exit) {

    if(!clean_exit) {
        fprintf(stderr, "HueCyclingMovTask::upon_death(...)\n");
        fprintf(stderr, "    A movement task has died horribly...\n");
        fprintf(stderr, "    (unclean exit, possibly due to its Navigator dying).\n");
    }

}

} // namespace utils
} // namespace world
} // namespace pirates