#include <assert.h>
#include <sstream>

#include "gmatx.h"
#include "stdio.h"


GMatx::GMatx(void)
{
    printf("---GMatx new\n");
}

void GMatx::add_row(const std::string& id, const t_key_value_hash& row_content)
{
    // 处理新增的title
    //
    for (t_key_value_hash::const_iterator iter = row_content.begin();
            iter != row_content.end(); ++iter)
    {
        const std::string& title = iter->first;
        if (m_titles.is_title_exist(title) == false)
            m_titles.add_title(title);
    }

    // 加到content中
    //
    m_contents.set_row_with_order(id, row_content, m_titles.get_titles());
}

t_key_value_hash GMatx::get_row(const std::string& id) const
{
    t_key_value_hash result;
    const std::vector<std::string>& titles = m_titles.get_titles();
    const MatxRow& row = m_contents.get_row(id);
    if (&row == &MatxRow::null_row)
        return result;

    const std::vector<std::string>& values = row.get_values();
    assert(titles.size() >= values.size());

    for (size_t i = 0; i < titles.size(); ++i)
    {
        const std::string& title = titles[i];
        if (i < values.size())
            result[title] = values[i];
        else
            result[title] = MatxRow::null_string;
    }
    return result;
}


std::string GMatx::to_s(void) const
{
    std::ostringstream ret;
    ret << "GMatx: " << m_contents.size() << '\n';

    const std::vector<std::string>& titles = m_titles.get_titles();
    ret << m_contents.to_s(titles);
    return ret.str();
}
