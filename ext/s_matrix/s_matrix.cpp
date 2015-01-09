#include "ruby.h"
#include "ruby/intern.h"
#include "ruby/st.h"
#include "ruby/util.h"

#include "gmatx.h"
#include "matx_hash_util.h"

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
    const t_key_value_hash& row_hash = MatxHashUtil::rb_hash_to_key_value_hash(row);

    // 存数据   store data
    //
    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->add_row(std::string(p_id_c_str), row_hash);
    //printf("\n\n after save a row\n%s\n", pMatx->to_s().c_str());

    return self;
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
    return MatxHashUtil::key_value_hash_to_ruby_hash(row_hash);
}

struct FindCallbackArgs
{
    std::string id;
    t_key_value_hash row_content;

    t_key_value_hash search_keys;
};
static bool callback_func__find(const std::string& id, const t_key_value_hash& row_content, void* args)
{
    FindCallbackArgs* pargs = (FindCallbackArgs*)(args);
    for (t_key_value_hash::const_iterator iter = pargs->search_keys.begin();
        iter != pargs->search_keys.end(); ++iter)
    {
         const std::string& search_key = iter->first;
         const std::string& search_value = iter->second;

         t_key_value_hash::const_iterator row_content_iter = row_content.find(search_key);

         // compare nil value.    -- s.find({a: '3', b: nil})   when compile b: nil
         if (search_value == MatxRow::null_string)
         {
             if (row_content_iter != row_content.end() && row_content_iter->second != search_value)
                 return GMatx::t_continue;
             continue;
         }

         // compare value not nil
         if (row_content_iter == row_content.end() || row_content_iter->second != search_value)
             return GMatx::t_continue;
    }
    pargs->id = id;
    pargs->row_content = row_content;
    return GMatx::t_break;
}

static VALUE t_find(VALUE self, VALUE search_keys_ruby_hash)
{
    Check_Type(search_keys_ruby_hash, T_HASH);

    struct FindCallbackArgs args;
    args.search_keys = MatxHashUtil::rb_hash_to_key_value_hash(search_keys_ruby_hash);

    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->each_call(callback_func__find, &args);
    if (args.id.size() > 0)
    {
        VALUE ret_ary = rb_ary_new();
        rb_ary_push(ret_ary, rb_str_new_cstr(args.id.c_str()));
        rb_ary_push(ret_ary, MatxHashUtil::key_value_hash_to_ruby_hash(args.row_content));
        return ret_ary;
    }
    return Qnil;
}

struct FindAllCallbackArgs
{
    t_key_value_hash search_keys;

    std::map<std::string /*id*/, t_key_value_hash /*search_keys*/> result;
};
static bool callback_func__find_all(const std::string& id, const t_key_value_hash& row_content, void* args)
{
    FindAllCallbackArgs* pargs = (FindAllCallbackArgs*)(args);
    for (t_key_value_hash::const_iterator iter = pargs->search_keys.begin();
        iter != pargs->search_keys.end(); ++iter)
    {
         const std::string& search_key = iter->first;
         const std::string& search_value = iter->second;

         t_key_value_hash::const_iterator row_content_iter = row_content.find(search_key);

         // compare nil value.    -- s.find({a: '3', b: nil})   when compile b: nil
         if (search_value == MatxRow::null_string)
         {
             if (row_content_iter != row_content.end() && row_content_iter->second != search_value)
                 return GMatx::t_continue;
             continue;
         }

         // compare value not nil
         if (row_content_iter == row_content.end() || row_content_iter->second != search_value)
             return GMatx::t_continue;
    }
    pargs->result[id] = row_content;
    return GMatx::t_continue;
}

