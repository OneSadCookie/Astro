#ifndef MODEL_HPP
#define MODEL_HPP

struct opaque_model;

class model {
public:
  model(const char* filename);
  ~model();
  
  void draw() const;
private:
  opaque_model* _model;
};

#endif
