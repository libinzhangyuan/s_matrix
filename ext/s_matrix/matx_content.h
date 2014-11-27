#ifndef _MATX_CONTENT_H_
#define _MATX_CONTENT_H_

#include <string>
#include <map>
#include "matx_row.h"
#include "matx_type_def.h"

class MatxContent
{
public:
    MatxContent(void);

    // 按照titles_for_order中的顺序，格式化row_content中的内容，来设置新行
    void set_row_with_order(const std::string& id, const t_key_value_hash& row_content, const std::vector<std::string>& titles_for_order);

    const MatxRow& get_row(const std::string& id) const;
    void set_row(const std::string& id, const MatxRow& row);

    const std::map<std::string /* id */, MatxRow>::const_iterator begin(void) const;
    const std::map<std::string /* id */, MatxRow>::const_iterator end(void) const;

    std::string to_s(const std::vector<std::string>& titles) const;
    size_t size(void) const;

private:
    std::map<std::string /* id */, MatxRow> m_rows; // each row of this matrix
};



#endif // _MATX_CONTENT_H_
