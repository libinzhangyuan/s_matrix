#include <assert.h>
#include <sstream>

#include "gmatx.h"
#include "stdio.h"


GMatx::GMatx(void)
{
}

void GMatx::add_row(const std::string& id, const t_key_value_hash& row_content)
{
    // 处理新增的title
    // deal the new title
    //
    for (t_key_value_hash::const_iterator iter = row_content.begin();
            iter != row_content.end(); ++iter)
    {
        const std::string& title = iter->first;
        if (m_titles.is_title_exist(title) == false)
            m_titles.add_title(title);
    }

    // 加到content中    add this row to content.
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

    return row.to_key_value_hash(titles);
}

// typedef void (*each_call_func)(const std::string& /*key*/, const t_key_value_hash& /*row_content*/, void* args);
void GMatx::each_call(each_call_func func, void* args) const
{
    const std::vector<std::string>& titles = m_titles.get_titles();
    for (std::map<std::string /* id */, MatxRow>::const_iterator iter = m_contents.begin();
        iter != m_contents.end(); ++iter)
    {
        const std::string& id = iter->first;
        const MatxRow& row = iter->second;
        func(id, row.to_key_value_hash(titles), args);
    }
}

std::string GMatx::to_s(void) const
{
    std::ostringstream ret;
    ret << "GMatx: " << m_contents.size() << '\n';

    const std::vector<std::string>& titles = m_titles.get_titles();
    ret << m_contents.to_s(titles);
    return ret.str();
}

size_t GMatx::size(void) const
{
    return m_contents.size();
}
