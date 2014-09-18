#include "ruby.h"
#include "gmatx.h"


static VALUE t_init(VALUE self);
static VALUE t_init_copy(VALUE self, VALUE orig);
static void free_matx(class GMatx* pMatx);
static VALUE t_alloc(VALUE klass);


VALUE sm_init(VALUE self)
{
    printf("--------------init %lx\n", self);
  VALUE arr;

  arr = rb_ary_new();
  rb_iv_set(self, "@arr", arr);
  return self;
}

static VALUE sm_init_copy(VALUE self, VALUE orig)
{
    if (self == orig)
        return self;

    if ( (TYPE(orig) != T_DATA) || (RDATA(orig)->dfree != (RUBY_DATA_FUNC)free_matx) )
        rb_raise(rb_eTypeError, "Wrong origin obj when coping!");

    class GMatx* p_self = NULL;
    Data_Get_Struct(self, class GMatx, p_self);
    if (p_self == NULL) rb_raise(rb_eTypeError, "Encapsulate struct not alloc -- self");

    class GMatx* p_orig = NULL;
    Data_Get_Struct(orig, class GMatx, p_orig);
    if (p_orig == NULL) rb_raise(rb_eTypeError, "Encapsulate struct not alloc -- origin");

    *p_self = *p_orig;
    return self;
}

static void free_matx(class GMatx* pMatx)
{
    printf("----------------------------------------- free pmatx\n");
    delete pMatx;
}

static VALUE sm_alloc(VALUE klass)
{
    printf("-------------- alloc\n");
    class GMatx* pMatx = new GMatx;
    return Data_Wrap_Struct(klass, 0, free_matx, pMatx);
}


static VALUE t_get_i(VALUE self)
{
    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    return INT2NUM(pMatx->get_i());
}

static VALUE t_set_i(VALUE self, VALUE i)
{
    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->set_i(NUM2INT(i));
    return self;
}



VALUE cSMatrix;

extern "C" void Init_s_matrix() {
    cSMatrix = rb_define_class("SMatrix", rb_cObject);
    rb_define_alloc_func(cSMatrix, sm_alloc);
    rb_define_method(cSMatrix, "initialize", (VALUE(*)(ANYARGS))sm_init, 0);
    rb_define_method(cSMatrix, "initialize_copy", (VALUE(*)(ANYARGS))sm_init_copy, 1);
    rb_define_method(cSMatrix, "get_i", (VALUE(*)(ANYARGS))t_get_i, 0);
    rb_define_method(cSMatrix, "i", (VALUE(*)(ANYARGS))t_get_i, 0);
    rb_define_method(cSMatrix, "set_i", (VALUE(*)(ANYARGS))t_set_i, 1);
    rb_define_method(cSMatrix, "i=", (VALUE(*)(ANYARGS))t_set_i, 1);
}
