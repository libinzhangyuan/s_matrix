#include <sstream>

#include "matx_hash_util.h"
#include "stdio.h"
#include "matx_row.h"

namespace MatxHashUtil
{

    VALUE key_value_hash_to_ruby_hash(const t_key_value_hash& row_hash)
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

    int hash_iter_func(VALUE key, VALUE value, VALUE p_row_hash)
    {
        t_key_value_hash* p_hash = (t_key_value_hash*)p_row_hash;

        // key
        VALUE key_str = rb_funcall(key, rb_intern("to_s"), 0);

        // value
        std::string str_value;
        if (TYPE(value) == T_NIL)
            str_value = MatxRow::null_string;
        else
        {
            VALUE rb_str = rb_funcall(value, rb_intern("to_s"), 0);
            str_value = StringValueCStr( rb_str );
        }

        (*p_hash)[StringValueCStr(key_str)] = str_value;
        return ST_CONTINUE;
    }

    t_key_value_hash rb_hash_to_key_value_hash(VALUE rb_hash)
    {
        t_key_value_hash row_hash;
        rb_hash_foreach(rb_hash, (int (*)(ANYARGS))hash_iter_func, (VALUE)&row_hash);
        return row_hash;
    }

    std::string key_value_hash_to_str(const t_key_value_hash& row_hash)
    {
        std::ostringstream ostr;
        ostr << "--row_hash: size = " << row_hash.size() << std::endl;
        for (t_key_value_hash::const_iterator iter = row_hash.begin(); iter!= row_hash.end(); ++iter)
            ostr << iter->first << " -> " << iter->second << std::endl;
        ostr << std::endl;
        return ostr.str();
    }
}
