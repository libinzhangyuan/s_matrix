#include "matx_title.h"
#include <algorithm>
#include <stdio.h>


MatxTitle::MatxTitle(void)
{
}


void MatxTitle::add_title(const std::string& title)
{
    m_titles.push_back(title);
}

const std::vector<std::string>& MatxTitle::get_titles(void) const
{
    return m_titles;
}

const std::string& MatxTitle::operator[](size_t index) const
{
    return m_titles[index];
}

bool MatxTitle::is_title_exist(const std::string& title) const
{
    std::vector<std::string>::const_iterator iter = find(m_titles.begin(), m_titles.end(), title);
    return (iter != m_titles.end());
}