static VALUE t_find_all(VALUE self, VALUE search_keys_ruby_hash)
{
    Check_Type(search_keys_ruby_hash, T_HASH);

    struct FindAllCallbackArgs args;
    args.search_keys = MatxHashUtil::rb_hash_to_key_value_hash(search_keys_ruby_hash);

    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->each_call(callback_func__find_all, &args);

    // change result to ruby Array: [['234232', {'name' => 'haha', 'hp' => '23425'}], [], ...]
    VALUE ret_ary = rb_ary_new();
    for (std::map<std::string, t_key_value_hash>::const_iterator iter = args.result.begin();
        iter != args.result.end(); ++iter)
    {
        const std::string& id = iter->first;
        const t_key_value_hash& row = iter->second;

         VALUE sub_ary = rb_ary_new();
         rb_ary_push(sub_ary, rb_str_new_cstr(id.c_str()));
         rb_ary_push(sub_ary, MatxHashUtil::key_value_hash_to_ruby_hash(row));

         rb_ary_push(ret_ary, sub_ary);
    }

    return ret_ary;
}



static bool callback_func__each(const std::string& key, const t_key_value_hash& row_content, void* args)
{
    rb_yield_values(2, rb_str_new_cstr(key.c_str()), MatxHashUtil::key_value_hash_to_ruby_hash(row_content));
    return GMatx::t_continue;
}

static VALUE t_each(VALUE self)
{
    //raise '' if block do not giving
    //raise '' if count of block params is not 2
//    if (rb_block_arity() != 2)
//      rb_raise(rb_eTypeError, "count of block params must 2");

    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->each_call(callback_func__each, NULL);
    return self;
}

static bool callback_func__all(const std::string& key, const t_key_value_hash& row_content, void* args)
{
    VALUE* p_ret_hash = (VALUE*)(args);
    rb_hash_aset(*p_ret_hash, rb_str_new_cstr(key.c_str()), MatxHashUtil::key_value_hash_to_ruby_hash(row_content));
    return GMatx::t_continue;
}

static VALUE t_all(VALUE self)
{
    VALUE ret_hash = rb_hash_new();

    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->each_call(callback_func__all, &ret_hash);
    return ret_hash;
}

static bool callback_func__ids(const std::string& key, const t_key_value_hash& row_content, void* args)
{
    VALUE* p_ret_array = (VALUE*)(args);
    rb_ary_push(*p_ret_array, rb_str_new_cstr(key.c_str()));
    return GMatx::t_continue;
}

static VALUE t_ids(VALUE self)
{
    VALUE ret_array = rb_ary_new();

    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    pMatx->each_call(callback_func__ids, &ret_array);
    return ret_array;
}

static VALUE t_keys(VALUE self)
{
    VALUE ret_array = rb_ary_new();

    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);
    const std::vector<std::string>& titles = pMatx->get_titles();
    for (size_t i = 0; i < titles.size(); ++i)
    {
        const std::string& title = titles[i];
        rb_ary_push(ret_array, rb_str_new_cstr(title.c_str()));
    }
    return ret_array;
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

static VALUE t_first(VALUE self)
{
    class GMatx* pMatx = NULL;
    Data_Get_Struct(self, class GMatx, pMatx);

    std::string id;
    t_key_value_hash row_hash = pMatx->first(id);
    if (id.empty())
        return Qnil;
    VALUE ret_array = rb_ary_new();
    rb_ary_push(ret_array, rb_str_new_cstr(id.c_str()));
    rb_ary_push(ret_array, MatxHashUtil::key_value_hash_to_ruby_hash(row_hash));

    return ret_array;
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
    rb_define_method(cSMatrix, "find", (VALUE(*)(ANYARGS))t_find, 1);
    rb_define_method(cSMatrix, "find_all", (VALUE(*)(ANYARGS))t_find_all, 1);
    rb_define_method(cSMatrix, "each", (VALUE(*)(ANYARGS))t_each, 0);
    rb_define_method(cSMatrix, "first", (VALUE(*)(ANYARGS))t_first, 0);
    rb_define_method(cSMatrix, "all", (VALUE(*)(ANYARGS))t_all, 0);
    rb_define_method(cSMatrix, "ids", (VALUE(*)(ANYARGS))t_ids, 0);
    rb_define_method(cSMatrix, "keys", (VALUE(*)(ANYARGS))t_keys, 0);
}
