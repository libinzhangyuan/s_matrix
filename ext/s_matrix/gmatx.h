#ifndef _GMATX_H_
#define _GMATX_H_

#include "matx_title.h"
#include "matx_content.h"
#include "matx_type_def.h"

class GMatx
{
public:
    GMatx(void);

    void add_row(const std::string& id, const t_key_value_hash& row_content);

    // 当找不到id所对应的行时，返回的t_key_value_hash数据的size=0
    t_key_value_hash get_row(const std::string& id) const;
    std::string to_s(void) const;

private:
    MatxTitle m_titles;
    MatxContent m_contents;
};

#endif
