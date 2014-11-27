#include <sstream>

#include "matx_content.h"
#include "stdio.h"


MatxContent::MatxContent(void)
{
}

const MatxRow& MatxContent::get_row(const std::string& id) const
{
    std::map<std::string, MatxRow>::const_iterator iter = m_rows.find(id);
    if (iter == m_rows.end())
        return MatxRow::null_row;

    return iter->second;
}

const std::map<std::string /* id */, MatxRow>::const_iterator MatxContent::begin(void) const
{
    return m_rows.begin();
}

const std::map<std::string /* id */, MatxRow>::const_iterator MatxContent::end(void) const
{
    return m_rows.end();
}

size_t MatxContent::size(void) const
{
    return m_rows.size();
}

void MatxContent::set_row(const std::string& id, const MatxRow& row)
{
    m_rows[id] = row;
}

std::string MatxContent::to_s(const std::vector<std::string>& titles) const
{
    std::ostringstream ret;
    std::map<std::string, MatxRow>::const_iterator iter;
    for (iter = m_rows.begin(); iter != m_rows.end(); ++iter)
    {
        const std::string& id = iter->first;
        const MatxRow& row = iter->second;

        ret << "  \"" << id << "\"\n";
        
        const std::vector<std::string>& values = row.get_values();
        for (size_t i = 0; i < values.size(); ++i)
        {
            const std::string& value = values[i];
            std::string value_show = (value == MatxRow::null_string ? "nil" : value);
            ret << "    -- \"" << titles[i] << "\" \"" << value_show << "\"\n";
        }

    }
    return ret.str();
}

void MatxContent::set_row_with_order(const std::string& id, const t_key_value_hash& row_content, const std::vector<std::string>& titles_for_order)
{
    // 根据titles_for_order的顺序，组装出value
    std::vector<std::string> values;
    for (std::vector<std::string>::const_iterator iter = titles_for_order.begin();
            iter != titles_for_order.end(); ++iter)
    {
        const std::string& title = *iter;
        t_key_value_hash::const_iterator content_iter = row_content.find(title);
        if (content_iter == row_content.end())
        {
            values.push_back(MatxRow::null_string); // 没有的项目，使用nil字符串来保存
        }
        else
            values.push_back(content_iter->second);
    }

    // 保存行
    m_rows[id] = MatxRow(values);
}
