#ifndef PIRATES_WORLD_UTILS_WORLDVECTOR_H_
#define PIRATES_WORLD_UTILS_WORLDVECTOR_H_

namespace pirates {
namespace world {
namespace utils {

class WorldVector {
  public:
    WorldVector() : x_(0.0f), y_(0.0f) {}
    WorldVector(float& x, float& y) : x_(x), y(y) {}
    
    // getters
    float theta() const { return theta_; }
    float phi() const { return phi_; }

    // setters
    void set_theta(float& theta) { theta_ = theta; }
    void set_phi(float& phi) { phi_ = phi; } 

  private:
	union {
		struct { float x_, y_; };
		struct { float theta_, phi_; };
		struct { float val_[2]; };
	};
};

}
}
}

#endif

