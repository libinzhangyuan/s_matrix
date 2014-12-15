#include "ruby.h"
#include "ruby/intern.h"
#include "ruby/st.h"
#include "ruby/util.h"

#include "gmatx.h"

static VALUE t_init(VALUE self);
static VALUE t_init_copy(VALUE self, VALUE orig);
static void free_matx(class GMatx* pMatx);
static VALUE t_alloc(VALUE klass);


VALUE sm_init(VALUE self)
{
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
    delete pMatx;
}

static VALUE sm_alloc(VALUE klass)
{
    class GMatx* pMatx = new GMatx;
    return Data_Wrap_Struct(klass, 0, free_matx, pMatx);
}

static int t_add_row_hash_iter_func(VALUE key, VALUE value, VALUE p_row_hash)
{
    t_key_value_hash* p_hash = (t_key_value_hash*)p_row_hash;

    VALUE key_str = rb_funcall(key, rb_intern("to_s"), 0);
    VALUE value_str = rb_funcall(value, rb_intern("to_s"), 0);
    //printf("key: %s, value: %s, p_hash size %lu\n", StringValueCStr(key_str), StringValueCStr(value_str), p_hash->size());

    (*p_hash)[StringValueCStr(key_str)] = StringValueCStr(value_str);
    return ST_CONTINUE;
}


// row： {hp: 23, name: '2332342', 'text' => 'fsdfsd'}
static VALUE t_add_row(VALUE self, VALUE id, VALUE row)
{
    Check_Type(row, T_HASH);


    // 处理id     deal with id
    //
    VALUE id_str = rb_funcall(id, rb_intern("to_s"), 0);
    char* p_id_c_str = StringValuePtr(id_str);
    // id 为空字符串
    if (p_id_c_str[0] == 0)
        rb_raise(rb_eTypeError, "id can't be blank!");

    // 处理row    deal with row
    //
    t_key_value_hash row_hash;
    rb_hash_foreach(row, (int (*)(ANYARGS))t_add_row_hash_iter_func, (VALUE)&row_hash);

    // 存数据   store data
    //
    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->add_row(std::string(p_id_c_str), row_hash);
    //printf("\n\n after save a row\n%s\n", pMatx->to_s().c_str());

    return self;
}

static VALUE row_hash_to_ruby_hash(const t_key_value_hash& row_hash)
{
    VALUE ret_hash = rb_hash_new();
    for (t_key_value_hash::const_iterator iter = row_hash.begin(); iter != row_hash.end(); ++iter)
    {
        const std::string& key = iter->first;
        const std::string& value = iter->second;
        if (value == MatxRow::null_string)
            rb_hash_aset(ret_hash, rb_str_new_cstr(key.c_str()), Qnil);
        else
            rb_hash_aset(ret_hash, rb_str_new_cstr(key.c_str()), rb_str_new_cstr(value.c_str()));
    }
    return ret_hash;
}

static VALUE t_get_row(VALUE self, VALUE id)
{
    // 处理id      deal with id
    //
    VALUE id_str = rb_funcall(id, rb_intern("to_s"), 0);
    char* p_id_c_str = StringValuePtr(id_str);
    // id 为空字符串   check whether id blank
    if (p_id_c_str[0] == 0)
        rb_raise(rb_eTypeError, "id can't be blank!");


    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);

    t_key_value_hash row_hash = pMatx->get_row(std::string(p_id_c_str));
    return row_hash_to_ruby_hash(row_hash);
}

static void do_each_call_func(const std::string& key, const t_key_value_hash& row_content)
{
    rb_yield_values(2, rb_str_new_cstr(key.c_str()), row_hash_to_ruby_hash(row_content));
}

static VALUE t_each(VALUE self)
{
    //raise '' if block do not giving
    //raise '' if count of block params is not 2
//    if (rb_block_arity() != 2)
//      rb_raise(rb_eTypeError, "count of block params must 2");

    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->each_call(do_each_call_func);
    return self;
}

static VALUE t_to_s(VALUE self)
{
    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);

    return rb_str_new_cstr(pMatx->to_s().c_str());
}

static VALUE t_size(VALUE self)
{
    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);

    return INT2NUM(pMatx->size());
}


VALUE cSMatrix;

extern "C" void Init_s_matrix()
{
    cSMatrix = rb_define_class("SMatrix", rb_cObject);
    rb_define_alloc_func(cSMatrix, sm_alloc);
    rb_define_method(cSMatrix, "initialize", (VALUE(*)(ANYARGS))sm_init, 0);
    rb_define_method(cSMatrix, "initialize_copy", (VALUE(*)(ANYARGS))sm_init_copy, 1);
    rb_define_method(cSMatrix, "size", (VALUE(*)(ANYARGS))t_size, 0);
    rb_define_method(cSMatrix, "to_s", (VALUE(*)(ANYARGS))t_to_s, 0);
    rb_define_method(cSMatrix, "add_row", (VALUE(*)(ANYARGS))t_add_row, 2);
    rb_define_method(cSMatrix, "get_row", (VALUE(*)(ANYARGS))t_get_row, 1);
    rb_define_method(cSMatrix, "each", (VALUE(*)(ANYARGS))t_each, 0);
}
