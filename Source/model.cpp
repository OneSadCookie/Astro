#include "model.hpp"

#include "globals.hpp"
#include "main.hpp"

const size_t MAX_TEXTURE_NAME_LENGTH = 32;

struct vertex {
  float x, y, z;
  float s, t;
};

struct gl_command {
  char     texture[MAX_TEXTURE_NAME_LENGTH];
  size_t   num_vertices;
  size_t*  vertices;
};

struct opaque_model {
  size_t      num_vertices;
  vertex*     vertices;
  size_t      num_strips;
  gl_command* strips;
  size_t      num_fans;
  gl_command* fans;
};

model::model(const char* name) {
  _model = NULL;
  size_t i;

  FILE* file = fopen(name, "r");
  if (file == NULL) {
    return;
  }
  
  _model = new opaque_model;
  if (_model == NULL) {
    return;
  }
  
  _model->num_vertices = 0;
  _model->vertices = NULL;
  _model->num_strips = 0;
  _model->strips = NULL;
  _model->num_fans = 0;
  _model->fans = NULL;
  
  fscanf(file, "vertices %zu\n", &(_model->num_vertices));
  //printf("%zu vertices in file\n", _model->num_vertices);
  
  if (_model->num_vertices != 0) {
    _model->vertices = new vertex[_model->num_vertices];
    for (i = 0; i < _model->num_vertices; ++i) {
      fscanf(file, "%f %f %f (%f %f)\n",
			 &(_model->vertices[i].x),
			 &(_model->vertices[i].y),
			 &(_model->vertices[i].z),
			 &(_model->vertices[i].s),
			 &(_model->vertices[i].t));
      /*printf("vertex %zu is at (%f, %f, %f) with texcoords (%f, %f)\n",
			 i,
			 _model->vertices[i].x,
			 _model->vertices[i].y,
			 _model->vertices[i].z,
			 _model->vertices[i].s,
			 _model->vertices[i].t);*/
    }
  }
  
  fscanf(file, "strips %zu\n", &(_model->num_strips));
  //printf("%zu strips in file\n", _model->num_strips);
  
  if (_model->num_strips != 0) {
    _model->strips = new gl_command[_model->num_strips];
    for (i = 0; i < _model->num_strips; ++i) {
      size_t j;
      fscanf(file, "%zu (%[^)])",
			 &(_model->strips[i].num_vertices),
			 _model->strips[i].texture);
      /*printf("read strip %zu (%s)",
             _model->strips[i].num_vertices, _model->strips[i].texture);*/

	  _model->strips[i].vertices = new size_t[_model->strips[i].num_vertices];
      for (j = 0; j < _model->strips[i].num_vertices; ++j) {
        fscanf(file, " %zu", _model->strips[i].vertices + j);
        //printf(" %zu", _model->strips[i].vertices[j]);
      }
      fscanf(file, "\n");
      //printf("\n");
    }
  }
  
  fscanf(file, "fans %zu\n", &(_model->num_fans));
  //printf("%zu fans in file\n", _model->num_fans);
  
  if (_model->num_fans != 0) {
    _model->fans = new gl_command[_model->num_fans];
    for (i = 0; i < _model->num_fans; ++i) {
      size_t j;
      fscanf(file, "%zu (%[^)])",
			 &(_model->fans[i].num_vertices),
       _model->fans[i].texture);
      /*printf("read fan %zu (%s)",
             _model->fans[i].num_vertices,
			 _model->fans[i].texture);*/
                   
      _model->fans[i].vertices = new size_t[_model->fans[i].num_vertices];
      for (j = 0; j < _model->fans[i].num_vertices; ++j) {
        fscanf(file, " %zu", _model->fans[i].vertices + j);
        //printf(" %zu", _model->fans[i].vertices[j]);
      }
      fscanf(file, "\n");
      //printf("\n");
    }
  }
  
  fclose(file);
}

model::~model() {
  size_t i;
  
  if (_model == NULL) {
    return;
  }
  
  if (_model->strips != NULL) {
    for (i = 0; i < _model->num_strips; ++i) {
      delete [] _model->strips[i].vertices;
    }
    delete [] _model->strips;
  }
  
  if (_model->fans != NULL) {
    for (i = 0; i < _model->num_fans; ++i) {
      delete [] _model->fans[i].vertices;
    }
    delete [] _model->fans;
  }
  
  delete _model;
  _model = NULL;
}

void model::draw() const {
  size_t i;

  assert(_model != NULL);
  assert(_model->vertices != NULL);
  if (_model->num_strips != 0) {
    assert(_model->strips != NULL);
  }
  if (_model->num_fans != 0) {
    assert(_model->fans != NULL);
  }
    
  for (i = 0; i < _model->num_strips; ++i) {
    size_t j;

	glBindTexture(GL_TEXTURE_2D, TEXTURES[_model->strips[i].texture]);
	
    glBegin(GL_TRIANGLE_STRIP);
    for (j = 0; j < _model->strips[i].num_vertices; ++j) {
	  glTexCoord2f(_model->vertices[_model->strips[i].vertices[j]].s,
				   _model->vertices[_model->strips[i].vertices[j]].t);
	  
      glVertex3f(_model->vertices[_model->strips[i].vertices[j]].x,
                 _model->vertices[_model->strips[i].vertices[j]].y,
                 _model->vertices[_model->strips[i].vertices[j]].z);
    }
    glEnd();
  }
  
  for (i = 0; i < _model->num_fans; ++i) {
    size_t j;

	glBindTexture(GL_TEXTURE_2D, TEXTURES[_model->fans[i].texture]);
	
    glBegin(GL_TRIANGLE_FAN);
    for (j = 0; j < _model->fans[i].num_vertices; ++j) {
	  glTexCoord2f(_model->vertices[_model->fans[i].vertices[j]].s,
				   _model->vertices[_model->fans[i].vertices[j]].t);
	  
      glVertex3f(_model->vertices[_model->fans[i].vertices[j]].x,
                 _model->vertices[_model->fans[i].vertices[j]].y,
                 _model->vertices[_model->fans[i].vertices[j]].z);
    }
    glEnd();
  }
}
