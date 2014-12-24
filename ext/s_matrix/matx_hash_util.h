#ifndef _MATX_HASH_UTIL_H_
#define _MATX_HASH_UTIL_H_

#include <string>
#include <map>
#include "matx_type_def.h"

#include "ruby.h"

namespace MatxHashUtil
{
    VALUE key_value_hash_to_ruby_hash(const t_key_value_hash& row_hash);
    t_key_value_hash rb_hash_to_key_value_hash(VALUE rb_hash);
}



#endif // _MATX_HASH_UTIL_H_
